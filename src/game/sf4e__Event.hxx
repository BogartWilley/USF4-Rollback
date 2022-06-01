#pragma once

#include "Dimps__Event.hxx"

namespace sf4e {
	namespace Event {
		void Install();

		struct EventController : Dimps::Event::EventController
		{
			static void Install();
			static bool bHaltAfterNext;
			static bool bUpdateAllowed;

			void RunUpdate();
		};
	}
}