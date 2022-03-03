#include <windows.h>

#include "Dimps__Game__Battle__Effect.hxx"

namespace Effect = Dimps::Game::Battle::Effect;
using Effect::Unit;

Unit::__privateMethods Unit::privateMethods;
Unit::__publicMethods Unit::publicMethods;
Unit::__staticMethods Unit::staticMethods;

void Effect::Locate(HMODULE peRoot) {
	Unit::Locate(peRoot);
}

void Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)(&publicMethods.RecordToInternalMementoKey) = (PVOID)(peRootOffset + 0x16ee70);
	*(PVOID*)(&publicMethods.RestoreFromInternalMementoKey) = (PVOID)(peRootOffset + 0x16eea0);
}