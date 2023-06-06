#pragma once

#include <windows.h>

namespace Dimps {
	namespace Event {
		void Locate(HMODULE peRoot);

		struct EventController {
			static void Locate(HMODULE peRoot);

			typedef struct __publicMethods {
				void (EventController::* CreateEventWithFlow)(DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5);
				void (EventController::* EnterTerminalState)(DWORD arg1, DWORD arg2);
				void (EventController::* QueueEvent)(char* eventName, DWORD arg1, DWORD arg2, DWORD arg3);
				void (EventController::* RunUpdate)();
			} __publicMethods;

			static __publicMethods publicMethods;
		};

		struct EventBase {
			static char* GetName(EventBase* e);
			static EventController** GetSourceController(EventBase* e);
		};
	}
}