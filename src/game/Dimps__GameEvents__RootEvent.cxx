#include <windows.h>

#include "Dimps__GameEvents__RootEvent.hxx"

using Dimps::GameEvents::RootEvent;

char** RootEvent::eventFlowDefinition;

void RootEvent::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	eventFlowDefinition = (char**)(peRootOffset + 0x665958);
}