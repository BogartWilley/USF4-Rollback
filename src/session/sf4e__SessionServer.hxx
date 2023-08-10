#pragma once

#include <map>
#include <string>
#include <vector>

#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <nlohmann/json.hpp>

#include "sf4e__SessionProtocol.hxx"

namespace sf4e {
	extern const int SESSION_SERVER_MAX_MESSAGES_PER_POLL;

	class SessionServer
	{
	private:
		// Connection related data
		std::string _sidecarHash;
		HSteamListenSocket _listenSock;
		HSteamNetPollGroup _pollGroup;
		ISteamNetworkingSockets* _interface;

		// Lobby data
		bool _dataDirty;
		SessionProtocol::LobbyData _lobbyData;
		SessionProtocol::MatchData _matchData;

		// Connection callbacks and message utilities
		static SessionServer* s_pCallbackInstance;
		static void _OnVsPreBattleTasksRegistered();
		static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
		void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);
		void BroadcastMessage(nlohmann::json& msg);
		void Respond(HSteamNetConnection client, nlohmann::json& msg);

		// Direct lobby data manipulation utilities
		SessionProtocol::JoinResult RegisterToWait(
			const HSteamNetConnection& conn,
			const uint16_t& port,
			const std::string& sidecarHash,
			const std::string& name,
			const SteamNetworkingIPAddr& peerAddr
		);
		void HandleResults(int loserSide);
		int BroadcastLobbyData();
		int BroadcastMatchData();

	public:
		SessionServer(
			uint16 nPort,
			std::string sidecarHash
		);
		~SessionServer();

		int Step();
		int Close();
		void PrepareForCallbacks();

		typedef struct SessionMember {
			SessionProtocol::MemberData data;
			HSteamNetConnection conn;
		} SessionMember;

		std::vector<SessionMember> clients;
	};

	void from_json(const nlohmann::json& j, SessionServer::SessionMember& m);
	void to_json(nlohmann::json& j, const SessionServer::SessionMember& p);
}