#pragma once

#include "Dimps__GameEvents__RootEvent.hxx"

namespace sf4e {
	namespace GameEvents {
		struct RootEvent : Dimps::GameEvents::RootEvent
		{
			static char* eventFlowDescription;
			static void Install();
		};
	}
}