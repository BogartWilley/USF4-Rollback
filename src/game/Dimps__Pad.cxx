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
	*(PVOID*)(&publicMethods.GetButtons_Mapped) = (PVOID)(peRootOffset + 0x1171b0);
	*(PVOID*)(&publicMethods.GetAllDeviceCount) = (PVOID)(peRootOffset + 0x110710);
	*(PVOID*)(&publicMethods.GetOKDeviceCount) = (PVOID)(peRootOffset + 0x110740);
	*(PVOID*)(&publicMethods.GetDeviceName) = (PVOID)(peRootOffset + 0x111cf0);
	*(PVOID*)(&publicMethods.GetDeviceIndexForPlayer) = (PVOID)(peRootOffset + 0x117240);
	*(PVOID*)(&publicMethods.GetDeviceTypeForPlayer) = (PVOID)(peRootOffset + 0x117290);
	*(PVOID*)(&publicMethods.GetAssigmentStatusForPlayer) = (PVOID)(peRootOffset + 0x1173c0);

	staticMethods.GetSingleton = (System * (*)())(peRootOffset + 0x119480);
}

int* System::PlayerEntry::DeviceIndex(System::PlayerEntry* e) {
	return (int*)((unsigned int)e + 0x0);
}

int* System::PlayerEntry::DeviceType(System::PlayerEntry* e) {
	return (int*)((unsigned int)e + 0x44);
}

int* System::PlayerEntry::AssignedController(System::PlayerEntry* e) {
	return (int*)((unsigned int)e + 0x48);
}