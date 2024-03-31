#pragma once

#include <stdint.h>
#include <windows.h>

#include "Dimps__Eva.hxx"
#include "Dimps__Math.hxx"

#define NUM_VALID_EDITIONS 7

namespace Dimps {
	namespace Game {
		namespace Battle {
			using Dimps::Eva::Task;
			using Dimps::Math::FixedPoint;

			void Locate(HMODULE peRoot);

			enum Edition {
				ED_SSF4 = 1,
				ED_AE2011 = 2,
				ED_AE2012 = 4,
				ED_USF4 = 14,
				ED_SF4 = 13,
				ED_OMEGA = 16
			};

			extern int orderedEditions[NUM_VALID_EDITIONS];

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

			struct ValidEditions {
				uint8_t valid[NUM_VALID_EDITIONS];
			};
			extern ValidEditions* validEditionsPerChara;

			struct GameManager {
				char pad[0x49c];

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

			namespace Network {
				struct Unit {
					char pad[0x64f4];
				};
			}

			namespace Pause {
				struct Unit {
					char pad[0x18];

					static Eva::Task** GetPauseTask(Unit* u);
				};
			}

			namespace Sound {
				// Sound methods often return handles or take them as arguments-
				// they seem intended to be opaque, and the fact that most
				// functions return these values in `eax` and receive them
				// without allocating an additional pointer on the stack
				// suggests that the sound subsystem is using basic types to
				// exchange the data. However, they can be decoded to
				// references.
				typedef unsigned int SoundHandle;
				struct SoundReference {
					unsigned short position;
					unsigned short useCount;
					inline static SoundReference FromHandle(SoundHandle h) {
						return SoundReference{ 
							(unsigned short)(h & 0xffff0000 >> 0x10),
							(unsigned short)(h & 0xffff)
						};
					}
					inline SoundHandle ToHandle() { return this->position << 0x10 & this->useCount; }
				};

				struct SoundPlayerManager {
					typedef struct __publicMethods {
						SoundHandle(SoundPlayerManager::* PlaySound)(SoundHandle cueSheetHandle, uint32_t cueIdx, DWORD param_3, int32_t flags, DWORD maybePosition);
					} __publicMethods;

					static void Locate(HMODULE peRoot);
					static __publicMethods publicMethods;
				};
			}
		}
	}
}