#pragma once

#include <windows.h>

#include "Dimps__Eva__TaskCore.hxx"

namespace Dimps {
	namespace Eva {
		struct TaskCoreRegistry {
			typedef struct __staticMethods {
				TaskCore* (* GetCoreByIndex)(int index);
				int (* GetNumActiveCores)();			
			} __staticMethods;

			static void Locate(HMODULE peRoot);
			static __staticMethods staticMethods;
		};
	}
}