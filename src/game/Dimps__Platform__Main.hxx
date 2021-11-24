#pragma once

#include <windows.h>

namespace Dimps {
	namespace Platform {
		struct Main
		{
			typedef struct __privateMethods {
				// TODO
			} __privateMethods;

			typedef struct __publicMethods {
				int (Main::* Initialize)(void*, void*, void*);
			} __publicMethods;

			typedef struct __staticMethods {
				void (WINAPI* RunWindowFunc)(Main* lpMain, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			} __staticMethods;

			static void Locate(HMODULE peRoot);
			static __privateMethods privateMethods;
			static __publicMethods publicMethods;
			static __staticMethods staticMethods;

			typedef struct Win32_WindowData {
				void* field_0x0;
				HWND hWnd;
				char field_0x8[44];
			} Win32_WindowData;
		};
	}
}