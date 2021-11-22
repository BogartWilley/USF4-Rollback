#pragma once

#include <windows.h>
#include "game.h"
#include "Dimps__Platform__Main.hxx"

namespace sf4x {
	namespace Platform {
		struct Main : Dimps::Platform::Main
		{
			static void Install(GameState* _lpGameState);

			int FakeInitialize(void*, void*, void*);
		};
	}
}