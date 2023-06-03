#pragma once

#include <map>
#include <string>

#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <nlohmann/json.hpp>

#include "../Dimps/Dimps__GameEvents.hxx"

namespace sf4e {
	const int SESSION_SERVER_MAX_MESSAGES_PER_POLL = 200;

	class SessionServer
	{
	public:
		SessionServer(uint16 nPort);
		void Step();
		void Close();
		Dimps::GameEvents::VsMode::ConfirmedCharaConditions confirmedConditions[2];
		Dimps::GameEvents::VsMode::ConfirmedCharaConditions myConditions;
		int confirmedStageID;
		int myStageID;

	private:
		HSteamListenSocket m_hListenSock;
		HSteamNetConnection m_hClient;
		ISteamNetworkingSockets* m_pInterface;
		static SessionServer* s_pCallbackInstance;
		static void _OnVsPreBattleTasksRegistered();
		static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
		std::vector<nlohmann::json> queuedMessages;
		void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);
		void PollConnectionStateChanges();
		void PollIncomingMessages();
		void SendQueuedMessages();
	};
}