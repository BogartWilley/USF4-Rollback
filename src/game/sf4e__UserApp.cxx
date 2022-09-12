#include <chrono>
#include <memory>

#include <windows.h>
#include <detours.h>

#include "asio.hpp"
#include "spdlog/spdlog.h"
#include "Dimps__UserApp.hxx"
#include "sf4e__UserApp.hxx"

using rUserApp = Dimps::UserApp;
using fUserApp = sf4e::UserApp;

static std::chrono::milliseconds asioDeadline(1);
std::unique_ptr<asio::io_context> fUserApp::io_context(new asio::io_context());


void fUserApp::Install() {
    DetourAttach((PVOID*)&rUserApp::staticMethods.Steam_PostUpdate, Steam_PostUpdate);
}

void fUserApp::Steam_PostUpdate() {
    rUserApp::staticMethods.Steam_PostUpdate();
    io_context->run_for(asioDeadline);
}
