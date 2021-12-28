#include <windows.h>

#include "Dimps__Eva.hxx"
#include "Dimps__Eva__TaskCore.hxx"
#include "Dimps__Eva__TaskCoreRegistry.hxx"

void Dimps::Eva::Locate(HMODULE peRoot) {
	Dimps::Eva::TaskCore::Locate(peRoot);
	Dimps::Eva::TaskCoreRegistry::Locate(peRoot);
}