#pragma once

#include <string>
#include <windows.h>
#include <d3d9.h>

namespace Dimps {
	namespace Platform {
		template <class T>
		struct list_entry {
			list_entry<T>* next;
			list_entry<T>* prev;
			T data;
		};

		template <class T>
		struct list {
			list_entry<T>* root;
			int numUsed;
		};

		// While SF4 seems to use std::string internally and std::string
		// pointers can actually be used to read data, they can't naively
		// be used to _write_ data. It's unclear if this is because SF4
		// uses a custom allocator for strings, because the DLL is linking
		// against a different std::string implementation that could be solved
		// with a different msvcrt, or another unidentified reason.
		// Regardless, we can hack around all of these by "de-templatizing"
		// the allocator and locating the allocator methods as if they were
		// method pointers of a string.
		struct dString : std::string {
			typedef struct __publicMethods {
				dString* (dString::* assign)(const char* s, int count);
			} __publicMethods;

			static __publicMethods publicMethods;
			static void Locate(HMODULE peRoot);
		};

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
				void (Main::* Destroy)();
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