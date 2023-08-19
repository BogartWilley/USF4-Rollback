#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

#include "sf4e__SessionProtocol.hxx"

namespace sf4e {
	extern const int SESSION_CLIENT_MAX_MESSAGES_PER_POLL;

	class SessionClient
	{
	public:
		SessionClient(
			std::string sidecarHash,
			const SteamNetworkingIPAddr& serverAddr,
			uint16_t port,
			std::string& name,
			uint8_t deviceType,
			uint8_t deviceIdx,
			uint8_t delay
		);
		~SessionClient();
		int Step();
		void PrepareForCallbacks();

		// Lobby data
		std::string _name;
		SessionProtocol::LobbyData _lobbyData;
		SessionProtocol::MatchData _matchData;
		int64_t _outstandingReadyRequestNumber = -1;

		EResult SetMatchConditions(SessionProtocol::SetConditionsRequest& r);
		EResult ReportResults(SessionProtocol::ReportResultsRequest& r);
		
	private:
		// Connection related data
		std::string _sidecarHash;
		bool _connected = false;
		SteamNetworkingIPAddr _serverAddr;
		HSteamNetConnection _conn;
		ISteamNetworkingSockets* _interface;

		// Lobby data
		uint16_t _port;
		uint8_t _delay;
		uint8_t _deviceType;
		uint8_t _deviceIdx;


		std::map<int, SessionProtocol::StateSnapshot> pendingRemoteSnapshots;

		EResult Send(nlohmann::json& msg, int64_t* outMessageNum);
		void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

		static SessionClient* s_pCallbackInstance;
		static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
		static void _OnVsBattleTasksRegistered();
		static void _OnVsPreBattleTasksRegistered();
	};
}
