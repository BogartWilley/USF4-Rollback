#include <windows.h>

#include "Dimps__GameEvents.hxx"
#include "Dimps__Platform.hxx"

namespace GameEvents = Dimps::GameEvents;
using Dimps::GameEvents::RootEvent;
using Dimps::GameEvents::VsCharaSelect;
using Dimps::Platform::dString;

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

VsCharaSelect::CharaSelectState* VsCharaSelect::GetState(VsCharaSelect* selectEvent) {
	return (CharaSelectState*)((unsigned int)selectEvent + 0x4c);
}

dString* VsCharaSelect::PlayerConditions::GetSelectedCharaAbbrev(PlayerConditions* c) {
	return (dString*)((unsigned int)c + 0x20);
}

dString* VsCharaSelect::PlayerConditions::GetHoveredCharaAbbrev(PlayerConditions* c) {
	return (dString*)((unsigned int)c + 0x3c);
}

BYTE* VsCharaSelect::PlayerConditions::GetUltraCombo(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x58);
}

BYTE* VsCharaSelect::PlayerConditions::GetColor(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x59);
}

BYTE* VsCharaSelect::PlayerConditions::GetCostume(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5a);
}

BYTE* VsCharaSelect::PlayerConditions::GetPersonalAction(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5b);
}

BYTE* VsCharaSelect::PlayerConditions::GetWinQuote(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5c);
}

BYTE* VsCharaSelect::PlayerConditions::GetHandicap(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5d);
}

BYTE* VsCharaSelect::PlayerConditions::GetEdition(PlayerConditions* c) {
	return (BYTE*)((unsigned int)c + 0x5e);
}
