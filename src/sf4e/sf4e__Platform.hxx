#pragma once

#include <windows.h>
#include "../Dimps/Dimps__Platform.hxx"
#include "sf4e.hxx"

namespace sf4e {
	namespace Platform {
		void Install();

		struct D3D : Dimps::Platform::D3D
		{
			static void Install();

			void Destroy();
			static void WINAPI RunD3DOperations(void* arg1);
		};

		struct GFxApp : Dimps::Platform::GFxApp
		{
			struct AdditionalMemento {
				std::pair<bool, sf4e::Eva::IEmSpriteAction::AdditionalMemento> actions[NUM_GFX_ACTIONS];
			};

			static void RecordToAdditionalMemento(Dimps::Platform::GFxApp* a, AdditionalMemento& m);
			static void RestoreFromAdditionalMemento(Dimps::Platform::GFxApp* a, const AdditionalMemento& m);
		};

		struct Main : Dimps::Platform::Main
		{
			static void Install();

			int Initialize(void*, void*, void*);
			void Destroy();
			static void WINAPI RunWindowFunc(Dimps::Platform::Main* lpMain, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		};
	}
}