#include <windows.h>
#include <detours/detours.h>

#include "spdlog/spdlog.h"

#include "../Dimps/Dimps__Game__Battle__Action.hxx"
#include "sf4e__Game__Battle__Action.hxx"

namespace fAction = sf4e::Game::Battle::Action;
namespace rAction = Dimps::Game::Battle::Action;
using fActor = fAction::Actor;
using rActor = rAction::Actor;

void fAction::Install() {
    fActor::Install();
}

void fActor::Install() {
    size_t (fActor:: * _fGetMementoSize)() = &GetMementoSize;
    DetourAttach((PVOID*)&rActor::publicMethods.GetMementoSize, *(PVOID*)&_fGetMementoSize);
}

size_t fActor::GetMementoSize() {
    rActor* _this = (rActor*)this;
    int expectedMaximum = (0x240 + 5 * (0x110 * 0x40));
    int observed = (_this->*rActor::publicMethods.GetMementoSize)();
    if (observed > expectedMaximum) {
        spdlog::warn("Action::Actor memento undersized! Expected {}, got {}", expectedMaximum, observed);
        return observed;
    }
    return expectedMaximum;
}
