#include <windows.h>

#include "Dimps__Game__Battle__System.hxx"

using Dimps::Game::Battle::System;

System::__privateMethods System::privateMethods;
System::__publicMethods System::publicMethods;
System::__staticMethods System::staticMethods;

void System::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)&publicMethods.GetCharaUnit = (PVOID)(peRootOffset + 0x163510);
	*(PVOID*)&publicMethods.IsFight = (PVOID)(peRootOffset + 0x1d9f60);
	*(PVOID*)&publicMethods.IsLeavingBattle = (PVOID)(peRootOffset + 0x1d6a70);
	staticMethods.GetSingleton = (System * (WINAPI*)())(peRootOffset + 0x1dba30);
}