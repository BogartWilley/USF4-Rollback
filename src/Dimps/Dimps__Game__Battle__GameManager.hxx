#pragma once

#include <windows.h>
#include "Dimps__Math.hxx"

using Dimps::Math::FixedPoint;

namespace Dimps {
	namespace Game {
		namespace Battle {
			struct GameManager {
				typedef struct __privateMethods {
					// TODO
				} __privateMethods;

				typedef struct __publicMethods {
					void (GameManager::* GetRoundTime)(FixedPoint* out);
				} __publicMethods;

				typedef struct __staticMethods {
					// TODO
				} __staticMethods;

				static void Locate(HMODULE peRoot);
				static __privateMethods privateMethods;
				static __publicMethods publicMethods;
				static __staticMethods staticMethods;
			};
		}
	}
}
