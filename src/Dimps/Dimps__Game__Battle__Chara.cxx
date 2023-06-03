#include <windows.h>

#include "Dimps__Game__Battle__Chara.hxx"
#include "Dimps__Game__Battle__Chara__Actor.hxx"
#include "Dimps__Game__Battle__Chara__Unit.hxx"

void Dimps::Game::Battle::Chara::Locate(HMODULE peRoot) {
	Dimps::Game::Battle::Chara::Actor::Locate(peRoot);
	Dimps::Game::Battle::Chara::Unit::Locate(peRoot);
}