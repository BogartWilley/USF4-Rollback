#include <windows.h>
#include <strsafe.h>
#include <detours.h>

#include "game.h"
#include "../overlay/overlay.h"
#include "Dimps__platform__Main.hxx"
#include "sf4x__platform__Main.hxx"

using fMain = sf4x::Platform::Main;
using rMain = Dimps::Platform::Main;

static GameState* lpGameState;

void fMain::Install(GameState* gameState) {
    int (fMain::* _FakeInitialize)(void*, void*, void*) = &FakeInitialize;
    DetourAttach((PVOID*)&rMain::publicMethods.Initialize, *(PVOID*)&_FakeInitialize);

    lpGameState = gameState;
}

int fMain::FakeInitialize(void* a, void* b, void* c) {
    int rval = (this->*(rMain::publicMethods.Initialize))(a, b, c);

    // We should, uh, make this not suck
    unsigned int peRootOffset = (unsigned int)LocatePERoot();
    void* _win32SubPtr = *(void**)(peRootOffset + 0x97d778);
    lpGameState->hWnd = *(HWND*)((unsigned int)_win32SubPtr + 0x4);
    InitializeOverlay(lpGameState);
    // End jank

    return rval;
}