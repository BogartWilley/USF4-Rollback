#include <windows.h>
#include <detours/detours.h>

#include "spdlog/spdlog.h"

#include "../Dimps/Dimps__Game__Battle__Vfx.hxx"
#include "../Dimps/Dimps__Platform.hxx"
#include "sf4e__Game__Battle__Vfx.hxx"

using Dimps::Platform::list_entry;
using Dimps::Platform::list;

namespace fVfx = sf4e::Game::Battle::Vfx;
namespace rVfx = Dimps::Game::Battle::Vfx;

using fColorFade = fVfx::ColorFade;
using rColorFade = rVfx::ColorFade;

int fColorFade::HIGHEST_OBSERVED_FADES = 0;

void fVfx::Install() {
    fColorFade::Install();
}

void fColorFade::Install() {
    list_entry<rVfx::ColorFadeData>* (fColorFade::* _fSpawn)(void* sourceData) = &Spawn;
    DetourAttach((PVOID*)&rColorFade::publicMethods.Spawn, *(PVOID*)&_fSpawn);
}

list_entry<rVfx::ColorFadeData>* fColorFade::Spawn(void* sourceData) {
    rColorFade* _this = (rColorFade*)this;
    list_entry<rVfx::ColorFadeData>* out = (_this->*rColorFade::publicMethods.Spawn)(sourceData);
    list<rVfx::ColorFadeData>* fadeList = rColorFade::GetList(_this);
    if (fadeList->numUsed > HIGHEST_OBSERVED_FADES) {
        spdlog::info("ColorFade: new max list size updated from {} to {}", HIGHEST_OBSERVED_FADES, fadeList->numUsed);
        HIGHEST_OBSERVED_FADES = fadeList->numUsed;
    }
    return out;
}
