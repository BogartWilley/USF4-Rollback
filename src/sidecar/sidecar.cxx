#ifndef UNICODE
#define UNICODE
#endif

#define DIRECTINPUT_VERSION 0x0800

#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <detours.h>
#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#include <vdf_parser.hpp>

#include "../game/game.h"
#include "../game/sf4x__Platform__D3D.hxx"
#include "../game/sf4x__Platform__Main.hxx"
#include "../overlay/overlay.h"
#include "sidecar.h"

static LPCWSTR DETOUR_FAILED_MESSAGE = TEXT("Could not detour targets!");

static BOOL g_bOverlayCreated = FALSE;
static GameMethods g_gameMethods;
static GameState g_gameState;


LRESULT WINAPI FakeWindowFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (OverlayWindowFunc(hWnd, msg, wParam, lParam)) {
		return true;
	}

	return g_gameMethods.WindowFunc(hWnd, msg, wParam, lParam);
}

HRESULT AttachFunctionDetours(GameMethods* src) {
	DetourAttach((PVOID*)&src->WindowFunc, FakeWindowFunc);
	sf4x::Platform::Main::Install(&g_gameState);
	sf4x::Platform::D3D::Install(&g_gameMethods, &g_gameState);

	return S_OK;
}

__declspec(dllexport) BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD dwReason,
	LPVOID reserved
) {
	HMODULE peRoot;
	LONG error;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DetourRestoreAfterWith();
		peRoot = LocatePERoot();
		if (LocateGameMethods(peRoot, &g_gameMethods) != S_OK) {
			MessageBox(NULL, TEXT("Could not locate game methods!"), NULL, MB_OK);
		}

		if (LocateGameState(peRoot, &g_gameState) != S_OK) {
			MessageBox(NULL, TEXT("Could not locate game state!"), NULL, MB_OK);
		}

		DetourTransactionBegin();
		AttachFunctionDetours(&g_gameMethods);
		error = DetourTransactionCommit();
		if (error != NO_ERROR) {
			MessageBox(NULL, DETOUR_FAILED_MESSAGE, NULL, MB_OK);
		}
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
