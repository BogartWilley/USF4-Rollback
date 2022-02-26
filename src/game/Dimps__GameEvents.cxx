#include <windows.h>

#include "Dimps__GameEvents.hxx"
#include "Dimps__GameEvents__RootEvent.hxx"

void Dimps::GameEvents::Locate(HMODULE peRoot) {
	Dimps::GameEvents::RootEvent::Locate(peRoot);
}