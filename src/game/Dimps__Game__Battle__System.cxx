#include <windows.h>

#include "Dimps__Game__Battle__System.hxx"

using Dimps::Game::Battle::System;

System::__privateMethods System::privateMethods;
System::__publicMethods System::publicMethods;
System::__staticMethods System::staticMethods;

void System::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)&publicMethods.GetCharaUnit = (PVOID)(peRootOffset + 0x163510);
	*(PVOID*)&publicMethods.GetGameManager = (PVOID)(peRootOffset + 0x1d9950);
	*(PVOID*)&publicMethods.GetUnitByIndex = (PVOID)(peRootOffset + 0x1d9720);
	*(PVOID*)&publicMethods.IsFight = (PVOID)(peRootOffset + 0x1d9f60);
	*(PVOID*)&publicMethods.IsLeavingBattle = (PVOID)(peRootOffset + 0x1d6a70);
	*(PVOID*)&publicMethods.RecordAllToInternalMementoKeys = (PVOID)(peRootOffset + 0x1d7f80);
	*(PVOID*)&publicMethods.RestoreAllFromInternalMementoKeys = (PVOID)(peRootOffset + 0x1d8020);

	*(PVOID*)&publicMethods.RecordToInternalMementoKey = (PVOID)(peRootOffset + 0x1daab0);
	*(PVOID*)&publicMethods.RestoreFromInternalMementoKey = (PVOID)(peRootOffset + 0x1d90d0);

	staticMethods.GetSingleton = (System*(*)())(peRootOffset + 0x1dba30);
}