#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <winuser.h>

#include <detours.h>
#include <vdf_parser.hpp>

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

void FindSidecar(LPSTR szDllPathA) {
	char szCwdBuffer[1024] = { 0 };
	GetCurrentDirectoryA(1024, szCwdBuffer);
	PathCombineA(szDllPathA, szCwdBuffer, "Sidecar.dll");
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
	LPSTR szSidecarDllPathA
) {
	wchar_t szErrorString[1024] = { 0 };
	DWORD dwError;
	LPCSTR dllsToLoad[1] = { szSidecarDllPathA };
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	SetLastError(0);

	if (
		!DetourCreateProcessWithDllsW(
			szExePath,
			NULL,
			NULL,
			NULL,
			TRUE,
			CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED,
			NULL,
			szGameDirectory,
			&si,
			&pi,
			1,
			dllsToLoad,
			NULL
		)) {
		dwError = GetLastError();
		StringCchPrintf(szErrorString, 1024, L"DetourCreateProcessWithDllEx failed: %d", dwError);
		MessageBox(NULL, szErrorString, NULL, MB_OK);
		MessageBox(NULL, szGameDirectory, NULL, MB_OK);
		MessageBox(NULL, szExePath, NULL, MB_OK);
		MessageBoxA(NULL, szSidecarDllPathA, NULL, MB_OK);
		if (dwError == ERROR_INVALID_HANDLE) {
			MessageBox(NULL, L"Can't detour a 64-bit target process from a 32-bit parent process or vice versa.", NULL, MB_OK);
		}
		ExitProcess(9009);
	}

	ResumeThread(pi.hThread);
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
) {
	wchar_t szGameDirectory[1024] = { 0 };
	wchar_t szExePath[1024] = { 0 };
	char szSidecarDllPathA[1024] = { 0 };

	DWORD env_size = 0;
	env_size = GetEnvironmentVariableW(L"sf4e_LINUX_DIR", szGameDirectory, 1024);

	if (env_size > 0) {
		PathCombineW(szExePath, szGameDirectory, L"SSFIV.exe");
	}
	else {
		FindSF4(szGameDirectory, szExePath);
	}
	FindSidecar(szSidecarDllPathA);
	CreateAppIDFile(szGameDirectory);
	CreateSF4Process(szGameDirectory, szExePath, szSidecarDllPathA);
	return 0;
}
