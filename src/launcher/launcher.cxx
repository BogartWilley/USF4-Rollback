#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <pathcch.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <winuser.h>

#include <detours/detours.h>
#include <spdlog/spdlog.h>
#include <vdf_parser.hpp>

#include "../sidecar/sidecar.hxx"

int FindSF4(LPWSTR szGameDirectory, LPWSTR szExePath) {
	DWORD dwDataRead = 1024;
	DWORD dwRegValueType;
	LSTATUS lQueryStatus;
	wchar_t szError[1024];
	const wchar_t* szGameSuffix = L"steamapps\\common\\Super Street Fighter IV - Arcade Edition";
	wchar_t szLibraries[8][1024];
	int nLibrariesUsed = 1;
	wchar_t szLibraryFolderVDFPath[1024];

	lQueryStatus = RegGetValueW(
		HKEY_CURRENT_USER,
		L"Software\\Valve\\Steam",
		L"SteamPath",
		RRF_RT_REG_SZ,
		&dwRegValueType,
		szLibraries[0],
		&dwDataRead
	);

	if (lQueryStatus != ERROR_SUCCESS) {
		StringCchPrintfW(szError, 1024, L"Query status: %d", lQueryStatus);
		MessageBoxW(NULL, szError, NULL, 0);
	}

	// Read additional libraries
	PathCombineW(szLibraryFolderVDFPath, szLibraries[0], L"steamapps\\libraryfolders.vdf");
	std::ifstream libraryFoldersFile(szLibraryFolderVDFPath);
	tyti::vdf::object libraryFoldersRoot = tyti::vdf::read(libraryFoldersFile);
	for (auto it = libraryFoldersRoot.childs.begin(); it != libraryFoldersRoot.childs.end(); ++it) {
		MultiByteToWideChar(
			CP_ACP,
			0,
			it->second->attribs["path"].c_str(),
			-1,
			szLibraries[nLibrariesUsed],
			1024
		);
		nLibrariesUsed++;
	}

	for (int i = 0; i < nLibrariesUsed; i++) {
		PathCombineW(szGameDirectory, szLibraries[i], szGameSuffix);
		if (PathIsDirectoryW(szGameDirectory)) {
			PathCombineW(szExePath, szGameDirectory, L"SSFIV.exe");
			return 0;
		}
	}

	return 1;
}

void CreateAppIDFile(LPWSTR szGuiltyDirectory) {
	wchar_t szAppIDPath[1024] = { 0 };
	DWORD nBytesWritten = 0;

	PathCombine(szAppIDPath, szGuiltyDirectory, L"steam_appid.txt");

	HANDLE hAppIDHandle = CreateFile(
		szAppIDPath,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	if (hAppIDHandle != INVALID_HANDLE_VALUE) {
		// Didn't exist, auto-created the file.
		WriteFile(hAppIDHandle, "45760", 6, &nBytesWritten, NULL);
		if (nBytesWritten != 6) {
			// Error!
		}
		CloseHandle(hAppIDHandle);
	}
}

void CreateSF4Process(
	LPWSTR szGameDirectory,
	LPWSTR szExePath,
	int nDlls,
	LPCSTR* rlpDlls
) {
	wchar_t szErrorString[1024] = { 0 };
	DWORD dwError;
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	HANDLE hSyncEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
	if (hSyncEvent == NULL) {
		spdlog::warn("CreateSF4Process: CreateEventW() could not create game sync handle, game may be unable to access Steam: err {}", GetLastError());
	}

	SetLastError(0);

	if (
		!DetourCreateProcessWithDllsW(
			szExePath,
			NULL,
			NULL,
			NULL,
			TRUE,
			CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED | CREATE_UNICODE_ENVIRONMENT,
			NULL,
			szGameDirectory,
			&si,
			&pi,
			nDlls,
			rlpDlls,
			NULL
		)) {
		dwError = GetLastError();
		StringCchPrintf(szErrorString, 1024, L"DetourCreateProcessWithDllEx failed: %d", dwError);
		MessageBox(NULL, szErrorString, NULL, MB_OK);
		MessageBox(NULL, szGameDirectory, NULL, MB_OK);
		MessageBox(NULL, szExePath, NULL, MB_OK);
		for (int i = 0; i < nDlls; i++) {
			MessageBoxA(NULL, rlpDlls[i], NULL, MB_OK);
		}
		if (dwError == ERROR_INVALID_HANDLE) {
			MessageBox(NULL, L"Can't detour a 64-bit target process from a 32-bit parent process or vice versa.", NULL, MB_OK);
		}
		ExitProcess(9009);
	}

	sf4eSidecar::Payload p = { 0 };
	if (hSyncEvent != NULL) {
		if (!DuplicateHandle(GetCurrentProcess(), hSyncEvent, pi.hProcess, &p.hSyncEvent, 0, false, DUPLICATE_SAME_ACCESS)) {
			spdlog::warn("CreateSF4Process: DuplicateHandle() could not duplicate game sync handle, game may be unable to access Steam: err {}", GetLastError());
		}
	}
	if (!DetourCopyPayloadToProcess(pi.hProcess, sf4eSidecar::s_guidSidecarPayload, &p, sizeof(sf4eSidecar::Payload))) {
		StringCchPrintf(szErrorString, 1024, L"DetourCopyPayloadToProcess failed: %d", GetLastError());
		MessageBox(NULL, szErrorString, NULL, MB_OK);
		ExitProcess(9008);
	}

	ResumeThread(pi.hThread);
	if (hSyncEvent != NULL) {
		DWORD lockWaitResult = WaitForSingleObject(hSyncEvent, 60 * 1000);
		if (lockWaitResult != 0) {
			spdlog::warn("CreateSF4Process: WaitForSingleObject() could not wait for game sync handle, game may be unable to access Steam: err {}", GetLastError());
		}
		CloseHandle(hSyncEvent);
	}
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {
	HRESULT res = S_OK;
	wchar_t szErrorStringW[4096] = { 0 };
	wchar_t szLauncherDirW[1024] = { 0 };
	wchar_t szGameDirectory[1024] = { 0 };
	wchar_t szExePath[1024] = { 0 };
	char szLauncherDirA[1024] = { 0 };
	char szSidecarDllPathA[1024] = { 0 };
	int nDlls = 1;
	const char* dlls[1] = {
		szSidecarDllPathA,
	};

	// Compute the path to the sidecar DLL based on the launcher's directory.
	// Ideally, this wouldn't have to convert from wide-char to multibyte in
	// the system's codepage, but Detours uses multibyte paths when injecting
	// DLLs.
	GetModuleFileNameW(NULL, szLauncherDirW, 1024);
	PathCchRemoveFileSpec(szLauncherDirW, 1024);
	WideCharToMultiByte(CP_ACP, 0, szLauncherDirW, 1024, szLauncherDirA, 1024, NULL, NULL);
	PathCombineA(szSidecarDllPathA, szLauncherDirA, "Sidecar.dll");

	// Modify PATH to contain the launcher's directory. While this isn't that
	// useful for the launcher itself, child processes inherit the parent's
	// environment by default, so the child SF4 process will search in the
	// launcher directory for DLLs.
	wchar_t szPathW[2048] = { 0 };
	wchar_t szNewPathW[2048] = { 0 };
	GetEnvironmentVariableW(L"PATH", szPathW, 2048);
	if (res = StringCchPrintf(
		szNewPathW,
		2048,
		TEXT("%s;%s"),
		szPathW,
		szLauncherDirW
	)) {
		StringCchPrintfW(
			szErrorStringW,
			4096,
			L"Could not create new PATH environment variable %s;%s : %d",
			szPathW,
			szLauncherDirW,
			res
		);
		MessageBoxW(NULL, szErrorStringW, NULL, MB_OK);
		return -1;
	}
	SetEnvironmentVariableW(L"PATH", szNewPathW);

	DWORD env_size = 0;
	env_size = GetEnvironmentVariableW(L"sf4e_LINUX_DIR", szGameDirectory, 1024);

	if (env_size > 0) {
		PathCombineW(szExePath, szGameDirectory, L"SSFIV.exe");
	}
	else {
		FindSF4(szGameDirectory, szExePath);
	}
	CreateAppIDFile(szGameDirectory);
	CreateSF4Process(szGameDirectory, szExePath, nDlls, dlls);
	return 0;
}
