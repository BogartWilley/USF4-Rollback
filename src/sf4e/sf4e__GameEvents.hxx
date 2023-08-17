#pragma once

#include <windows.h>

#include "../Dimps/Dimps__GameEvents.hxx"
#include "../Dimps/Dimps__Platform.hxx"

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

		struct VsBattle : Dimps::GameEvents::VsBattle
		{
			int CheckAndMaybeExitBasedOnExitType();
			void PrepareBattleRequest();
			static bool bForceNextMatchOnline;
			static bool bOverrideNextRandomSeed;
			static bool bTerminateOnNextLeftBattle;
			static DWORD nextMatchRandomSeed;
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
			static void (*OnTasksRegistered)();
			static bool bSkipToVersus;

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