#include <windows.h>

#include "Dimps__GameEvents.hxx"
#include "Dimps__Platform.hxx"

namespace GameEvents = Dimps::GameEvents;
namespace StageSelect = Dimps::GameEvents::StageSelect;

using Dimps::GameEvents::MainMenu;
using Dimps::GameEvents::RootEvent;
using Dimps::GameEvents::VsCharaSelect;
using Dimps::GameEvents::VsStageSelect;
using Dimps::Platform::dString;

MainMenu::__publicMethods MainMenu::publicMethods;
MainMenu::__staticMethods MainMenu::staticMethods;
char** RootEvent::eventFlowDefinition;
VsCharaSelect::__publicMethods VsCharaSelect::publicMethods;
VsCharaSelect::__staticMethods VsCharaSelect::staticMethods;
VsStageSelect::__publicMethods VsStageSelect::publicMethods;
VsStageSelect::__staticMethods VsStageSelect::staticMethods;
StageSelect::Control::__publicMethods StageSelect::Control::publicMethods;

void GameEvents::Locate(HMODULE peRoot) {
	MainMenu::Locate(peRoot);
	RootEvent::Locate(peRoot);
	StageSelect::Locate(peRoot);
	VsCharaSelect::Locate(peRoot);
	VsStageSelect::Locate(peRoot);
}

void StageSelect::Locate(HMODULE peRoot) {
	Control::Locate(peRoot);
}

void MainMenu::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.Destroy = (PVOID)(peRootOffset + 0x212f90);
	staticMethods.Factory = (MainMenu*(*)(DWORD, DWORD, DWORD))(peRootOffset + 0x212f30);
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

void VsStageSelect::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.Destroy = (PVOID)(peRootOffset + 0x23e5d0);
	staticMethods.Factory = (VsStageSelect * (*)(DWORD, DWORD, DWORD))(peRootOffset + 0x23e870);
}

VsStageSelect::StageSelectState* VsStageSelect::GetState(VsStageSelect* selectEvent) {
	return (StageSelectState*)((unsigned int)selectEvent + 0x48);
}

StageSelect::Control* VsStageSelect::GetControl(VsStageSelect* selectEvent) {
	return *(StageSelect::Control**)((unsigned int)selectEvent + 0x44);
}

void StageSelect::Control::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.GetPhase = (PVOID)(peRootOffset + 0x0d22c0);
}