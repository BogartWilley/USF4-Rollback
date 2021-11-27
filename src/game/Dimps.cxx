#include <windows.h>

#include "Dimps.hxx"
#include "Dimps__Game.hxx"
#include "Dimps__Platform.hxx"

void Dimps::Locate(HMODULE peRoot) {
	Dimps::Game::Locate(peRoot);
	Dimps::Platform::Locate(peRoot);
}