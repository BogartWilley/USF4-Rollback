#pragma once

#include <windows.h>

#include "Dimps__Eva.hxx"
#include "Dimps__Math.hxx"

namespace Dimps {
	namespace Game {
		namespace Battle {
			using Dimps::Eva::Task;
			using Dimps::Math::FixedPoint;

			void Locate(HMODULE peRoot);

			enum GameMode {
				GAMEMODE_ARCADE = 0x0,
				GAMEMODE_VERSUS = 0x1,
				GAMEMODE_BENCHMARK_DEMO = 0x2,
				GAMEMODE_TRAINING = 0x3,
				GAMEMODE_TRIAL = 0x5,
				GAMEMODE_BONUS = 0x6,

				// These appear to be dead, at least in consumer builds.
				// Notably, "RivalBattleMode" and "BossBattleMode" are referenced
				// by the root event's flow, but no corresponding event class
				// exists in the code. Likely, these were for debugging purposes.
				GAMEMODE_DEAD_4 = 0x4,
				GAMEMODE_DEAD_7 = 0x7,

				GAMEMODE_UNDEFINED = 0xffffffff
			};

			struct GameManager {
				typedef struct __publicMethods {
					void (GameManager::* GetRoundTime)(FixedPoint* out);
				} __publicMethods;

				static void Locate(HMODULE peRoot);
				static __publicMethods publicMethods;
			};

			struct IUnit {
				typedef struct __publicMethods {
					void (IUnit::* SharedHudUpdate)(Task** task);
				} __publicMethods;

				static void Locate(HMODULE peRoot);
				static __publicMethods publicMethods;
			};
		}
	}
}