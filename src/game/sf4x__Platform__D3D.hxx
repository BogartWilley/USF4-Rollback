#pragma once

#include <windows.h>
#include "game.h"
#include "Dimps__Platform__D3D.hxx"

namespace sf4x {
	namespace Platform {
		struct D3D : Dimps::Platform::D3D
		{
			static void Install(GameMethods* methods, GameState* state);

			void FakeDestroy();
			static void WINAPI FakeRunD3DOperations(void* arg1);
		};
	}
}