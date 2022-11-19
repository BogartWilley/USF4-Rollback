#include <chrono>
#include <memory>

#include <windows.h>
#include <detours/detours.h>

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include "spdlog/spdlog.h"

#include "Dimps__GameEvents.hxx"
#include "Dimps__UserApp.hxx"
#include "sf4e__SessionClient.hxx"
#include "sf4e__SessionServer.hxx"
#include "sf4e__UserApp.hxx"

using rVsMode = Dimps::GameEvents::VsMode;
using rUserApp = Dimps::UserApp;
using fUserApp = sf4e::UserApp;
using sf4e::SessionClient;
using sf4e::SessionServer;

std::unique_ptr<SessionClient> fUserApp::client;
std::unique_ptr<SessionServer> fUserApp::server;

void fUserApp::Install() {
    DetourAttach((PVOID*)&rUserApp::staticMethods.Steam_PostUpdate, Steam_PostUpdate);
}

void fUserApp::StartClient(char* joinAddr, Dimps::GameEvents::VsMode::ConfirmedCharaConditions& conditions) {
    size_t charaConditionSize = sizeof(rVsMode::ConfirmedCharaConditions);

    SteamNetworkingIPAddr addr;
    addr.Clear();
    addr.ParseString(joinAddr);
    client.reset(new SessionClient(addr));
    memcpy_s(&client->myConditions, charaConditionSize, &conditions, charaConditionSize);
}

void fUserApp::StartServer(uint16 hostPort, Dimps::GameEvents::VsMode::ConfirmedCharaConditions& conditions, int stageID) {
    size_t charaConditionSize = sizeof(rVsMode::ConfirmedCharaConditions);

    SteamNetworkingIPAddr addr;
    addr.Clear();
    addr.SetIPv6LocalHost(hostPort);
    server.reset(new SessionServer(hostPort));
    memcpy_s(&server->myConditions, charaConditionSize, &conditions, charaConditionSize);
    server->myStageID = stageID;
}

void fUserApp::Steam_PostUpdate() {
    if (client) {
        client->Step();
    }

    if (server) {
        server->Step();
    }

    rUserApp::staticMethods.Steam_PostUpdate();
}
