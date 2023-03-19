#pragma once

#include <windows.h>

#include "Dimps__Eva.hxx"

namespace Dimps {
	namespace Game {
		namespace Battle {
			using Dimps::Eva::Task;

			void Locate(HMODULE peRoot);

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