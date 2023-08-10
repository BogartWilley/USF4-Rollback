#include <utility>
#include <vector>
#include <windows.h>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include <ggponet.h>

#include "../Dimps/Dimps.hxx"
#include "../Dimps/Dimps__Event.hxx"
#include "../Dimps/Dimps__GameEvents.hxx"
#include "../Dimps/Dimps__Pad.hxx"
#include "../sf4e/sf4e__Game__Battle__System.hxx"
#include "../sf4e/sf4e__GameEvents.hxx"

#include "sf4e__SessionProtocol.hxx"
#include "sf4e__SessionServer.hxx"

using nlohmann::json;

namespace SessionProtocol = sf4e::SessionProtocol;
using Dimps::Event::EventBase;
using Dimps::Event::EventController;
using rVsMode = Dimps::GameEvents::VsMode;
using fMainMenu = sf4e::GameEvents::MainMenu;
using fSystem = sf4e::Game::Battle::System;
using fVsMode = sf4e::GameEvents::VsMode;
using fVsPreBattle = sf4e::GameEvents::VsPreBattle;
using sf4e::SessionServer;


const int sf4e::SESSION_SERVER_MAX_MESSAGES_PER_POLL = 200;
SessionServer* SessionServer::s_pCallbackInstance;

SessionServer::SessionServer(uint16 nPort, std::string sidecarHash) :
	_sidecarHash(sidecarHash),
	_interface(SteamNetworkingSockets()),
	_lobbyData(),
	_matchData(),
	_dataDirty(false)
{

	clients.reserve(MAX_SF4E_PROTOCOL_USERS + 1);
	_pollGroup = _interface->CreatePollGroup();

	// Start listening
	SteamNetworkingIPAddr serverLocalAddr;
	serverLocalAddr.Clear();
	serverLocalAddr.m_port = nPort;
	SteamNetworkingConfigValue_t opt;
	opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);
	_listenSock = _interface->CreateListenSocketIP(serverLocalAddr, 1, &opt);
	if (_listenSock == k_HSteamListenSocket_Invalid) {
		spdlog::error("Failed to listen on port {}", nPort);
		return;
	}
	spdlog::info("Server listening on port {}", nPort);
}

SessionServer::~SessionServer()
{
	if (_listenSock != k_HSteamListenSocket_Invalid) {
		Close();
		_listenSock = k_HSteamListenSocket_Invalid;
	}
}

int SessionServer::Step()
{
	if (_listenSock == k_HSteamListenSocket_Invalid) {
		return -1;
	}

	ISteamNetworkingMessage* pIncomingMsgs[SESSION_SERVER_MAX_MESSAGES_PER_POLL] = { 0 };
	int numMsgs = _interface->ReceiveMessagesOnPollGroup(_pollGroup, pIncomingMsgs, SESSION_SERVER_MAX_MESSAGES_PER_POLL);

	if (numMsgs < 0) {
		spdlog::error("Session server error checking for messages: {}", numMsgs);
		return -1;
	}

	for (int i = 0; i < numMsgs; i++) {
		ISteamNetworkingMessage* pIncomingMsg = pIncomingMsgs[i];
		if (!pIncomingMsg) {
			spdlog::error("Client: incoming message enumerated, but not data retrieved");
			return -1;
		}

		HSteamNetConnection conn = pIncomingMsg->m_conn;
		const char* start = (const char*)pIncomingMsg->m_pData;
		json msg;

		try {
			msg = json::parse(start, start + pIncomingMsg->m_cbSize);
		}
		catch (json::exception e) {
			spdlog::info("Server: got a non-json message");
			continue;
		}
		

		SessionProtocol::MessageType type;
		try {
			msg.at("type").get_to(type);
		}
		catch (json::exception e) {
			spdlog::info("Server: got a message without a type, or a type that was not a string");
			continue;
		}

		if (type == SessionProtocol::MT_JOIN_REQ) {
			SessionProtocol::JoinRequest request;
			try {
				msg.get_to(request);
			}
			catch (json::exception e) {
				spdlog::info("Server: could not deserialize join request");
				SessionProtocol::JoinReject reject;
				reject.result = SessionProtocol::JR_REQUEST_INVALID;
				json rejectMsg = reject;
				Respond(conn, rejectMsg);
				continue;
			}

			SteamNetworkingIPAddr peerAddr = *(pIncomingMsg->m_identityPeer.GetIPAddr());
			SessionProtocol::JoinResult joinResult = RegisterToWait(conn, request.port, request.sidecarHash, request.username, peerAddr);
			if (joinResult != SessionProtocol::JOIN_OK) {
				spdlog::info("Server: rejecting registration for reason {}", joinResult);
				SessionProtocol::JoinReject reject;
				reject.result = joinResult;
				json rejectMsg = reject;
				Respond(conn, rejectMsg);
				continue;
			}

			_dataDirty = true;
		}
		else if (type == SessionProtocol::MT_SET_CONDITIONS) {
			int side = -1;
			for (int i = 0; i < 2; i++) {
				if (clients.size() > i && clients.at(i).conn == conn) {
					side = i;
					break;
				}
			}
			if (side == -1) {
				spdlog::info("Server: sender {} tried to update conditions, but is not playing", conn);
				continue;
			}

			SessionProtocol::SetConditionsRequest request;
			try {
				msg.get_to(request);
			}
			catch (json::exception e) {
				spdlog::info("Server: could not deserialize SetConditionsRequest");
				continue;
			}

			_matchData.readyMessageNum[side] = pIncomingMsg->GetMessageNumber();
			_matchData.chara[side] = request.chara;
			if (side == 0) {
				_matchData.rngSeed = request.rngSeed;
				_matchData.stageID = request.stageID;
			}
			_dataDirty = true;
		}
		else if (type == SessionProtocol::MT_REPORT_RESULTS) {
			SessionProtocol::ReportResultsRequest request;
			try {
				msg.get_to(request);
			}
			catch (json::exception e) {
				spdlog::info("Server: could not deserialize ReportResultsRequest");
				continue;
			}

			HandleResults(request.loserSide);
			_dataDirty = true;
		}
		else if (type == SessionProtocol::MT_SNAPSHOT) {
			// Forward the snapshot to every other client.
			for (auto clientIter = clients.begin(); clientIter != clients.end(); clientIter++) {
				if (clientIter->conn != conn) {
					_interface->SendMessageToConnection(
						clientIter->conn, (const char*)pIncomingMsg->m_pData, pIncomingMsg->m_cbSize,
						k_nSteamNetworkingSend_Reliable, nullptr
					);
				}
			}
		}
		else {
			spdlog::warn("Server: got unrecognized message type: {}", type);
		}
	}

	for (int i = 0; i < numMsgs; i++) {
		ISteamNetworkingMessage* pIncomingMsg = pIncomingMsgs[i];
		if (pIncomingMsg) {
			pIncomingMsg->Release();
		}
	}

	// XXX (adanducci) replace SendMessageToConnection with SendMessages for
	// peformance gains, but ensuring low-copy with it is annoyingly difficult.
	if (_dataDirty) {
		json dataUpdateMsg;
		dataUpdateMsg["type"] = SessionProtocol::MT_DATA_UPDATE;
		dataUpdateMsg["lobbyData"] = clients;
		dataUpdateMsg["matchData"] = _matchData;
		std::string buf = dataUpdateMsg.dump();

		for (auto clientIter = clients.begin(); clientIter != clients.end(); clientIter++) {
			_interface->SendMessageToConnection(
				clientIter->conn, buf.c_str(), (uint32)buf.length(),
				k_nSteamNetworkingSend_Reliable, nullptr
			);
		}
		_dataDirty = false;
	}

	return 0;
}

void SessionServer::Respond(HSteamNetConnection client, nlohmann::json& msg) {
	std::string buf = msg.dump();
	_interface->SendMessageToConnection(
		client, buf.c_str(), (uint32)buf.length(),
		k_nSteamNetworkingSend_Reliable, nullptr
	);
}

int SessionServer::Close()
{
	spdlog::info("Closing connections...");
	// Close all connections.  We use "linger mode" to ask SteamNetworkingSockets
	// to flush this out and close gracefully.
	for (auto iter = clients.begin(); iter != clients.end(); iter++) {
		_interface->SetConnectionPollGroup(iter->conn, k_HSteamNetPollGroup_Invalid);
		_interface->CloseConnection(iter->conn, 0, "Server Shutdown", true);
	}
	clients.clear();
	_interface->DestroyPollGroup(_pollGroup);
	_pollGroup = k_HSteamNetPollGroup_Invalid;
	_interface->CloseListenSocket(_listenSock);
	_listenSock = k_HSteamListenSocket_Invalid;
	return 0;
}

void SessionServer::PrepareForCallbacks()
{
	s_pCallbackInstance = this;
}

void SessionServer::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	switch (pInfo->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
	{
		// Ignore if they were not previously connected.  (If they disconnected
		// before we accepted the connection.)
		if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected) {
			// Select appropriate log messages
			const char* pszDebugLogAction;
			if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
			{
				pszDebugLogAction = "problem detected locally";
			}
			else
			{
				// Note that here we could check the reason code to see if
				// it was a "usual" connection or an "unusual" one.
				pszDebugLogAction = "closed by peer";
			}

			spdlog::info("Connection {} {}, reason {}: {}",
				pInfo->m_info.m_szConnectionDescription,
				pszDebugLogAction,
				pInfo->m_info.m_eEndReason,
				pInfo->m_info.m_szEndDebug
			);

			for (auto iter = clients.begin(); iter != clients.end(); iter++) {
				if (iter->conn == pInfo->m_hConn) {
					clients.erase(iter);
					_dataDirty = true;
					break;
				}
			}

			// Clean up the connection.  This is important!
			// The connection is "closed" in the network sense, but
			// it has not been destroyed.  We must close it on our end, too
			// to finish up.  The reason information do not matter in this case,
			// and we cannot linger because it's already closed on the other end,
			// so we just pass 0's.
			_interface->SetConnectionPollGroup(pInfo->m_hConn, k_HSteamNetPollGroup_Invalid);
			_interface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		}

		break;
	}

	case k_ESteamNetworkingConnectionState_Connecting:
	{
		// Try to accept the connection.
		if (_interface->AcceptConnection(pInfo->m_hConn) != k_EResultOK)
		{
			// This could fail.  If the remote host tried to connect, but then
			// disconnected, the connection may already be half closed.  Just
			// destroy whatever we have on our side.
			_interface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
			spdlog::error("Can't accept connection.  (It was already closed?)");
			break;
		}

		_interface->SetConnectionPollGroup(pInfo->m_hConn, _pollGroup);
	}

	default:
		break;
	}
}

void SessionServer::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	s_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
}

SessionProtocol::JoinResult SessionServer::RegisterToWait(const HSteamNetConnection& conn, const uint16_t& port, const std::string& sidecarHash, const std::string& name, const SteamNetworkingIPAddr& peerAddr) {
	if (sidecarHash != _sidecarHash) {
		return SessionProtocol::JR_HASH_INVALID;
	}

	if (clients.size() >= MAX_SF4E_PROTOCOL_USERS) {
		return SessionProtocol::JR_LOBBY_FULL;
	}

	for (auto iter = clients.begin(); iter != clients.end(); iter++) {
		if (iter->data.name == name) {
			return SessionProtocol::JR_NAME_TAKEN;
		}
	}

	char peerAddrStr[SteamNetworkingIPAddr::k_cchMaxString];
	if (peerAddr.IsLocalHost()) {
		peerAddrStr[0] = 0;
	}
	else {
		peerAddr.ToString(peerAddrStr, SteamNetworkingIPAddr::k_cchMaxString, false);
	}
	SessionMember newMember{ {name, peerAddrStr, port}, conn };
	clients.push_back(std::move(newMember));
	return SessionProtocol::JOIN_OK;
}

void SessionServer::HandleResults(int loserIndex) {
	auto loser = clients.begin() + loserIndex;
	clients.push_back(*loser);
	clients.erase(loser);
	_matchData.Clear();
}

void sf4e::from_json(const nlohmann::json& j, SessionServer::SessionMember& m) {
	j.get_to(m.data);
}

void sf4e::to_json(nlohmann::json& j, const SessionServer::SessionMember& m) {
	j = m.data;
}