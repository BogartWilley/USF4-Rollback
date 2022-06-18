#pragma once

#include <windows.h>

#include "Dimps__GameEvents.hxx"

namespace sf4e {
	namespace GameEvents {
		void Install();

		struct RootEvent : Dimps::GameEvents::RootEvent
		{
			static char* eventFlowDescription;
			static void Install();
		};

		struct VsCharaSelect : Dimps::GameEvents::VsCharaSelect
		{
			void* Destroy(DWORD arg1);

			static Dimps::GameEvents::VsCharaSelect* instance;
			static Dimps::GameEvents::VsCharaSelect* Factory(DWORD arg1, DWORD arg2, DWORD arg3);
			static void Install();
		};
	}
}