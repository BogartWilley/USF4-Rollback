#include <memory>

#include <windows.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <strsafe.h>

#include <detours/detours.h>
#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "../Dimps/Dimps__Eva.hxx"
#include "../Dimps/Dimps__Platform.hxx"
#include "sf4e.hxx"
#include "sf4e__Platform.hxx"
#include "sf4e__UserApp.hxx"
#include "../overlay/overlay.h"

namespace rPlatform = Dimps::Platform;
using rD3D = rPlatform::D3D;
using rGFxApp = rPlatform::GFxApp;
using rMain = rPlatform::Main;

namespace fPlatform = sf4e::Platform;
using fD3D = fPlatform::D3D;
using fGFxApp = fPlatform::GFxApp;
using fMain = fPlatform::Main;
using fUserApp = sf4e::UserApp;

void fPlatform::Install() {
    D3D::Install();
    Main::Install();
}

void fD3D::Install() {
    void (fD3D:: * _fDestroy)() = &Destroy;
    DetourAttach((PVOID*)&rD3D::privateMethods.Destroy, *(PVOID*)&_fDestroy);
    DetourAttach((PVOID*)&rD3D::staticMethods.RunD3DOperations, RunD3DOperations);
}

void WINAPI fD3D::RunD3DOperations(void* data) {
    rD3D::staticMethods.RunD3DOperations(data);
    DrawOverlay();
}

void fD3D::Destroy() {
    FreeOverlay();
    (this->*rD3D::privateMethods.Destroy)();
}

void fGFxApp::RecordToAdditionalMemento(rGFxApp* a, AdditionalMemento& m) {
    int i;

    rGFxApp::ObjectPool<Dimps::Eva::IEmSpriteAction>* actionPool = rGFxApp::GetActionPool(a);
    for (i = 0; i < NUM_GFX_ACTIONS; i++) {
        m.actions[i].first = actionPool->useIndex[i];
        if (actionPool->useIndex[i]) {
            sf4e::Eva::IEmSpriteAction::RecordToAdditionalMemento(&actionPool->raw[i], m.actions[i].second);
        }
    }
}

void fGFxApp::RestoreFromAdditionalMemento(rGFxApp* a, const AdditionalMemento& m) {
    int i;

    rGFxApp::ObjectPool<Dimps::Eva::IEmSpriteAction>* actionPool = rGFxApp::GetActionPool(a);
    for (i = 0; i < NUM_GFX_ACTIONS; i++) {
        actionPool->useIndex[i] = m.actions[i].first;
        if (actionPool->useIndex[i]) {
            sf4e::Eva::IEmSpriteAction::RestoreFromAdditionalMemento(&actionPool->raw[i], m.actions[i].second);
        }
    }
}

void fMain::Install() {
    int (fMain:: * _fInitialize)(void*, void*, void*) = &Initialize;
    void (fMain:: * _fDestroy)() = &Destroy;
    DetourAttach((PVOID*)&rMain::publicMethods.Initialize, *(PVOID*)&_fInitialize);
    DetourAttach((PVOID*)&rMain::publicMethods.Destroy, *(PVOID*)&_fDestroy);
    DetourAttach((PVOID*)&rMain::staticMethods.RunWindowFunc, &RunWindowFunc);
}

int fMain::Initialize(void* a, void* b, void* c) {
    int rval = (this->*(rMain::publicMethods.Initialize))(a, b, c);

    // Set up spdlog
    PWSTR path;
    HRESULT queryResult = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);
    if (queryResult == S_OK) {
        try
        {
            wchar_t logpath[MAX_PATH];
            PathCombineW(logpath, path, L"sf4e/logs/sf4e.log");
            int max_size = 1048576 * 5; // 5MB
            int max_files = 10;
            auto logger = spdlog::rotating_logger_mt("sf4e", logpath, max_size, max_files, true);
            spdlog::set_default_logger(logger);
            spdlog::info("Welcome to sf4e");
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            MessageBoxA(NULL, ex.what(), NULL, MB_OK);
            DebugBreak();
        }
    }
    else {
        MessageBox(NULL, TEXT("Could not get appdata path for logs!"), NULL, MB_OK);
    }
    CoTaskMemFree(path);

    // Once Dimps::Platform::Main is fully specified, we can stop doing this
    // pointer math.
    rMain::Win32_WindowData** windowData = (rMain::Win32_WindowData**)((unsigned int)this + 0x490);
    InitializeOverlay((*windowData)->hWnd, Dimps::Platform::D3D::staticMethods.GetSingleton()->lpD3DDevice);

    SteamDatagramErrMsg errMsg;
    if (!GameNetworkingSockets_Init(nullptr, errMsg)) {
        spdlog::error("GameNetworkingSockets_Init failed.  {}", errMsg);
    }

    return rval;
}

void fMain::Destroy() {
    if (fUserApp::client) {
        fUserApp::client.release();
    }
    if (fUserApp::server) {
        fUserApp::server.release();
    }
    GameNetworkingSockets_Kill();
    spdlog::shutdown();
    (this->*rMain::publicMethods.Destroy)();
}

void WINAPI fMain::RunWindowFunc(rMain* lpMain, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (OverlayWindowFunc(hwnd, uMsg, wParam, lParam)) {
        return;
    }
    rMain::staticMethods.RunWindowFunc(lpMain, hwnd, uMsg, wParam, lParam);
}