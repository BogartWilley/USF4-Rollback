#pragma once

#include <windows.h>

namespace Dimps {
	extern char** characterCodes;
	extern char** characterNames;
	extern char** stageCodes;
	extern char** stageNames;

	void Locate(HMODULE peRoot);
}
