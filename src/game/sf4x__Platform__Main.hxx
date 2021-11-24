#pragma once

#include <windows.h>
#include "Dimps__Platform__Main.hxx"

namespace sf4x {
	namespace Platform {
		struct Main : Dimps::Platform::Main
		{
			static void Install();

			int Initialize(void*, void*, void*);
			static void WINAPI RunWindowFunc(Dimps::Platform::Main* lpMain, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		};
	}
}