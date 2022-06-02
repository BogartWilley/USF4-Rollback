#pragma once

#include <windows.h>
#include <d3d9.h>

namespace Dimps {
	namespace Platform {
		void Locate(HMODULE peRoot);

		struct D3D
		{
			typedef struct __privateMethods {
				void(D3D::* Destroy)();
			} __privateMethods;

			typedef struct __publicMethods {
				// TODO
			} __publicMethods;

			typedef struct __staticMethods {
				void (WINAPI* RunD3DOperations)(void* arg1);
				D3D* (WINAPI* GetSingleton)();
			} __staticMethods;

			static void Locate(HMODULE peRoot);
			static __privateMethods privateMethods;
			static __publicMethods publicMethods;
			static __staticMethods staticMethods;

			// Instance values here
			void* vtable;
			HWND hFocusWindow;
			DWORD workingMem;
			DWORD deviceType;
			DWORD behaviorFlags;
			IDirect3D9* lpD3D;
			IDirect3DDevice9* lpD3DDevice;
		};

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