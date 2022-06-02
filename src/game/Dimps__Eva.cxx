#include <windows.h>

#include "Dimps__Eva.hxx"

namespace Eva = Dimps::Eva;
using Dimps::Eva::TaskCore;
using Dimps::Eva::TaskCoreRegistry;

TaskCore::__privateMethods TaskCore::privateMethods;
TaskCore::__publicMethods TaskCore::publicMethods;
TaskCore::__staticMethods TaskCore::staticMethods;
TaskCoreRegistry::__staticMethods TaskCoreRegistry::staticMethods;

void Eva::Locate(HMODULE peRoot) {
	TaskCore::Locate(peRoot);
	TaskCoreRegistry::Locate(peRoot);
}

void TaskCore::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)(&publicMethods.GetName) = (PVOID)(peRootOffset + 0x2d4200);
}

void TaskCoreRegistry::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	staticMethods.GetCoreByIndex = (TaskCore * (*)(int))(peRootOffset + 0x2d62b0);
	staticMethods.GetNumActiveCores = (int (*)())(peRootOffset + 0x2d69b0);
}