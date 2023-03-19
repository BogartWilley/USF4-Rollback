#include <set>

#include <windows.h>
#include <detours/detours.h>

#include "spdlog/spdlog.h"

#include "Dimps__Game.hxx"
#include "sf4e__Game.hxx"
#include "sf4e__Game__Battle.hxx"

namespace rGame = Dimps::Game;
using rKey = rGame::GameMementoKey;

namespace fGame = sf4e::Game;
using fKey = fGame::GameMementoKey;

bool fKey::bEnableUnsafeReinitializationSkip = false;
std::set<rKey*> fKey::trackedKeys;
void (*fKey::SizeLogger)(rKey* k, int oldSize);

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
    rKey* _this = (rKey*)this;
    int oldSize = _this->sizeAllocated;
    bool shouldInit;
    if (bEnableUnsafeReinitializationSkip) {
        shouldInit = (
            _this->metadata == NULL ||
            _this->mementoableObject != mementoable ||
            _this->numMementos != numMementos
        );
    }
    else {
        shouldInit = true;
    }

    if (!shouldInit) {
        rKey tempKey = {};
        (tempKey.*rKey::publicMethods.Initialize)(mementoable, numMementos);
        if (_this->sizeAllocated != tempKey.sizeAllocated) {
            spdlog::warn(
                "UNSAFE MEMENTO REINIT: Memento w/ mementoable {} (vtbl {}) had differing size: Existing {:d}, clean {:d}",
                _this->mementoableObject,
                *(void**)_this->mementoableObject,
                _this->sizeAllocated,
                tempKey.sizeAllocated
            );
            if (SizeLogger != NULL) {
                SizeLogger(&tempKey, _this->sizeAllocated);
            }
        }
        (tempKey.*rKey::publicMethods.ClearKey)();
        return;
    }

    (_this->*rKey::publicMethods.Initialize)(mementoable, numMementos);
    if (oldSize != 0 && oldSize != _this->sizeAllocated) {
        spdlog::info(
            "GameMementoKey @ {} w/ mementoable {} (vtbl {}) had differing size: Old {:d}, new {:d}",
            (void*)_this,
            _this->mementoableObject,
            *(void**)_this->mementoableObject,
            oldSize,
            _this->sizeAllocated
        );
        if (SizeLogger != NULL) {
            SizeLogger(_this, oldSize);
        }
    }
    trackedKeys.insert(_this);
}

void fKey::ClearKey() {
    (this->*rKey::publicMethods.ClearKey)();
    trackedKeys.erase((rKey*)this);
}
