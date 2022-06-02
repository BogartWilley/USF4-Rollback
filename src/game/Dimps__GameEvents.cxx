#include <windows.h>

#include "Dimps__GameEvents.hxx"

namespace GameEvents = Dimps::GameEvents;
using Dimps::GameEvents::RootEvent;

char** RootEvent::eventFlowDefinition;

void GameEvents::Locate(HMODULE peRoot) {
	RootEvent::Locate(peRoot);
}

void RootEvent::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	eventFlowDefinition = (char**)(peRootOffset + 0x665958);
}