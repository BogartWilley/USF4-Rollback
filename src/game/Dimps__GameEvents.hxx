#pragma once

#include <string>
#include <windows.h>

#include "Dimps__Event.hxx"
#include "Dimps__Platform.hxx"

namespace Dimps {
	namespace GameEvents {
		void Locate(HMODULE peRoot);

		struct RootEvent : Dimps::Event::EventBase {
			static char** eventFlowDefinition;

			static void Locate(HMODULE peRoot);
		};

		struct MainMenu : Dimps::Event::EventBase {
			static void Locate(HMODULE peRoot);

			typedef struct __publicMethods {
				void* (MainMenu::* Destroy)(DWORD arg1);
			} __publicMethods;

			typedef struct __staticMethods {
				MainMenu* (*Factory)(DWORD arg1, DWORD arg2, DWORD arg3);
			} __staticMethods;

			static __publicMethods publicMethods;
			static __staticMethods staticMethods;
		};

		struct VsCharaSelect : Dimps::Event::EventBase {
			enum CharaSelectFlags {
				CHARASEL_P1_SIDEENABLED = 0x1,
				CHARASEL_P2_SIDEENABLED = 0x2,
				CHARASEL_SINGLE_SELECTOR = 0x4,
				CHARASEL_P1_REMOTE_BLINDPICK = 0x8,
				CHARASEL_P2_REMOTE_BLINDPICK = 0x10,
				CHARASEL_TIMER_ENABLED = 0x20,
				CHARASEL_ALLOW_CUSTOM_SELECT = 0x800,
				CHARASEL_ALLOW_UC_SELECT = 0x1000,
				CHARASEL_ALLOW_HANDICAP_SELECT = 0x2000,
				CHARASEL_ALLOW_CANCEL = 0x8000
			};

			struct PlayerConditions {
				BYTE pad[492];

				static Platform::dString* GetSelectedCharaAbbrev(PlayerConditions* c);
				static Platform::dString* GetHoveredCharaAbbrev(PlayerConditions* c);
				static BYTE* GetColor(PlayerConditions* c);
				static BYTE* GetCostume(PlayerConditions* c);
				static BYTE* GetPersonalAction(PlayerConditions* c);
				static BYTE* GetUltraCombo(PlayerConditions* c);
				static BYTE* GetWinQuote(PlayerConditions* c);
				static BYTE* GetHandicap(PlayerConditions* c);
				static BYTE* GetEdition(PlayerConditions* c);
			};

			struct CharaSelectState {
				DWORD flags;
				PlayerConditions playerConditions[2];
			};

			static void Locate(HMODULE peRoot);
			static CharaSelectState* GetState(VsCharaSelect* event);

			typedef struct __publicMethods {
				void* (VsCharaSelect::* Destroy)(DWORD arg1);
			} __publicMethods;

			typedef struct __staticMethods {
				VsCharaSelect* (*Factory)(DWORD arg1, DWORD arg2, DWORD arg3);
			} __staticMethods;

			static __publicMethods publicMethods;
			static __staticMethods staticMethods;
		};
	}
}