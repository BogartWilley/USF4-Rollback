#include <memory>

#include <windows.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <strsafe.h>
#include <detours.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "Dimps__Platform.hxx"
#include "sf4e__Platform.hxx"
#include "../overlay/overlay.h"

namespace rPlatform = Dimps::Platform;
using rD3D = rPlatform::D3D;
using rMain = rPlatform::Main;

namespace fPlatform = sf4e::Platform;
using fD3D = fPlatform::D3D;
using fMain = fPlatform::Main;

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

void fMain::Install() {
    int (fMain:: * _fInitialize)(void*, void*, void*) = &Initialize;
    DetourAttach((PVOID*)&rMain::publicMethods.Initialize, *(PVOID*)&_fInitialize);
    DetourAttach((PVOID*)&rMain::staticMethods.RunWindowFunc, &RunWindowFunc);
}

int fMain::Initialize(void* a, void* b, void* c) {
    int rval = (this->*(rMain::publicMethods.Initialize))(a, b, c);

    // Once Dimps::Platform::Main is fully specified, we can stop doing this
    // pointer math.
    rMain::Win32_WindowData** windowData = (rMain::Win32_WindowData**)((unsigned int)this + 0x490);
    InitializeOverlay((*windowData)->hWnd, Dimps::Platform::D3D::staticMethods.GetSingleton()->lpD3DDevice);

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
            std::shared_ptr<spdlog::logger> logger = spdlog::rotating_logger_mt("sf4e", logpath, max_size, max_files);
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

    return rval;
}

void WINAPI fMain::RunWindowFunc(rMain* lpMain, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (OverlayWindowFunc(hwnd, uMsg, wParam, lParam)) {
        return;
    }
    rMain::staticMethods.RunWindowFunc(lpMain, hwnd, uMsg, wParam, lParam);
}