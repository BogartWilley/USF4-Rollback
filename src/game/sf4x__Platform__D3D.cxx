#include <windows.h>
#include <strsafe.h>
#include <detours.h>

#include "../overlay/overlay.h"
#include "game.h"

#include "Dimps__Platform__D3D.hxx"
#include "sf4x__Platform__D3D.hxx"

using fD3D = sf4x::Platform::D3D;
using rD3D = Dimps::Platform::D3D;

static GameMethods* lpGameMethods;
static GameState* lpGameState;

void fD3D::Install(GameMethods* methods, GameState* state) {
    void (fD3D:: * _FakeDestroy)() = &FakeDestroy;
    DetourAttach((PVOID*)&rD3D::privateMethods.Destroy, *(PVOID*)&_FakeDestroy);
    DetourAttach((PVOID*)&rD3D::staticMethods.RunD3DOperations, FakeRunD3DOperations);
    lpGameMethods = methods;
    lpGameState = state;
}

void WINAPI fD3D::FakeRunD3DOperations(void* data) {
    rD3D::staticMethods.RunD3DOperations(data);
    DrawOverlay(lpGameMethods, lpGameState);
}

void fD3D::FakeDestroy() {
    FreeOverlay();
    (this->*rD3D::privateMethods.Destroy)();
}