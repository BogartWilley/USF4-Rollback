#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>

#include "../Dimps/Dimps__GameEvents.hxx"

namespace sf4e {
	const int SESSION_CLIENT_MAX_MESSAGES_PER_POLL = 20;

	class SessionClient
	{
	public:
		SessionClient(const SteamNetworkingIPAddr& serverAddr);
		void Step();
		Dimps::GameEvents::VsMode::ConfirmedCharaConditions myConditions;
		Dimps::GameEvents::VsMode::ConfirmedCharaConditions confirmedConditions[2];
		int confirmedStageID;

	private:
		std::vector<nlohmann::json> queuedMessages;
		HSteamNetConnection m_hConnection;
		ISteamNetworkingSockets* m_pInterface;

		void PollIncomingMessages();
		void PollConnectionStateChanges();
		void SendQueuedMessages();

		void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

		static SessionClient* s_pCallbackInstance;
		static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
		static void _OnVsPreBattleTasksRegistered();
	};
}
