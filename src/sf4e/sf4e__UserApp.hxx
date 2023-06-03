#pragma once

#include <memory>

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
        static void StartClient(char* joinAddr, Dimps::GameEvents::VsMode::ConfirmedCharaConditions& conditions);
        static void StartServer(uint16 hostPort, Dimps::GameEvents::VsMode::ConfirmedCharaConditions& conditions, int stageID);
    };
}