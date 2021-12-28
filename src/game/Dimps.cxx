#include <windows.h>

#include "Dimps.hxx"
#include "Dimps__Game.hxx"
#include "Dimps__Platform.hxx"

char** Dimps::characterCodes;

void Dimps::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	characterCodes = (char**)(peRootOffset + 0x66a8a8);

	Dimps::Game::Locate(peRoot);
	Dimps::Platform::Locate(peRoot);
}