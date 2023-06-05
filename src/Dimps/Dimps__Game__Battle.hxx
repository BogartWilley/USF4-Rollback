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