#include <string>
#include <windows.h>

#include "Dimps__GameEvents.hxx"

namespace GameEvents = Dimps::GameEvents;
using Dimps::GameEvents::RootEvent;
using Dimps::GameEvents::VsCharaSelect;

char** RootEvent::eventFlowDefinition;
VsCharaSelect::__publicMethods VsCharaSelect::publicMethods;
VsCharaSelect::__staticMethods VsCharaSelect::staticMethods;

void GameEvents::Locate(HMODULE peRoot) {
	RootEvent::Locate(peRoot);
	VsCharaSelect::Locate(peRoot);
}

void RootEvent::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	eventFlowDefinition = (char**)(peRootOffset + 0x665958);
}

void VsCharaSelect::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.Destroy = (PVOID)(peRootOffset + 0x23d0e0);

	staticMethods.Factory = (VsCharaSelect * (*)(DWORD, DWORD, DWORD))(peRootOffset + 0x23d660);
}

VsCharaSelect::PlayerConditions* VsCharaSelect::GetPlayerConditions(VsCharaSelect* selectEvent) {
	return (PlayerConditions*)((unsigned int)selectEvent + 0x4c);
}

std::string* VsCharaSelect::PlayerConditions::GetSelectedCharaAbbrev(PlayerConditions* c) {
	return (std::string*)((unsigned int)c + 0x24);
}

std::string* VsCharaSelect::PlayerConditions::GetHoveredCharaAbbrev(PlayerConditions* c) {
	return (std::string*)((unsigned int)c + 0x40);
}

BYTE* VsCharaSelect::PlayerConditions::GetUltraCombo(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5c);
}

BYTE* VsCharaSelect::PlayerConditions::GetColor(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5d);
}

BYTE* VsCharaSelect::PlayerConditions::GetCostume(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5e);
}

BYTE* VsCharaSelect::PlayerConditions::GetPersonalAction(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5f);
}

BYTE* VsCharaSelect::PlayerConditions::GetWinQuote(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x60);
}

BYTE* VsCharaSelect::PlayerConditions::GetEdition(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x62);
}
