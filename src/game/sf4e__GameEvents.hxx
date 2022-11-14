#pragma once

#include <windows.h>

#include "Dimps__GameEvents.hxx"

namespace sf4e {
	namespace GameEvents {
		void Install();

		struct MainMenu : Dimps::GameEvents::MainMenu
		{
			void* Destroy(DWORD arg1);

			static Dimps::GameEvents::MainMenu* instance;
			static Dimps::GameEvents::MainMenu* Factory(DWORD arg1, DWORD arg2, DWORD arg3);
			static void Install();
		};

		struct RootEvent : Dimps::GameEvents::RootEvent
		{
			static char* eventFlowDescription;
			static void Install();
		};

		struct VsMode : Dimps::GameEvents::VsMode
		{
			void* Destroy(DWORD arg1);

			static Dimps::GameEvents::VsMode* instance;
			static Dimps::GameEvents::VsMode* Factory(DWORD arg1, DWORD arg2, DWORD arg3);
			static void Install();
		};

		struct VsCharaSelect : Dimps::GameEvents::VsCharaSelect
		{
			void* Destroy(DWORD arg1);

			static Dimps::GameEvents::VsCharaSelect* instance;
			static Dimps::GameEvents::VsCharaSelect* Factory(DWORD arg1, DWORD arg2, DWORD arg3);
			static void Install();
		};

		struct VsPreBattle : Dimps::GameEvents::VsPreBattle
		{
			static bool bSkipToVersus;
			static int skipP1Chara;
			static int skipP2Chara;
			static int skipStage;

			void RegisterTasks();
			static void Install();
		};

		struct VsStageSelect : Dimps::GameEvents::VsStageSelect
		{
			void* Destroy(DWORD arg1);

			static bool forceTimerOnNextStageSelect;
			static Dimps::GameEvents::VsStageSelect* instance;
			static Dimps::GameEvents::VsStageSelect* Factory(DWORD arg1, DWORD arg2, DWORD arg3);
			static void Install();
		};
	}
}