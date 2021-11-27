#include <windows.h>

#include "Dimps__Game.hxx"
#include "Dimps__Game__Battle.hxx"

void Dimps::Game::Locate(HMODULE peRoot) {
	Dimps::Game::Battle::Locate(peRoot);
}