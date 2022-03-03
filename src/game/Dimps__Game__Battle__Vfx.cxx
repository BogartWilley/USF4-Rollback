#include <windows.h>

#include "Dimps__Game__Battle__Vfx.hxx"

namespace Vfx = Dimps::Game::Battle::Vfx;
using Vfx::Unit;

Unit::__privateMethods Unit::privateMethods;
Unit::__publicMethods Unit::publicMethods;
Unit::__staticMethods Unit::staticMethods;

void Vfx::Locate(HMODULE peRoot) {
	Unit::Locate(peRoot);
}

void Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.RecordToInternalMementoKey = (PVOID)(peRootOffset + 0x1b6800);
	*(PVOID*)&publicMethods.RestoreFromInternalMementoKey = (PVOID)(peRootOffset + 0x1b6840);
}