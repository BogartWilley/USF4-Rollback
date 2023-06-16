#include <set>

#include <windows.h>
#include <detours/detours.h>
#include "spdlog/spdlog.h"

#include "../Dimps/Dimps__Game.hxx"
#include "sf4e__Game.hxx"
#include "sf4e__Game__Battle.hxx"

namespace rGame = Dimps::Game;
using rKey = rGame::GameMementoKey;

namespace fGame = sf4e::Game;
using fKey = fGame::GameMementoKey;

std::set<rKey*> fKey::trackedKeys;

void fGame::Install() {
    Battle::Install();
    GameMementoKey::Install();
}

void fKey::Install() {
    void (fKey::* _fInitialize)(void*, int) = &Initialize;
    void (fKey:: * _fClearKey)() = &ClearKey;
    DetourAttach((PVOID*)&rKey::publicMethods.Initialize, *(PVOID*)&_fInitialize);
    DetourAttach((PVOID*)&rKey::publicMethods.ClearKey, *(PVOID*)&_fClearKey);
}

void fKey::Initialize(void* mementoable, int numMementos) {
    (this->*rKey::publicMethods.Initialize)(mementoable, numMementos);
    trackedKeys.insert(this);
}

void fKey::ClearKey() {
    (this->*rKey::publicMethods.ClearKey)();
    trackedKeys.erase((rKey*)this);
}
