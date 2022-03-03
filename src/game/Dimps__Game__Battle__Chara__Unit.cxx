#include <windows.h>

#include "Dimps__Game__Battle__Chara__Unit.hxx"

using Dimps::Game::Battle::Chara::Unit;

Unit::__privateMethods Unit::privateMethods;
Unit::__publicMethods Unit::publicMethods;
Unit::__staticMethods Unit::staticMethods;

void Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)&publicMethods.GetActorByIndex = (PVOID)(peRootOffset + 0x165d10);
	*(PVOID*)&publicMethods.CanStoreMemento_MaybeActorExists = (PVOID)(peRootOffset + 0x165cd0);
	*(PVOID*)&publicMethods.RecordToInternalMementoKey = (PVOID)(peRootOffset + 0x1633a0);
	*(PVOID*)&publicMethods.RestoreFromInternalMementoKey = (PVOID)(peRootOffset + 0x1633f0);
}