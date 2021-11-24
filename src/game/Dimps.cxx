#include <windows.h>

#include "Dimps.hxx"
#include "Dimps__Platform.hxx"

void Dimps::Locate(HMODULE peRoot) {
	Dimps::Platform::Locate(peRoot);
}