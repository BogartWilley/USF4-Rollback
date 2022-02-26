#pragma once

#include <windows.h>

#include "Dimps__Eva__TaskCore.hxx"

namespace Dimps {
	namespace GameEvents {
		struct RootEvent {
			static char** eventFlowDefinition;

			static void Locate(HMODULE peRoot);
		};
	}
}