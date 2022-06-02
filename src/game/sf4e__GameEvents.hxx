#pragma once

#include "Dimps__GameEvents.hxx"

namespace sf4e {
	namespace GameEvents {
		void Install();

		struct RootEvent : Dimps::GameEvents::RootEvent
		{
			static char* eventFlowDescription;
			static void Install();
		};
	}
}