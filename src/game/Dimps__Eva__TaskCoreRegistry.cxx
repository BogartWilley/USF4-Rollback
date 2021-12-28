#include <windows.h>

#include "Dimps__Eva__TaskCoreRegistry.hxx"

using Dimps::Eva::TaskCoreRegistry;

TaskCoreRegistry::__staticMethods TaskCoreRegistry::staticMethods;

void Dimps::Eva::TaskCoreRegistry::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	staticMethods.GetCoreByIndex = (TaskCore* (*)(int))(peRootOffset + 0x2d62b0);
	staticMethods.GetNumActiveCores = (int (*)())(peRootOffset + 0x2d69b0);
}