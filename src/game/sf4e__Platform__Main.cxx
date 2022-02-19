#include <windows.h>
#include <strsafe.h>
#include <detours.h>

#include "../overlay/overlay.h"
#include "Dimps__Platform__D3D.hxx"
#include "Dimps__Platform__Main.hxx"
#include "sf4e__Platform__Main.hxx"

using fMain = sf4e::Platform::Main;
using rMain = Dimps::Platform::Main;

void fMain::Install() {
    int (fMain::* _fInitialize)(void*, void*, void*) = &Initialize;
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