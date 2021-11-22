#include <windows.h>

#include <shlwapi.h>
#include <strsafe.h>
#include <winuser.h>

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
