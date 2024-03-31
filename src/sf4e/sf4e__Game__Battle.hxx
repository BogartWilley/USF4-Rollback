#pragma once

#include <windows.h>

#include "../Dimps/Dimps__Eva.hxx"
#include "../Dimps/Dimps__Game__Battle.hxx"
#include "../Dimps/Dimps__Game__Battle__Camera.hxx"
#include "../Dimps/Dimps__Game__Battle__Chara.hxx"
#include "../Dimps/Dimps__Game__Battle__Command.hxx"
#include "../Dimps/Dimps__Game__Battle__Training.hxx"

namespace sf4e {
	namespace Game {
		namespace Battle {
			using Dimps::Eva::Task;
			using Dimps::Game::Battle::Sound::SoundHandle;

			void Install();

			struct IUnit : Dimps::Game::Battle::IUnit {
				// In order for the compiler to construct this method
				// with __thiscall__, the method needs to be declared
				// in the class declaration, despite the fact that this
				// method isn't actually part of the IUnit interface.
				//
				// This is intentionally not installed as part of IUnit.
				void SharedHudUpdate(Task** task);

				static bool bAllowHudUpdate;
			};

			namespace Sound {
				struct SoundPlayerManager : Dimps::Game::Battle::Sound::SoundPlayerManager {
					SoundHandle PlaySound(
						SoundHandle cueSheetHandle,
						uint32_t cueIdx,
						DWORD param_3,
						int32_t flags,
						DWORD maybePosition
					);

					static bool bTrackPlays;
					static void Install();
				};
			}
		}
	}
}