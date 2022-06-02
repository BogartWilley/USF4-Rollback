#include <windows.h>
#include <strsafe.h>
#include <detours.h>

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

    return rval;
}

void WINAPI fMain::RunWindowFunc(rMain* lpMain, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (OverlayWindowFunc(hwnd, uMsg, wParam, lParam)) {
        return;
    }
    rMain::staticMethods.RunWindowFunc(lpMain, hwnd, uMsg, wParam, lParam);
}