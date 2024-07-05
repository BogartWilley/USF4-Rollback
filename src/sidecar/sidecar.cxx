#ifndef UNICODE
#define UNICODE
#endif

#define DIRECTINPUT_VERSION 0x0800

#include <stdio.h>
#include <windows.h>
#include <bcrypt.h>
#include <strsafe.h>
#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>

#include <detours/detours.h>

#include "../Dimps/Dimps.hxx"
#include "../sf4e/sf4e.hxx"

#include "sidecar.hxx"

static HMODULE LocatePERoot() {
	return DetourGetContainingModule(DetourGetEntryPoint(NULL));
}

sf4eSidecar::Payload* FindPayload() {
	HMODULE hMod;
	sf4eSidecar::Payload* payload;
	DWORD payloadLength;

	for (hMod = DetourEnumerateModules(NULL); hMod != NULL; hMod = DetourEnumerateModules(hMod)) {
		payload = (sf4eSidecar::Payload*)DetourFindPayload(hMod, sf4eSidecar::s_guidSidecarPayload, &payloadLength);
		if (GetLastError() == 0 && payload != NULL) {
			return payload;
		}
	}
	MessageBoxA(NULL, "Could not find launcher payload!", NULL, MB_OK);
	return NULL;
}

static LPCWSTR DETOUR_FAILED_MESSAGE = TEXT("Could not detour targets!");

__declspec(dllexport) BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD dwReason,
	LPVOID reserved
) {
	LONG error;
	sf4eSidecar::Payload* payload = NULL;


	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DetourRestoreAfterWith();
		payload = FindPayload();
		Dimps::Locate(LocatePERoot());
		DetourTransactionBegin();
		sf4e::Install(hinstDLL, payload != NULL ? payload->hSyncEvent : NULL);
		payload = NULL;
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
