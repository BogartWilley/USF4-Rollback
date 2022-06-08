#pragma once

#include <windows.h>

namespace Dimps {
	namespace Game {
		void Locate(HMODULE peRoot);

		struct GameMementoKey {
			struct MementoID {
				DWORD lo;
				DWORD hi;
			};
		};
	}
}