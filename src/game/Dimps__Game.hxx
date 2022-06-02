#pragma once

#include <windows.h>

namespace Dimps {
	namespace Game {
		void Locate(HMODULE peRoot);

		struct GameMementoKey {
			struct MementoID {
				DWORD id1;
				DWORD id2;
			};
		};
	}
}