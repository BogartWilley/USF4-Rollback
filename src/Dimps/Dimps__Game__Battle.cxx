#include <windows.h>

#include "Dimps__Eva.hxx"
#include "Dimps__Game__Battle.hxx"
#include "Dimps__Game__Battle__Action.hxx"
#include "Dimps__Game__Battle__Camera.hxx"
#include "Dimps__Game__Battle__Chara.hxx"
#include "Dimps__Game__Battle__Command.hxx"
#include "Dimps__Game__Battle__Effect.hxx"
#include "Dimps__Game__Battle__Hud.hxx"
#include "Dimps__Game__Battle__System.hxx"
#include "Dimps__Game__Battle__Training.hxx"
#include "Dimps__Game__Battle__Vfx.hxx"

namespace Battle = Dimps::Game::Battle;

using Dimps::Eva::Task;
using Battle::GameManager;
using Battle::IUnit;

GameManager::__publicMethods GameManager::publicMethods;
IUnit::__publicMethods IUnit::publicMethods;

int Battle::orderedEditions[NUM_VALID_EDITIONS] = {
	Battle::ED_SF4,
	Battle::ED_SSF4,
	Battle::ED_AE2011,
	// The index-3 edition is disabled on every character.
	// It looks like it corresponds to regulation 3 which
	// might be AE2012 1.0, alluding to edition select
	// possibly originally supporting subversions.
	0,
	Battle::ED_AE2012,
	Battle::ED_USF4,
	Battle::ED_OMEGA,
};

Battle::ValidEditions* Battle::validEditionsPerChara;
void Battle::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	validEditionsPerChara = (ValidEditions*)(peRootOffset + 0x539ba8);

	Action::Locate(peRoot);
	Camera::Locate(peRoot);
	Chara::Locate(peRoot);
	Command::Locate(peRoot);
	GameManager::Locate(peRoot);
	Effect::Locate(peRoot);
	Hud::Locate(peRoot);
	IUnit::Locate(peRoot);
	System::Locate(peRoot);
	Training::Locate(peRoot);
	Vfx::Locate(peRoot);
}

void IUnit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.SharedHudUpdate = (PVOID)(peRootOffset + 0x18ae90);
}

void GameManager::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)&publicMethods.GetRoundTime = (PVOID)(peRootOffset + 0x1d14d0);
}

Task** Battle::Pause::Unit::GetPauseTask(Unit* u) {
	return (Task**)((unsigned int)u + 0x14);
}
