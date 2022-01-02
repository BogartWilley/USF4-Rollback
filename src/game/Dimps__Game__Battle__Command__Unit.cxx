#include <windows.h>

#include "Dimps__Game__Battle__Command__Unit.hxx"

using Dimps::Game::Battle::Command::Unit;

Unit::__privateMethods Unit::privateMethods;
Unit::__publicMethods Unit::publicMethods;
Unit::__staticMethods Unit::staticMethods;

void Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)(&publicMethods.GetCommandImplForEntry) = (PVOID)(peRootOffset + 0x199400);
}