#include <windows.h>

#include "Dimps__Game__Battle.hxx"
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

void Battle::Locate(HMODULE peRoot) {
	Camera::Locate(peRoot);
	Chara::Locate(peRoot);
	Command::Locate(peRoot);
	GameManager::Locate(peRoot);
	Effect::Locate(peRoot);
	Hud::Locate(peRoot);
	System::Locate(peRoot);
	Training::Locate(peRoot);
	Vfx::Locate(peRoot);
}