#include <windows.h>

#include "Dimps__Game__Battle.hxx"
#include "Dimps__Game__Battle__Chara.hxx"
#include "Dimps__Game__Battle__System.hxx"

void Dimps::Game::Battle::Locate(HMODULE peRoot) {
	Dimps::Game::Battle::Chara::Locate(peRoot);
	Dimps::Game::Battle::System::Locate(peRoot);
}