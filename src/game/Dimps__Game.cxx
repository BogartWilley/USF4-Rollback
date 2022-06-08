#include <windows.h>

#include "Dimps__Game.hxx"
#include "Dimps__Game__Battle.hxx"

namespace Game = Dimps::Game;
using Dimps::Game::GameMementoKey;

GameMementoKey::__publicMethods GameMementoKey::publicMethods;
int* GameMementoKey::totalMementoSize;

void Game::Locate(HMODULE peRoot) {
	Battle::Locate(peRoot);
	GameMementoKey::Locate(peRoot);
}

void GameMementoKey::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.Initialize = (PVOID)(peRootOffset + 0x12fd40);
	totalMementoSize = (int*)(peRootOffset + 0x6a5840);
}