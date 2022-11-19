#pragma once

#include <windows.h>
#include "Dimps__Platform.hxx"

namespace sf4e {
	namespace Platform {
		void Install();

		struct D3D : Dimps::Platform::D3D
		{
			static void Install();

			void Destroy();
			static void WINAPI RunD3DOperations(void* arg1);
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