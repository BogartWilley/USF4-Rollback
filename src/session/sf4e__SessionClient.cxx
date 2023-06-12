#include <string>

#include <windows.h>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

#include "../Dimps/Dimps.hxx"
#include "../Dimps/Dimps__Event.hxx"
#include "../Dimps/Dimps__GameEvents.hxx"
#include "../Dimps/Dimps__Pad.hxx"

#include "../sf4e/sf4e__GameEvents.hxx"
#include "../sf4e/sf4e__Game__Battle__System.hxx"

#include "sf4e__SessionClient.hxx"
#include "sf4e__SessionProtocol.hxx"

using nlohmann::json;

namespace SessionProtocol = sf4e::SessionProtocol;
using Dimps::Event::EventBase;
using Dimps::Event::EventController;
using rVsMode = Dimps::GameEvents::VsMode;
using fMainMenu = sf4e::GameEvents::MainMenu;
using fSystem = sf4e::Game::Battle::System;
using fVsMode = sf4e::GameEvents::VsMode;
using fVsPreBattle = sf4e::GameEvents::VsPreBattle;
using sf4e::SessionClient;

SessionClient* SessionClient::s_pCallbackInstance;


SessionClient::SessionClient(const SteamNetworkingIPAddr& serverAddr)
{
	// Select instance to use.  For now we'll always use the default.
	m_pInterface = SteamNetworkingSockets();

	// Start connecting
	char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
	serverAddr.ToString(szAddr, sizeof(szAddr), true);
	spdlog::info("Connecting to chat server at {}", szAddr);
	SteamNetworkingConfigValue_t opt;
	opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);
	m_hConnection = m_pInterface->ConnectByIPAddress(serverAddr, 1, &opt);
	if (m_hConnection == k_HSteamNetConnection_Invalid) {
		spdlog::error("Client failed to create connection");
	}
}

void SessionClient::Step()
{
	PollIncomingMessages();
	PollConnectionStateChanges();
	SendQueuedMessages();
}

void SessionClient::SendQueuedMessages() {
	std::string buf;

	for (auto iter = queuedMessages.begin(); iter != queuedMessages.end(); iter++) {
		buf = iter->dump();
		m_pInterface->SendMessageToConnection(
			m_hConnection, buf.c_str(), (uint32)buf.length(),
			k_nSteamNetworkingSend_Reliable, nullptr
		);
	}

	queuedMessages.clear();
}

void SessionClient::PollIncomingMessages()
{
	ISteamNetworkingMessage* pIncomingMsgs[SESSION_CLIENT_MAX_MESSAGES_PER_POLL] = { 0 };
	int numMsgs = m_pInterface->ReceiveMessagesOnConnection(m_hConnection, pIncomingMsgs, SESSION_CLIENT_MAX_MESSAGES_PER_POLL);

	if (numMsgs < 0) {
		spdlog::error("Session client error checking for messages: {}", numMsgs);
		return;
	}

	for (int i = 0; i < numMsgs; i++) {
		ISteamNetworkingMessage* pIncomingMsg = pIncomingMsgs[i];
		if (!pIncomingMsg) {
			spdlog::error("Client: incoming message enumerated, but not data retrieved");
			return;
		}

		const char* start = (const char*)pIncomingMsg->m_pData;
		json msg = json::parse(start, start + pIncomingMsg->m_cbSize);
		SteamNetworkingIPAddr peerAddr = *(pIncomingMsg->m_identityPeer.GetIPAddr());
		pIncomingMsg->Release();

		std::string type;
		try {
			msg.at("type").get_to(type);
		}
		catch (json::exception e) {
			spdlog::info("Client: got a message without a type, or a type that was not a string");
			continue;
		}

		if (type == "response") {
			// Since handling a request forces the process to load into a battle,
			// handling the request can only reasonably be done if the process is
			// currently on the main menu.
			if (fMainMenu::instance == NULL) {
				spdlog::info("Client: ignoring request because we're not on the main menu");
				continue;
			}

			SessionProtocol::SessionResponse response;
			try {
				msg.get_to(response);
			}
			catch (json::exception e) {
				spdlog::info("Client: could not deserialize response");
				continue;
			}

			// Hack to configure inputs
			Dimps::Pad::System* padSys = Dimps::Pad::System::staticMethods.GetSingleton();
			Dimps::Pad::System::__publicMethods& padSysMethods = Dimps::Pad::System::publicMethods;
			(padSys->*padSysMethods.AssociatePlayerAndGamepad)(0, 0);
			(padSys->*padSysMethods.SetDeviceTypeForPlayer)(0, 1);
			(padSys->*padSysMethods.SetSideHasAssignedController)(0, 1);
			(padSys->*padSysMethods.AssociatePlayerAndGamepad)(1, 1);
			(padSys->*padSysMethods.SetDeviceTypeForPlayer)(1, 1);
			(padSys->*padSysMethods.SetSideHasAssignedController)(1, 1);
			(padSys->*padSysMethods.SetActiveButtonMapping)(Dimps::Pad::System::BUTTON_MAPPING_FIGHT);

			size_t charaConditionSize = sizeof(rVsMode::ConfirmedCharaConditions);
			memcpy_s(&confirmedConditions[0], charaConditionSize, &response.chara, charaConditionSize);
			confirmedStageID = response.stageID;
			s_pCallbackInstance = this;
			fVsPreBattle::bSkipToVersus = true;
			fVsPreBattle::OnTasksRegistered = _OnVsPreBattleTasksRegistered;
			fMainMenu::GoToVersusBattle();

			// Start the GGPO connection
			fSystem::StartGGPO(0, &peerAddr);
		}
		else {
			spdlog::warn("Server: got unrecognized message type: {}", type);
		}
	}
}

void SessionClient::_OnVsPreBattleTasksRegistered()
{
	SessionClient* _this = s_pCallbackInstance;
	size_t charaConditionSize = sizeof(rVsMode::ConfirmedCharaConditions);

	// XXX (adanducci): this is fragile- passing in the VsPreBattle event and
	// traversing the parent events would avoid the need to track state that
	// could potentially interleave with other event construction and
	// destruction.
	rVsMode* mode = fVsMode::instance;
	Dimps::Platform::dString* stageName = rVsMode::GetStageName(mode);
	rVsMode::ConfirmedPlayerConditions* conditions = rVsMode::GetConfirmedPlayerConditions(mode);
	for (int i = 0; i < 2; i++) {
		*(rVsMode::ConfirmedPlayerConditions::GetCharaID(&conditions[i])) = _this->confirmedConditions[i].charaID;
		*(rVsMode::ConfirmedPlayerConditions::GetSideActive(&conditions[i])) = 1;
		rVsMode::ConfirmedCharaConditions* charaConditions = rVsMode::ConfirmedPlayerConditions::GetCharaConditions(&conditions[i]);
		memcpy_s(charaConditions, charaConditionSize, &_this->confirmedConditions[i], charaConditionSize);
	}

	(stageName->*Dimps::Platform::dString::publicMethods.assign)(Dimps::stageCodes[_this->confirmedStageID], 4);
	*(rVsMode::GetStageCode(mode)) = _this->confirmedStageID;
}

void SessionClient::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{

	switch (pInfo->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
	{
		// Print an appropriate message
		if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
		{
			spdlog::error("Client could not connect: {}", pInfo->m_info.m_szEndDebug);
			MessageBoxA(NULL, "Client: could not connect- maybe wrong IP or no forwarding", NULL, MB_OK);
		}
		else if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
		{
			spdlog::error("Client lost contact with host: {}", pInfo->m_info.m_szEndDebug);
			MessageBoxA(NULL, "Client: Problem detected locally- lost contact with host", NULL, MB_OK);
		}

		// Clean up the connection.  This is important!
		// The connection is "closed" in the network sense, but
		// it has not been destroyed.  We must close it on our end, too
		// to finish up.  The reason information do not matter in this case,
		// and we cannot linger because it's already closed on the other end,
		// so we just pass 0's.
		m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		m_hConnection = k_HSteamNetConnection_Invalid;
		break;
	}
	case k_ESteamNetworkingConnectionState_Connected:
	{
		spdlog::info("Client connected to server OK");
		size_t charaConditionSize = sizeof(rVsMode::ConfirmedCharaConditions);
		SessionProtocol::SessionRequest request;
		memcpy_s(&request.chara, charaConditionSize, &myConditions, charaConditionSize);
		memcpy_s(&confirmedConditions[1], charaConditionSize, &myConditions, charaConditionSize);
		json msg = request;
		queuedMessages.push_back(msg);
		break;
	}

	default:
		break;
	}
}

void SessionClient::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	s_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
}

void SessionClient::PollConnectionStateChanges()
{
	s_pCallbackInstance = this;
	m_pInterface->RunCallbacks();
}
