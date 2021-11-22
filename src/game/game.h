#pragma once

#include <windows.h>
#include <d3d9.h>
#include <dinput.h>

typedef struct GameState {
    LPDIRECT3DDEVICE9* d3dDevice;
    HWND hWnd;
} GameState;

typedef struct GameMethods {
    LRESULT(WINAPI* WindowFunc)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
} GameMethods;

HMODULE LocatePERoot();
HRESULT LocateGameMethods(HMODULE peRoot, GameMethods* methods);
HRESULT LocateGameState(HMODULE peRoot, GameState* state);
