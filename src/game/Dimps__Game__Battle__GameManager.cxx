#include <windows.h>

#include "Dimps__Game__Battle__GameManager.hxx"

using Dimps::Game::Battle::GameManager;

GameManager::__privateMethods GameManager::privateMethods;
GameManager::__publicMethods GameManager::publicMethods;
GameManager::__staticMethods GameManager::staticMethods;

void GameManager::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)&publicMethods.GetRoundTime = (PVOID)(peRootOffset + 0x1d14d0);
}