#include <windows.h>

#include "Dimps__Game__Battle__Action.hxx"

namespace Action = Dimps::Game::Battle::Action;
using Action::Actor;

Actor::__publicMethods Actor::publicMethods;

void Action::Locate(HMODULE peRoot) {
	Actor::Locate(peRoot);
}

void Actor::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.GetMementoSize = (PVOID)(peRootOffset + 0x12b730);
}