#include <windows.h>

#include "Dimps__Game__Battle__Command.hxx"
#include "Dimps__Game__Battle__Command__CommandImpl.hxx"
#include "Dimps__Game__Battle__Command__Unit.hxx"

void Dimps::Game::Battle::Command::Locate(HMODULE peRoot) {
	Dimps::Game::Battle::Command::CommandImpl::Locate(peRoot);
	Dimps::Game::Battle::Command::Unit::Locate(peRoot);
}