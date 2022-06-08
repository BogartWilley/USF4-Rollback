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
void (*fKey::SizeLogger)(rKey* k, int oldSize);

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
    int oldSize = _this->sizeAllocated;
    (_this->*rKey::publicMethods.Initialize)(mementoable, numMementos);
    if (oldSize != 0 && oldSize != _this->sizeAllocated && SizeLogger != NULL) {
        SizeLogger(_this, oldSize);
    }
    trackedKeys.insert(_this);
}
