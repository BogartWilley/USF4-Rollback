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
			static void Locate(HMODULE peRoot);

			typedef struct __publicMethods {
				// 17 different children of EventBase (most Battle-related events)
				// have identical implementations of their wait-for-exit method,
				// and their only shared ancestor is EventBase. Since the
				// implementations are identical, the compiler deduplicated the
				// implementations, which makes it impossible to safely represent
				// the original source implementation with pointers in each
				// battle-event class. Instead, _one_ pointer to the deduplicated
				// implementation is stored here, in the nearest parent.
				BOOL (EventBase::* IsTerminationComplete)();
			} __publicMethods;

			static __publicMethods publicMethods;
		};
	}
}