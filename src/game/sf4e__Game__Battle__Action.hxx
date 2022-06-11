#pragma once

#include "Dimps__Game__Battle__Action.hxx"

namespace sf4e {
	namespace Game {
		namespace Battle {
			namespace Action {
				void Install();

				struct Actor : Dimps::Game::Battle::Action::Actor {
					size_t GetMementoSize();

					static void Install();
				};
			}
		}
	}
}