#pragma once

#include <memory>
#include <string>

#include <windows.h>

#include "../Dimps/Dimps__GameEvents.hxx"
#include "../Dimps/Dimps__UserApp.hxx"
#include "../session/sf4e__SessionClient.hxx"
#include "../session/sf4e__SessionServer.hxx"

namespace sf4e {
    struct UserApp : Dimps::UserApp
    {
        static std::unique_ptr<SessionClient> client;
        static std::unique_ptr<SessionServer> server;
        static void Install();
        static void Steam_PostUpdate();
        static void StartClient(const SessionClient::Callbacks& callbacks, char* joinAddr, uint16_t port, std::string& sidecarHash, std::string& name, uint8_t deviceType, uint8_t deviceIdx, uint8_t delay);
        static void StartServer(uint16 hostPort, std::string& sidecarHash);
    };
}