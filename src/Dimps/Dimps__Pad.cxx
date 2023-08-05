#include <windows.h>

#include "Dimps__Pad.hxx"

namespace Pad = Dimps::Pad;
using Pad::System;

System::__publicMethods System::publicMethods;
System::__staticMethods System::staticMethods;

const int System::BUTTON_MAPPING_FIGHT = 0;
const int System::BUTTON_MAPPING_MENU = 1;

void Pad::Locate(HMODULE peRoot) {
	System::Locate(peRoot);
}

void System::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)(&publicMethods.GetButtons_RawOn) = (PVOID)(peRootOffset + 0x117130);
	*(PVOID*)(&publicMethods.GetButtons_RawRising) = (PVOID)(peRootOffset + 0x117150);
	*(PVOID*)(&publicMethods.GetButtons_RawFalling) = (PVOID)(peRootOffset + 0x117170);
	*(PVOID*)(&publicMethods.GetButtons_RawRisingWithRepeat) = (PVOID)(peRootOffset + 0x117190);
	*(PVOID*)(&publicMethods.GetButtons_MappedOn) = (PVOID)(peRootOffset + 0x1171b0);
	*(PVOID*)(&publicMethods.GetAllDeviceCount) = (PVOID)(peRootOffset + 0x110710);
	*(PVOID*)(&publicMethods.GetOKDeviceCount) = (PVOID)(peRootOffset + 0x110740);
	*(PVOID*)(&publicMethods.GetDeviceName) = (PVOID)(peRootOffset + 0x111cf0);
	*(PVOID*)(&publicMethods.GetDeviceIndexForPlayer) = (PVOID)(peRootOffset + 0x117240);
	*(PVOID*)(&publicMethods.GetDeviceTypeForPlayer) = (PVOID)(peRootOffset + 0x117290);
	*(PVOID*)(&publicMethods.GetAssigmentStatusForPlayer) = (PVOID)(peRootOffset + 0x1173c0);
	*(PVOID*)(&publicMethods.AssociatePlayerAndGamepad) = (PVOID)(peRootOffset + 0x117530);
	*(PVOID*)(&publicMethods.SetSideHasAssignedController) = (PVOID)(peRootOffset + 0x117360);
	*(PVOID*)(&publicMethods.SetDeviceTypeForPlayer) = (PVOID)(peRootOffset + 0x117270);
	*(PVOID*)(&publicMethods.SetActiveButtonMapping) = (PVOID)(peRootOffset + 0x110170);
	*(PVOID*)(&publicMethods.CaptureNextMatchingPadToSide) = (PVOID)(peRootOffset + 0x111110);

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