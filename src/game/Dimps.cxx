#include <windows.h>

#include "Dimps.hxx"
#include "Dimps__Eva.hxx"
#include "Dimps__Event.hxx"
#include "Dimps__Game.hxx"
#include "Dimps__GameEvents.hxx"
#include "Dimps__Pad.hxx"
#include "Dimps__Platform.hxx"
#include "Dimps__UserApp.hxx"

char** Dimps::characterCodes;
char** Dimps::characterNames;
char** Dimps::stageCodes;
char** Dimps::stageNames;

void Dimps::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	characterCodes = (char**)(peRootOffset + 0x66a8a8);
	characterNames = (char**)(peRootOffset + 0x66a958);
	stageCodes = (char**)(peRootOffset + 0x66b678);
	stageNames = (char**)(peRootOffset + 0x66b600);

	Dimps::Eva::Locate(peRoot);
	Dimps::Event::Locate(peRoot);
	Dimps::Game::Locate(peRoot);
	Dimps::GameEvents::Locate(peRoot);
	Dimps::Pad::Locate(peRoot);
	Dimps::Platform::Locate(peRoot);
	Dimps::UserApp::Locate(peRoot);
}