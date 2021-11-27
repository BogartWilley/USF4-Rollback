#pragma once

#include <windows.h>

#include "Dimps__Game__Battle__Chara__Actor.hxx"

using Dimps::Game::Battle::Chara::Actor;

namespace Dimps {
	namespace Game {
		namespace Battle {
			namespace Chara {
				struct Unit
				{
					typedef struct __privateMethods {
						// TODO
					} __privateMethods;

					typedef struct __publicMethods {
						Actor* (Unit::* GetActorByIndex)(unsigned int index);
					} __publicMethods;

					typedef struct __staticMethods {
						// TODO
					} __staticMethods;

					static void Locate(HMODULE peRoot);
					static __privateMethods privateMethods;
					static __publicMethods publicMethods;
					static __staticMethods staticMethods;

					// Instance values here
					void* vtable;
					DWORD pad_0x4;
					Actor* playerActors[2];
					void* actors_0x10[2];
					void* actors_0x18[2];
					void* lpItemCollection;
				};
			}
		}
	}
}