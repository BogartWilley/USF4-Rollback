#pragma once

#include <windows.h>

namespace Dimps {
	namespace GameEvents {
		void Locate(HMODULE peRoot);

		struct RootEvent {
			static char** eventFlowDefinition;

			static void Locate(HMODULE peRoot);
		};
	}
}