#pragma once

#include <windows.h>

#include "Dimps__Game.hxx"

using Dimps::Game::GameMementoKey;

namespace Dimps {
	namespace Game {
		namespace Battle {
			namespace Training {
				void Locate(HMODULE peRoot);

				struct Manager
				{
					typedef struct __privateMethods {
						// TODO
					} __privateMethods;

					typedef struct __publicMethods {
						void (Manager::* RecordToInternalMementoKey)(GameMementoKey::MementoID* id);
						void (Manager::* RestoreFromInternalMementoKey)(GameMementoKey::MementoID* id);
					} __publicMethods;

					typedef struct __staticMethods {
						Manager* (*GetSingleton)();
					} __staticMethods;

					static void Locate(HMODULE peRoot);
					static __privateMethods privateMethods;
					static __publicMethods publicMethods;
					static __staticMethods staticMethods;

					// Instance values here
				};
			}
		}
	}
}