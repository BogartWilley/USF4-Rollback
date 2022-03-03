#include <windows.h>

#include "Dimps__Game__Battle__Hud.hxx"

namespace Hud = Dimps::Game::Battle::Hud;
using Hud::Unit;

Unit::__privateMethods Unit::privateMethods;
Unit::__publicMethods Unit::publicMethods;
Unit::__staticMethods Unit::staticMethods;

void Hud::Locate(HMODULE peRoot) {
	Unit::Locate(peRoot);
}

void Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.RecordToInternalMementoKey = (PVOID)(peRootOffset + 0x172e60);
	*(PVOID*)&publicMethods.RestoreFromInternalMementoKey = (PVOID)(peRootOffset + 0x172e90);
}