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
		enum ErrorType {
			SCE_UNKNOWN,
			SCE_JOIN_REJECTED_HASH_INVALID,
			SCE_JOIN_REJECTED_LOBBY_FULL,
			SCE_JOIN_REJECTED_NAME_TAKEN,
			SCE_JOIN_REJECTED_REQUEST_INVALID,
		};

		struct Callbacks {
			void* data;
			void (*OnError)(ErrorType errorType, const Callbacks& callbacks);
		};

		SessionClient(
			const Callbacks& callbacks,
			std::string sidecarHash,
			uint16_t ggpoPort,
			std::string& name,
			uint8_t deviceType,
			uint8_t deviceIdx,
			uint8_t delay
		);
		~SessionClient();


		int Connect(HSteamNetConnection newConn);
		int Connect(const SteamNetworkingIPAddr& serverAddr);
		int Step();
		void PrepareForCallbacks();

		// Lobby data
		std::string _name;
		SessionProtocol::LobbyData _lobbyData;
		SessionProtocol::MatchData _matchData;
		int64_t _outstandingReadyRequestNumber = -1;
		bool _snapshotsEnabled;

		EResult SetMatchConditions(SessionProtocol::SetConditionsRequest& r);
		EResult ReportResults(SessionProtocol::ReportResultsRequest& r);

		// Public for testing
		EResult Send(nlohmann::json& msg, int64_t* outMessageNum);
		
		// Connection related data - public for testing
		std::string _sidecarHash;
		uint16_t _ggpoPort;
	private:

		// Connection related data
		Callbacks _callbacks;
		bool _connected = false;
		SteamNetworkingIPAddr _serverAddr;
		HSteamNetConnection _conn;
		ISteamNetworkingSockets* _interface;

		// Lobby data
		uint8_t _delay;
		uint8_t _deviceType;
		uint8_t _deviceIdx;

		std::map<int, SessionProtocol::StateSnapshot> pendingRemoteSnapshots;

		void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

		static SessionClient* s_pCallbackInstance;
		static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
		static void _OnVsBattleTasksRegistered();
		static void _OnVsPreBattleTasksRegistered();
	};
}
