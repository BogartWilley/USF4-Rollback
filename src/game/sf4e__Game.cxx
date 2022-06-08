#include <set>

#include <windows.h>
#include <detours.h>

#include "Dimps__Game.hxx"
#include "sf4e__Game.hxx"
#include "sf4e__Game__Battle__System.hxx"

namespace rGame = Dimps::Game;
using rKey = rGame::GameMementoKey;

namespace fGame = sf4e::Game;
using fKey = fGame::GameMementoKey;

std::set<rKey*> fKey::trackedKeys;

void fGame::Install() {
    Battle::System::Install();
    GameMementoKey::Install();
}

void fKey::Install() {
    void (fKey::* _fInitialize)(void*, int) = &Initialize;
    DetourAttach((PVOID*)&rKey::publicMethods.Initialize, *(PVOID*)&_fInitialize);
}

void fKey::Initialize(void* mementoable, int numMementos) {
    rKey* _this = (rKey*)this;
    (_this->*rKey::publicMethods.Initialize)(mementoable, numMementos);
    trackedKeys.insert(_this);
}
