#pragma once

#include <set>

#include "Dimps__Game.hxx"

namespace sf4e {
	namespace Game {
		void Install();

		struct GameMementoKey : Dimps::Game::GameMementoKey
		{
			void Initialize(void* mementoable, int numMementos);
			void ClearKey();
			static void Install();

			static bool bEnableUnsafeReinitializationSkip;
			static std::set<Dimps::Game::GameMementoKey*> trackedKeys;
			static void (*SizeLogger)(Dimps::Game::GameMementoKey* k, int oldSize);
		};
	}
}