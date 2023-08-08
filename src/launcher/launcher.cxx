#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <winuser.h>

#include <detours/detours.h>
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

void FindCrypto(LPSTR szDllPathA) {
	char szCwdBuffer[1024] = { 0 };
	GetCurrentDirectoryA(1024, szCwdBuffer);
	PathCombineA(szDllPathA, szCwdBuffer, "libcrypto-3.dll");
}

void FindFmt(LPSTR szDllPathA) {
	char szCwdBuffer[1024] = { 0 };
	GetCurrentDirectoryA(1024, szCwdBuffer);
#if DEBUG
	PathCombineA(szDllPathA, szCwdBuffer, "fmtd.dll");
#else
	PathCombineA(szDllPathA, szCwdBuffer, "fmt.dll");
#endif
}

void FindGameNetworkingSockets(LPSTR szDllPathA) {
	char szCwdBuffer[1024] = { 0 };
	GetCurrentDirectoryA(1024, szCwdBuffer);
	PathCombineA(szDllPathA, szCwdBuffer, "GameNetworkingSockets.dll");
}

void FindGGPO(LPSTR szDllPathA) {
	char szCwdBuffer[1024] = { 0 };
	GetCurrentDirectoryA(1024, szCwdBuffer);
	PathCombineA(szDllPathA, szCwdBuffer, "GGPO.dll");
}

void FindProtobuf(LPSTR szDllPathA) {
	char szCwdBuffer[1024] = { 0 };
	GetCurrentDirectoryA(1024, szCwdBuffer);
#if DEBUG
	PathCombineA(szDllPathA, szCwdBuffer, "libprotobufd.dll");
#else
	PathCombineA(szDllPathA, szCwdBuffer, "libprotobuf.dll");
#endif
}


void FindSidecar(LPSTR szDllPathA) {
	char szCwdBuffer[1024] = { 0 };
	GetCurrentDirectoryA(1024, szCwdBuffer);
	PathCombineA(szDllPathA, szCwdBuffer, "Sidecar.dll");
}

void FindSpdlog(LPSTR szDllPathA) {
	char szCwdBuffer[1024] = { 0 };
	GetCurrentDirectoryA(1024, szCwdBuffer);
#if DEBUG
	PathCombineA(szDllPathA, szCwdBuffer, "spdlogd.dll");
#else
	PathCombineA(szDllPathA, szCwdBuffer, "spdlog.dll");
#endif
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
	char szCryptoDllPathA[1024] = { 0 };
	char szFmtDllPathA[1024] = { 0 };
	char szGameNetworkingSocketsDllPathA[1024] = { 0 };
	char szGGPODllPathA[1024] = { 0 };
	char szProtobufDllPathA[1024] = { 0 };
	char szSidecarDllPathA[1024] = { 0 };
	char szSpdlogDllPathA[1024] = { 0 };
	int nDlls = 7;

	// To prevent the need for manipulating the subprocess's import table,
	// manipulating the subprocess's DLL search paths, or copying sf4e's
	// dependencies into SF4's folder directly, sf4e injects dependencies
	// manually and dynamically. Consequently, this list is order-sensitive-
	// if the sidecar DLL depends on spdlog and spdlog is not injected first,
	// spdlog won't be found via normal search paths and the SF4 will halt
	// with an error.
	const char* dlls[7] = {
		szFmtDllPathA,
		szSpdlogDllPathA,
		szCryptoDllPathA,
		szProtobufDllPathA,
		szGameNetworkingSocketsDllPathA,
		szGGPODllPathA,
		szSidecarDllPathA,
	};

	DWORD env_size = 0;
	env_size = GetEnvironmentVariableW(L"sf4e_LINUX_DIR", szGameDirectory, 1024);

	if (env_size > 0) {
		PathCombineW(szExePath, szGameDirectory, L"SSFIV.exe");
	}
	else {
		FindSF4(szGameDirectory, szExePath);
	}
	FindCrypto(szCryptoDllPathA);
	FindFmt(szFmtDllPathA);
	FindGameNetworkingSockets(szGameNetworkingSocketsDllPathA);
	FindGGPO(szGGPODllPathA);
	FindProtobuf(szProtobufDllPathA);
	FindSidecar(szSidecarDllPathA);
	FindSpdlog(szSpdlogDllPathA);
	CreateAppIDFile(szGameDirectory);
	CreateSF4Process(szGameDirectory, szExePath, nDlls, dlls);
	return 0;
}
