#pragma once

#include <windows.h>

namespace Dimps {
	namespace Event {
		void Locate(HMODULE peRoot);

		struct EventController {
			static void Locate(HMODULE peRoot);

			typedef struct __publicMethods {
				void (EventController::* RunUpdate)();
			} __publicMethods;

			static __publicMethods publicMethods;
		};
	}
}