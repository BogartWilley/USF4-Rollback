#include <chrono>
#include <memory>

#include <windows.h>
#include <detours/detours.h>

#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include <spdlog/spdlog.h>

#include "../Dimps/Dimps__GameEvents.hxx"
#include "../Dimps/Dimps__UserApp.hxx"
#include "../session/sf4e__SessionClient.hxx"
#include "../session/sf4e__SessionServer.hxx"

#include "sf4e__Game__Battle.hxx"
#include "sf4e__Game__Battle__System.hxx"
#include "sf4e__UserApp.hxx"

using rVsMode = Dimps::GameEvents::VsMode;
using rUserApp = Dimps::UserApp;
using fSystem = sf4e::Game::Battle::System;
using fUserApp = sf4e::UserApp;
using sf4e::Game::Battle::Sound::SoundPlayerManager;
using sf4e::SessionClient;
using sf4e::SessionServer;

std::unique_ptr<SessionClient> fUserApp::client;
std::unique_ptr<SessionServer> fUserApp::server;

void fUserApp::Install() {
    DetourAttach((PVOID*)&rUserApp::staticMethods.Steam_PostUpdate, Steam_PostUpdate);
}

void fUserApp::StartClient(char* joinAddr, uint16_t port, std::string& sidecarHash, std::string& name, uint8_t deviceType, uint8_t deviceIdx, uint8_t delay) {
    SteamNetworkingIPAddr addr;
    addr.Clear();
    addr.ParseString(joinAddr);
    client.reset(new SessionClient(sidecarHash, addr, port, name, deviceType, deviceIdx, delay));
}

void fUserApp::StartServer(uint16 hostPort, std::string& sidecarHash) {
    server.reset(new SessionServer(hostPort, sidecarHash));
}

void fUserApp::Steam_PostUpdate() {
    if (client) {
        client->PrepareForCallbacks();
    }
    if (server) {
        server->PrepareForCallbacks();
    }
    SteamNetworkingSockets()->RunCallbacks();

    if (client) {
        if (client->Step()) {
            delete client.release();
        }
    }

    if (server) {
        if (server->Step()) {
            delete server.release();
        }
    }

    if (fSystem::ggpo) {
        ggpo_idle(fSystem::ggpo, 1);
    }

    rUserApp::staticMethods.Steam_PostUpdate();
}
