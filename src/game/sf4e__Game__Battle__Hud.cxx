#include <windows.h>
#include <detours/detours.h>

#include "spdlog/spdlog.h"

#include "Dimps__Eva.hxx"
#include "Dimps__Game__Battle.hxx"
#include "Dimps__Game__Battle__Hud.hxx"
#include "sf4e__Game__Battle.hxx"
#include "sf4e__Game__Battle__Hud.hxx"

namespace fHud = sf4e::Game::Battle::Hud;
namespace rHud = Dimps::Game::Battle::Hud;

using Dimps::Eva::Task;

using fIUnit = sf4e::Game::Battle::IUnit;
using rIUnit = Dimps::Game::Battle::IUnit;

bool fHud::bAllowHudUpdate = true;

void fHud::Install() {
    Announce::Unit::Install();
    Cockpit::Unit::Install();
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
