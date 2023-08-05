#include <string>
#include <utility>

#include <windows.h>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include <ggponet.h>

#include "../Dimps/Dimps.hxx"
#include "../Dimps/Dimps__Event.hxx"
#include "../Dimps/Dimps__GameEvents.hxx"
#include "../Dimps/Dimps__Game__Battle__System.hxx"
#include "../Dimps/Dimps__Pad.hxx"

#include "../sf4e/sf4e__GameEvents.hxx"
#include "../sf4e/sf4e__Game__Battle__System.hxx"

#include "sf4e__SessionClient.hxx"
#include "sf4e__SessionProtocol.hxx"

using nlohmann::json;

namespace SessionProtocol = sf4e::SessionProtocol;
using Dimps::Event::EventBase;
using Dimps::Event::EventController;
using rSystem = Dimps::Game::Battle::System;
using rVsMode = Dimps::GameEvents::VsMode;
using fMainMenu = sf4e::GameEvents::MainMenu;
using fSystem = sf4e::Game::Battle::System;
using fVsMode = sf4e::GameEvents::VsMode;
using fVsPreBattle = sf4e::GameEvents::VsPreBattle;
using sf4e::SessionClient;

const int sf4e::SESSION_CLIENT_MAX_MESSAGES_PER_POLL = 20;
SessionClient* SessionClient::s_pCallbackInstance;

SessionClient::SessionClient(
	const SteamNetworkingIPAddr& serverAddr,
	uint16_t port,
	std::string& name,
	uint8_t deviceType,
	uint8_t deviceIdx,
	uint8_t delay
):
	_name(name),
	_port(port),
	_interface(SteamNetworkingSockets()),
	_delay(delay),
	_deviceType(deviceType),
	_deviceIdx(deviceIdx)
{
	char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
	SteamNetworkingConfigValue_t opt;

	serverAddr.ToString(szAddr, sizeof(szAddr), true);
	spdlog::info("Connecting to session server at {}", szAddr);

	opt.SetPtr(
		k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
		(void*)SteamNetConnectionStatusChangedCallback
	);
	_conn = _interface->ConnectByIPAddress(serverAddr, 1, &opt);
	if (_conn == k_HSteamNetConnection_Invalid) {
		spdlog::error("Client failed to create connection");
	}
}

SessionClient::~SessionClient()
{
	if (_conn != k_HSteamNetConnection_Invalid) {
		_interface->CloseConnection(_conn, k_ESteamNetConnectionEnd_App_Generic, nullptr, true);
		_conn = k_HSteamNetConnection_Invalid;
		_connected = false;
	}
	_interface = nullptr;
}

void SessionClient::PrepareForCallbacks()
{
	s_pCallbackInstance = this;
}

int SessionClient::Step()
{
	if (_interface == nullptr || _conn == k_HSteamNetConnection_Invalid) {
		return -1;
	}

	if (!_connected) {
		// Not yet connected- not an error state, but nothing to do.
		return 0;
	}

	ISteamNetworkingMessage* pIncomingMsgs[SESSION_CLIENT_MAX_MESSAGES_PER_POLL] = { 0 };
	int numMsgs = _interface->ReceiveMessagesOnConnection(_conn, pIncomingMsgs, SESSION_CLIENT_MAX_MESSAGES_PER_POLL);

	if (numMsgs < 0) {
		spdlog::error("Session client error checking for messages: {}", numMsgs);
		return -1;
	}

	for (int i = 0; i < numMsgs; i++) {
		ISteamNetworkingMessage* pIncomingMsg = pIncomingMsgs[i];
		if (!pIncomingMsg) {
			spdlog::error("Client: incoming message enumerated, but not data retrieved");
			return -1;
		}

		const char* start = (const char*)pIncomingMsg->m_pData;
		json msg = json::parse(start, start + pIncomingMsg->m_cbSize);
		SteamNetworkingIPAddr peerAddr = *(pIncomingMsg->m_identityPeer.GetIPAddr());
		pIncomingMsg->Release();

		SessionProtocol::MessageType type;
		try {
			msg.at("type").get_to(type);
		}
		catch (json::exception e) {
			spdlog::info("Client: got a message without a type, or a type that was not a string");
			continue;
		}

		if (type == SessionProtocol::MT_JOIN_REJ) {
			SessionProtocol::JoinReject reject;
			try {
				msg.get_to(reject);
			}
			catch (json::exception e) {
				spdlog::info("Client: couldn't deserialize join rejection?");
				continue;
			}

			spdlog::info("Join rejected, reason: {}", reject.result);
			_interface->CloseConnection(_conn, 0, nullptr, false);
			_conn = k_HSteamNetConnection_Invalid;
			return -1;
		}
		else if (type == SessionProtocol::MT_DATA_UPDATE) {
			SessionProtocol::DataUpdate update;
			try {
				msg.get_to(update);
			}
			catch (json::exception e) {
				spdlog::info("Client: could not deserialize response");
				continue;
			}
			_lobbyData = update.lobbyData;
			_matchData = update.matchData;

			if (_outstandingReadyRequestNumber > -1) {
				for (int i = 0; i < _lobbyData.size() && i < 2; i++) {
					if (_lobbyData[i].name == _name) {
						if (_matchData.readyMessageNum[i] == _outstandingReadyRequestNumber) {
							// This contains the ready data, so there's no longer an outstanding request.
							_outstandingReadyRequestNumber = -1;
						}
						break;
					}
				}
			}

			if (_matchData.readyMessageNum[0] > -1 && _matchData.readyMessageNum[1] > -1) {
				// Since handling a request forces the process to load into a battle,
				// handling the request can only reasonably be done if the process is
				// currently on the main menu.
				if (fMainMenu::instance == NULL) {
					spdlog::info("Client: ignoring that both clients are ready because we're not on the main menu");
					continue;
				}

				s_pCallbackInstance = this;
				fVsPreBattle::bSkipToVersus = true;
				fVsPreBattle::OnTasksRegistered = _OnVsPreBattleTasksRegistered;
				fMainMenu::GoToVersusBattle();

				// Start the GGPO connection
				bool isPlayer = false;
				for (int i = 0; i < 2; i++) {
					if (_lobbyData[i].name == _name) {
						isPlayer = true;
						break;
					}
				}
				if (isPlayer) {
					GGPOPlayer players[MAX_SF4E_PROTOCOL_USERS];
					for (int i = 0; i < 2 && i < _lobbyData.size(); i++) {
						SessionProtocol::MemberData& memberData = _lobbyData[i];
						GGPOPlayer& player = players[i];
						player.size = sizeof(GGPOPlayer);
						player.player_num = i + 1;
						if (_lobbyData[i].name == _name) {
							player.type = GGPO_PLAYERTYPE_LOCAL;

							// Inject the chosen device into this player's side
							Dimps::Pad::System* padSys = Dimps::Pad::System::staticMethods.GetSingleton();
							Dimps::Pad::System::__publicMethods& padSysMethods = Dimps::Pad::System::publicMethods;
							(padSys->*padSysMethods.AssociatePlayerAndGamepad)(i, _deviceIdx);
							(padSys->*padSysMethods.SetDeviceTypeForPlayer)(i, _deviceType);
							(padSys->*padSysMethods.SetSideHasAssignedController)(i, 1);
							(padSys->*padSysMethods.SetActiveButtonMapping)(Dimps::Pad::System::BUTTON_MAPPING_FIGHT);
						}
						else {
							SessionProtocol::MemberData& memberData = _lobbyData[i];
							player.type = GGPO_PLAYERTYPE_REMOTE;
							strcpy_s(player.u.remote.ip_address, 32, memberData.ip.c_str());
							player.u.remote.port = memberData.port;
						}
					}
					for (int i = 2; i < _lobbyData.size(); i++) {
						SessionProtocol::MemberData& memberData = _lobbyData[i];
						GGPOPlayer& player = players[i];
						player.type = GGPO_PLAYERTYPE_SPECTATOR;
						strcpy_s(player.u.remote.ip_address, 32, memberData.ip.c_str());
						player.u.remote.port = memberData.port;
					}
					fSystem::StartGGPO(players, _lobbyData.size(), _port, _delay, _matchData.rngSeed);
				}
				else {
					fSystem::StartSpectating(
						_port,
						2,
						// Always spectate from the host for now- the protocol has
						// limited enough players that there's marginal bandwidth
						// differences.
						// 
						// Safe-_ish_ removal of const. This gets passed through
						// to an inet_pton() call and never modified.
						(char*)_lobbyData[0].ip.c_str(),
						_lobbyData[0].port,
						_matchData.rngSeed
					);
				}
			}
		}
		else if (type == SessionProtocol::MT_SNAPSHOT) {
			SessionProtocol::SnapshotMsg m;
			try {
				msg.get_to(m);
			}
			catch (json::exception e) {
				spdlog::info("Client: could not deserialize incoming checksum msg");
				continue;
			}

			auto localSnapshotIter = fSystem::snapshotMap.find(m.snapshot.frameIdx);
			if (localSnapshotIter != fSystem::snapshotMap.end()) {
				// This client is ahead and already has a snapshot for this frame.
				// Compare it.
				SessionProtocol::StateSnapshot& localSnapshot = localSnapshotIter->second.first;
				if (memcmp(&m.snapshot, &localSnapshot, sizeof(SessionProtocol::StateSnapshot)) != 0) {
					spdlog::error("Client: Desync detected!");
					MessageBoxA(NULL, "Desync detected!", NULL, MB_OK);
					*rSystem::GetReadyState(rSystem::staticMethods.GetSingleton()) = rSystem::RS_ISLEAVING;
				}
				localSnapshotIter->second.second.confirmed = true;
				if (localSnapshotIter->second.second.confirmed && localSnapshotIter->second.second.sent) {
					fSystem::snapshotMap.erase(localSnapshotIter);
				}
			}
			else {
				// Opponent's ahead- can't compare yet.
				pendingRemoteSnapshots.emplace(m.snapshot.frameIdx, m.snapshot);
			}
		}
		else {
			spdlog::warn("Client: got unrecognized message type: {}", type);
		}
	}

	// Send all our outstanding local snapshots and compare any to pending
	// snapshots.
	{	
		int mostRecentPredictiveFrame = (
			rSystem::GetNumFramesSimulated_FixedPoint(rSystem::staticMethods.GetSingleton())->integral
		);
		auto localSnapshotIter = fSystem::snapshotMap.begin();
		while (localSnapshotIter != fSystem::snapshotMap.end()) {
			if (mostRecentPredictiveFrame - localSnapshotIter->first < 60) {
				// Snapshot not yet old enough.
				localSnapshotIter++;
				continue;
			}

			if (!localSnapshotIter->second.second.sent) {
				// Snapshot not yet sent. Send it.
				localSnapshotIter->second.second.sent = true;
				SessionProtocol::SnapshotMsg m;
				m.snapshot = localSnapshotIter->second.first;
				json msg = m;
				if (Send(msg, nullptr) != k_EResultOK) {
					spdlog::error("Client: Could not send snapshot update");
				}
			}

			if (!localSnapshotIter->second.second.confirmed) {
				auto remoteSnapshotIter = pendingRemoteSnapshots.find(localSnapshotIter->first);
				if (remoteSnapshotIter != pendingRemoteSnapshots.end()) {
					// Caught up to the opponent- compare to a snapshot already sent by the opponent.
					SessionProtocol::StateSnapshot& localSnapshot = localSnapshotIter->second.first;
					if (memcmp(&remoteSnapshotIter->second, &localSnapshot, sizeof(SessionProtocol::StateSnapshot)) != 0) {
						spdlog::error("Client: Desync detected!");
						MessageBoxA(NULL, "Desync detected!", NULL, MB_OK);
						*rSystem::GetReadyState(rSystem::staticMethods.GetSingleton()) = rSystem::RS_ISLEAVING;
					}
					localSnapshotIter->second.second.confirmed = true;
				}
			}

			if (localSnapshotIter->second.second.confirmed && localSnapshotIter->second.second.sent) {
				localSnapshotIter = fSystem::snapshotMap.erase(localSnapshotIter);
			}
			else {
				localSnapshotIter++;
			}
		}
	}

	return 0;
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
		*(rVsMode::ConfirmedPlayerConditions::GetCharaID(&conditions[i])) = _this->_matchData.chara[i].charaID;
		*(rVsMode::ConfirmedPlayerConditions::GetSideActive(&conditions[i])) = 1;
		rVsMode::ConfirmedCharaConditions* charaConditions = rVsMode::ConfirmedPlayerConditions::GetCharaConditions(&conditions[i]);
		memcpy_s(charaConditions, charaConditionSize, &_this->_matchData.chara[i], charaConditionSize);
	}

	(stageName->*Dimps::Platform::dString::publicMethods.assign)(Dimps::stageCodes[_this->_matchData.stageID], 4);
	*(rVsMode::GetStageCode(mode)) = _this->_matchData.stageID;
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
		_interface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		_conn = k_HSteamNetConnection_Invalid;
		_connected = false;
		break;
	}
	case k_ESteamNetworkingConnectionState_Connected:
	{
		spdlog::info("Client connected to server OK, attempting to join...");
		_connected = true;
		SessionProtocol::JoinRequest request;
		request.username = _name;
		request.port = _port;
		json msg = request;
		if (Send(msg, nullptr) != k_EResultOK) {
			spdlog::warn("Client could send initial join request");
		}
		break;
	}
	default:
		break;
	}
}

EResult SessionClient::Send(nlohmann::json& msg, int64_t* outMessageNum) {
	std::string buf = msg.dump();
	return _interface->SendMessageToConnection(
		_conn, buf.c_str(), (uint32)buf.length(),
		k_nSteamNetworkingSend_Reliable, outMessageNum
	);
}

EResult SessionClient::SetMatchConditions(SessionProtocol::SetConditionsRequest& r)
{
	json msg = r;
	EResult result = Send(msg, &_outstandingReadyRequestNumber);
	if (result != k_EResultOK) {
		spdlog::warn("Client: could not send match conditions! Result: {}", result);
	}
	return result;
}

EResult SessionClient::ReportResults(SessionProtocol::ReportResultsRequest& r)
{
	json msg = r;
	EResult result = Send(msg, nullptr);
	if (result != k_EResultOK) {
		spdlog::warn("Client: could not report results! Result: {}", result);
	}
	return result;
}

void SessionClient::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	s_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
}
