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
				// TODO
			} __staticMethods;

			static void Locate(HMODULE peRoot);
			static __privateMethods privateMethods;
			static __publicMethods publicMethods;
			static __staticMethods staticMethods;
		};
	}
}