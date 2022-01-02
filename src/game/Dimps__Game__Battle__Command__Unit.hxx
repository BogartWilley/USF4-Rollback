#pragma once

#include <windows.h>

#include "Dimps__Game__Battle__Command__CommandImpl.hxx"

using Dimps::Game::Battle::Command::CommandImpl;

namespace Dimps {
	namespace Game {
		namespace Battle {
			namespace Command {
				struct Unit
				{
					typedef struct __privateMethods {
						// TODO
					} __privateMethods;

					typedef struct __publicMethods {
						CommandImpl* (Unit::* GetCommandImplForEntry)(unsigned int index);
					} __publicMethods;

					typedef struct __staticMethods {
						// TODO
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