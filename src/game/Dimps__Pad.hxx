#pragma once

#include <windows.h>

namespace Dimps {
	namespace Pad {
		void Locate(HMODULE peRoot);

		struct System {
			typedef struct __publicMethods {
				DWORD(System::* GetButtons_On)(int pindex);
				DWORD(System::* GetButtons_Rising)(int pindex);
				DWORD(System::* GetButtons_Falling)(int pindex);
				DWORD(System::* GetButtons_RisingWithRepeat)(int pindex);
			} __publicMethods;

			typedef struct __staticMethods {
				System* (*GetSingleton)();
			} __staticMethods;

			static void Locate(HMODULE peRoot);
			static __publicMethods publicMethods;
			static __staticMethods staticMethods;
		};
	}
}