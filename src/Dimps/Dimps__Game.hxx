#pragma once

#include <windows.h>

namespace Dimps {
	namespace Game {
		void Locate(HMODULE peRoot);

		struct GameMementoKey {
			struct MementoID {
				DWORD lo;
				DWORD hi;
			};

			struct Metadata {
				MementoID id;
				void* memento;
			};

			typedef struct __publicMethods {
				void (GameMementoKey::* Initialize)(void* mementoable, int numMementos);
				void (GameMementoKey::* ClearKey)();
			} __publicMethods;

			static void Locate(HMODULE peRoot);
			static __publicMethods publicMethods;

			static int* totalMementoSize;

			void* mementoableObject;
			Metadata* metadata;
			int numMementos;
			void* mementos;
			int sizeAllocated;
			int nextMementoIndex;
		};
	}
}