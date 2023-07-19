#include <windows.h>
#include <detours/detours.h>

#include "spdlog/spdlog.h"

#include "../Dimps/Dimps__Eva.hxx"
#include "../Dimps/Dimps__Game.hxx"
#include "../Dimps/Dimps__Game__Battle.hxx"
#include "../Dimps/Dimps__Game__Battle__Hud.hxx"
#include "../Dimps/Dimps__Platform.hxx"
#include "sf4e__Game__Battle.hxx"
#include "sf4e__Game__Battle__Hud.hxx"

namespace fHud = sf4e::Game::Battle::Hud;
namespace rHud = Dimps::Game::Battle::Hud;

using Dimps::Eva::Task;
using Dimps::Game::GameMementoKey;
using Dimps::Platform::UNK_ScaleformRelated;

using fIUnit = sf4e::Game::Battle::IUnit;
using rIUnit = Dimps::Game::Battle::IUnit;

bool fHud::bAllowHudUpdate = true;

void fHud::Install() {
    Unit::Install();
    Announce::Unit::Install();
    Cockpit::Unit::Install();
    Cockpit::View::Install();
    Cursor::Unit::Install();
    Result::Unit::Install();
    Subtitle::Unit::Install();
    Training::Unit::Install();

    void (fIUnit:: * _fSharedHudUpdate)(Task** task) = &fIUnit::SharedHudUpdate;

    DetourAttach(
        (PVOID*)&rIUnit::publicMethods.SharedHudUpdate,
        *(PVOID*)&_fSharedHudUpdate
    );
}

void fHud::Unit::Install() {
    DetourAttach((PVOID*)&rHud::Unit::staticMethods.Factory, Factory);
}


rHud::Unit* fHud::Unit::Factory(DWORD arg1, DWORD arg2) {
    rHud::Unit* u = rHud::Unit::staticMethods.Factory(arg1, arg2);
    // SF4's factory doesn't initialize the memory allocated to the
    // HUD unit to zero, resulting in the possibility of pointers
    // in the unit being non-zero. Null checks against those pointers
    // evaluate non-null, but don't actually point at real memory.
    //
    // This is particularly notable in the case of mementos, where
    // a null check on the training unit is executed before attempting
    // to save it. If the memory is not initialized, attempting to
    // access the training unit's vtable will segfault.
    rHud::Unit::ResetUnits(u);
    return u;
}

void fHud::Announce::Unit::Install() {
    void (Unit:: * _fHudAnnounce_Update)(Task** task) = &HudAnnounce_Update;
    DetourAttach(
        (PVOID*)&rHud::Announce::Unit::publicMethods.HudAnnounce_Update,
        *(PVOID*)&_fHudAnnounce_Update
    );
}

void fHud::Announce::Unit::HudAnnounce_Update(Task** task) {
    if (bAllowHudUpdate) {
        (this->*rHud::Announce::Unit::publicMethods.HudAnnounce_Update)(task);
    }
}

void fHud::Cockpit::Unit::Install() {
    void (Unit:: * _fHudCockpit_Update)(Task** task) = &HudCockpit_Update;
    DetourAttach(
        (PVOID*)&rHud::Cockpit::Unit::publicMethods.HudCockpit_Update,
        *(PVOID*)&_fHudCockpit_Update
    );
}

void fHud::Cockpit::Unit::HudCockpit_Update(Task** task) {
    if (bAllowHudUpdate) {
        (this->*rHud::Cockpit::Unit::publicMethods.HudCockpit_Update)(task);
    }
}

void fHud::Cockpit::View::Install() {
    void (View::* _fRestoreFromInternalMementoKey)(GameMementoKey::MementoID* id) = &RestoreFromInternalMementoKey;
    DetourAttach(
        (PVOID*)&rHud::Cockpit::View::publicMethods.RestoreFromInternalMementoKey,
        *(PVOID*)&_fRestoreFromInternalMementoKey
    );
}

void fHud::Cockpit::View::RestoreFromInternalMementoKey(GameMementoKey::MementoID* id) {
    (this->*rHud::Cockpit::View::publicMethods.RestoreFromInternalMementoKey)(id);

    // SF4's implementation of the cockpit view's load operation is incomplete.
    //
    // When the cockpit view updates, it first delegates updates to its children,
    // then forcibly adjusts some of the children to take into account animations
    // like frame-out as well as to respect the user's HUD position settings.
    // The super-combo meter (Dimps::Game::Battle::Hud::Cockpit::Sc) stages
    // its child sprite nodes on update as necessary.
    // 
    // However, loading is a different story. When loading, the super-combo
    // meter will re-stage its child sprite nodes, but the View does _not_
    // adjust the position of the new sprite nodes. This isn't generally
    // user visible because when the save states are used in training mode,
    // the save and load operations occur very early in control flow, and
    // then the HUD update later in the control flow repositions the nodes
    // before rendering. Unfortunately, since our savestate-freeing method
    // requires loading the state, `ggpo_advance_frame()` will load state
    // _after_ the HUD update and _before_ rendering, which results in
    // sprites visibly out-of-place.
    // 
    // Work around this by updating the position of the child controls,
    // while taking care to not animate the sprites. It may be necessary
    // to skip updating the child controls as well, but specific controls
    // will need to be disabled or reenabled to prevent all the child
    // controls from being turned on at once (which could lead to scenarios
    // like win marks appearing in training mode).
    // 
    // One other possible workaround: never call `ggpo_advance_frame()` until
    // after rendering.
    //
    // If savestates ever move away from using SF4's internal memento keys,
    // this will need to be implemented a different way, although it may go
    // away entirely depending on the implementation of the load operation.
    UNK_ScaleformRelated* scaleform = UNK_ScaleformRelated::staticMethods.GetSingleton();
    float originalFrameCount = (scaleform->*UNK_ScaleformRelated::publicMethods.GetNumFramesToSim)();
    (scaleform->*UNK_ScaleformRelated::publicMethods.SetNumFramesToSim)(0.0);
    (this->*rHud::Cockpit::View::publicMethods.Update)();
    (scaleform->*UNK_ScaleformRelated::publicMethods.SetNumFramesToSim)(originalFrameCount);
}

void fHud::Cursor::Unit::Install() {
    void (Unit:: * _fHudCursor_Update)(Task** task) = &HudCursor_Update;
    DetourAttach(
        (PVOID*)&rHud::Cursor::Unit::publicMethods.HudCursor_Update,
        *(PVOID*)&_fHudCursor_Update
    );
}

void fHud::Cursor::Unit::HudCursor_Update(Task** task) {
    if (bAllowHudUpdate) {
        (this->*rHud::Cursor::Unit::publicMethods.HudCursor_Update)(task);
    }
}

void fHud::Result::Unit::Install() {
    void (Unit:: * _fHudResult_Update)(Task** task) = &HudResult_Update;
    DetourAttach(
        (PVOID*)&rHud::Result::Unit::publicMethods.HudResult_Update,
        *(PVOID*)&_fHudResult_Update
    );
}

void fHud::Result::Unit::HudResult_Update(Task** task) {
    if (bAllowHudUpdate) {
        (this->*rHud::Result::Unit::publicMethods.HudResult_Update)(task);
    }
}

void fHud::Subtitle::Unit::Install() {
    void (Unit:: * _fHudSubtitle_Update)(Task** task) = &HudSubtitle_Update;
    DetourAttach(
        (PVOID*)&rHud::Subtitle::Unit::publicMethods.HudSubtitle_Update,
        *(PVOID*)&_fHudSubtitle_Update
    );
}

void fHud::Subtitle::Unit::HudSubtitle_Update(Task** task) {
    if (bAllowHudUpdate) {
        (this->*rHud::Subtitle::Unit::publicMethods.HudSubtitle_Update)(task);
    }
}

void fHud::Training::Unit::Install() {
    void (Unit:: * _fHudTraining_Update)(Task** task) = &HudTraining_Update;
    DetourAttach(
        (PVOID*)&rHud::Training::Unit::publicMethods.HudTraining_Update,
        *(PVOID*)&_fHudTraining_Update
    );
}

void fHud::Training::Unit::HudTraining_Update(Task** task) {
    if (bAllowHudUpdate) {
        (this->*rHud::Training::Unit::publicMethods.HudTraining_Update)(task);
    }
}
