#include <windows.h>

#include "Dimps__Game__Battle.hxx"
#include "Dimps__Game__Battle__Action.hxx"
#include "Dimps__Game__Battle__Camera.hxx"
#include "Dimps__Game__Battle__Chara.hxx"
#include "Dimps__Game__Battle__Command.hxx"
#include "Dimps__Game__Battle__GameManager.hxx"
#include "Dimps__Game__Battle__Effect.hxx"
#include "Dimps__Game__Battle__Hud.hxx"
#include "Dimps__Game__Battle__System.hxx"
#include "Dimps__Game__Battle__Training.hxx"
#include "Dimps__Game__Battle__Vfx.hxx"

namespace Battle = Dimps::Game::Battle;

Battle::IUnit::__publicMethods Battle::IUnit::publicMethods;

void Battle::Locate(HMODULE peRoot) {
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

void Battle::IUnit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.SharedHudUpdate = (PVOID)(peRootOffset + 0x18ae90);
}