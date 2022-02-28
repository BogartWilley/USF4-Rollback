#include <windows.h>

#include "Dimps__Pad.hxx"

namespace Pad = Dimps::Pad;
using Pad::System;

System::__publicMethods System::publicMethods;
System::__staticMethods System::staticMethods;

void Pad::Locate(HMODULE peRoot) {
	System::Locate(peRoot);
}

void System::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)(&publicMethods.GetButtons_On) = (PVOID)(peRootOffset + 0x117130);
	*(PVOID*)(&publicMethods.GetButtons_Rising) = (PVOID)(peRootOffset + 0x117150);
	*(PVOID*)(&publicMethods.GetButtons_Falling) = (PVOID)(peRootOffset + 0x117170);
	*(PVOID*)(&publicMethods.GetButtons_RisingWithRepeat) = (PVOID)(peRootOffset + 0x117190);
	staticMethods.GetSingleton = (System * (*)())(peRootOffset + 0x119480);
}