#pragma once

#include <string>
#include <windows.h>

namespace Dimps {
	namespace GameEvents {
		void Locate(HMODULE peRoot);

		struct RootEvent {
			static char** eventFlowDefinition;

			static void Locate(HMODULE peRoot);
		};

		struct VsCharaSelect {
			struct PlayerConditions {
				BYTE pad[492];

				static std::string* GetSelectedCharaAbbrev(PlayerConditions* c);
				static std::string* GetHoveredCharaAbbrev(PlayerConditions* c);
				static BYTE* GetColor(PlayerConditions* c);
				static BYTE* GetCostume(PlayerConditions* c);
				static BYTE* GetPersonalAction(PlayerConditions* c);
				static BYTE* GetUltraCombo(PlayerConditions* c);
				static BYTE* GetWinQuote(PlayerConditions* c);
				static BYTE* GetEdition(PlayerConditions* c);
			};

			static void Locate(HMODULE peRoot);
			static PlayerConditions* GetPlayerConditions(VsCharaSelect* event);

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