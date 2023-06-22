#include <windows.h>

#include "Dimps__Game.hxx"
#include "Dimps__Game__Battle.hxx"

namespace Game = Dimps::Game;
using Game::GameMementoKey;
using Game::Request;

GameMementoKey::__publicMethods GameMementoKey::publicMethods;
int* GameMementoKey::totalMementoSize;
Request::__publicMethods Request::publicMethods;

void Game::Locate(HMODULE peRoot) {
	Battle::Locate(peRoot);
	GameMementoKey::Locate(peRoot);
	Request::Locate(peRoot);
}

void GameMementoKey::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.Initialize = (PVOID)(peRootOffset + 0x12fd40);
	*(PVOID*)&publicMethods.ClearKey = (PVOID)(peRootOffset + 0x12f3d0);
	totalMementoSize = (int*)(peRootOffset + 0x6a5840);
}

void Request::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.GetRandomSeed = (PVOID)(peRootOffset + 0x285d70);
	*(PVOID*)&publicMethods.SetIsOnlineBattle = (PVOID)(peRootOffset + 0x284620);
	*(PVOID*)&publicMethods.SetRandomSeed = (PVOID)(peRootOffset + 0x285300);
}