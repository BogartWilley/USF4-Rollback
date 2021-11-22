#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <shlwapi.h>
#include <strsafe.h>

#include <detours.h>
#include <d3d9.h>
#include <sstream>

#include "game.h"
#include "Dimps__Platform__D3D.hxx"
#include "Dimps__Platform__Main.hxx"

static GameMethods* g_lpGameMethods;
static GameState* g_lpGameState;

HMODULE LocatePERoot() {
	return DetourGetContainingModule(DetourGetEntryPoint(NULL));
}

HRESULT LocateGameMethods(HMODULE peRoot, GameMethods* dest) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	dest->WindowFunc = (LRESULT(WINAPI*)(HWND, UINT, WPARAM, LPARAM))(peRootOffset + 0x382DA0);
	g_lpGameMethods = dest;

	Dimps::Platform::D3D::Locate(peRoot);
	Dimps::Platform::Main::Locate(peRoot);
	return S_OK;
}

HRESULT LocateGameState(HMODULE peRoot, GameState* dest) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	dest->d3dDevice = (LPDIRECT3DDEVICE9*)(peRootOffset + 0x97cfe0);
	g_lpGameState = dest;

	return S_OK;
}