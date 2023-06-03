#pragma once

#include <windows.h>

#include "../Dimps/Dimps__Event.hxx"

namespace sf4e {
	namespace Event {
		void Install();

		struct EventController : Dimps::Event::EventController
		{
			static void Install();
			static bool bHaltAfterNext;
			static bool bUpdateAllowed;

			static void ReplaceNextEvent(char* replacementEventName);
			void QueueEvent(char* eventName, DWORD arg1, DWORD arg2, DWORD arg3);
			void RunUpdate();

		private:
			static char* replacementEvent;
		};
	}
}