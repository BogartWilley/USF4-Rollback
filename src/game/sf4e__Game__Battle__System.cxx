#include <windows.h>
#include <detours/detours.h>

#include "Dimps__Game.hxx"
#include "Dimps__Game__Battle__Camera.hxx"
#include "Dimps__Game__Battle__Chara__Actor.hxx"
#include "Dimps__Game__Battle__Chara__Unit.hxx"
#include "Dimps__Game__Battle__Command__Unit.hxx"
#include "Dimps__Game__Battle__Effect.hxx"
#include "Dimps__Game__Battle__GameManager.hxx"
#include "Dimps__Game__Battle__Hud.hxx"
#include "Dimps__Game__Battle__System.hxx"
#include "Dimps__Game__Battle__Training.hxx"
#include "Dimps__Game__Battle__Vfx.hxx"

#include "sf4e__Game__Battle__System.hxx"
#include "sf4e__Pad.hxx"

using CameraUnit = Dimps::Game::Battle::Camera::Unit;
using CharaActor = Dimps::Game::Battle::Chara::Actor;
using CharaUnit = Dimps::Game::Battle::Chara::Unit;
using CommandUnit = Dimps::Game::Battle::Command::Unit;
using EffectUnit = Dimps::Game::Battle::Effect::Unit;
using HudUnit = Dimps::Game::Battle::Hud::Unit;
using rSystem = Dimps::Game::Battle::System;
using TrainingManager = Dimps::Game::Battle::Training::Manager;
using VfxUnit = Dimps::Game::Battle::Vfx::Unit;
using Dimps::Game::GameMementoKey;

using fPadSystem = sf4e::Pad::System;

using fSystem = sf4e::Game::Battle::System;
bool fSystem::bHaltAfterNext = false;
bool fSystem::bUpdateAllowed = true;
int fSystem::nExtraFramesToSimulate = 0;

GameMementoKey::MementoID fSystem::loadRequest = { 0xffffffff, 0xffffffff };
GameMementoKey::MementoID fSystem::saveRequest = { 0xffffffff, 0xffffffff };

void fSystem::Install() {
    void (fSystem:: * _fBattleUpdate)() = &BattleUpdate;
    void (fSystem:: * _fSysMain_HandleTrainingModeFeatures)() = &SysMain_HandleTrainingModeFeatures;
    DetourAttach((PVOID*)&rSystem::publicMethods.BattleUpdate, *(PVOID*)&_fBattleUpdate);
    DetourAttach((PVOID*)&rSystem::publicMethods.SysMain_HandleTrainingModeFeatures, *(PVOID*)&_fSysMain_HandleTrainingModeFeatures);
}

void fSystem::BattleUpdate() {
    rSystem* _this = (rSystem*)this;

    if (bUpdateAllowed) {
        if (nExtraFramesToSimulate > 0) {
            for (int i = 0; i < nExtraFramesToSimulate; i++) {
                fPadSystem::playbackFrame = i;
                (_this->*rSystem::publicMethods.BattleUpdate)();
            }
            fPadSystem::playbackFrame = -1;
            nExtraFramesToSimulate = 0;
        }

        (_this->*rSystem::publicMethods.BattleUpdate)();

        if (bHaltAfterNext) {
            bHaltAfterNext = false;
            bUpdateAllowed = false;
        }
    }
}

void fSystem::SysMain_HandleTrainingModeFeatures() {
    rSystem* _this = (rSystem*)this;
    void* (rSystem:: * GetUnitByIndex)(unsigned int) = rSystem::publicMethods.GetUnitByIndex;
    CharaUnit* charaUnit = (CharaUnit*)(_this->*GetUnitByIndex)(rSystem::U_CHARA);

    if (loadRequest.lo != -1 && loadRequest.hi != -1) {
        fSystem::RestoreAllFromInternalMementos(_this, &loadRequest);
        loadRequest.lo = -1;
        loadRequest.hi = -1;
    }

    if (saveRequest.lo != -1 && saveRequest.hi != -1) {
        BOOL canSave = (charaUnit->*CharaUnit::publicMethods.CanStoreMemento_MaybeActorExists)();

        if (canSave) {
            fSystem::RecordAllToInternalMementos(_this, &saveRequest);
        }

        saveRequest.lo = -1;
        saveRequest.hi = -1;
    }


    (_this->*rSystem::publicMethods.SysMain_HandleTrainingModeFeatures)();
}


void fSystem::RestoreAllFromInternalMementos(rSystem* system, GameMementoKey::MementoID * id) {
    void* (rSystem:: * GetUnitByIndex)(unsigned int) = rSystem::publicMethods.GetUnitByIndex;
    CharaUnit* charaUnit = (CharaUnit*)(system->*GetUnitByIndex)(rSystem::U_CHARA);

    (system->*rSystem::publicMethods.RestoreFromInternalMementoKey)(id);
    (charaUnit->*CharaUnit::publicMethods.RestoreFromInternalMementoKey)(id);
    (
        ((EffectUnit*)(system->*GetUnitByIndex)(rSystem::U_EFFECT))->*
        EffectUnit::publicMethods.RestoreFromInternalMementoKey
        )(id);

    (
        ((VfxUnit*)(system->*GetUnitByIndex)(rSystem::U_VFX))->*
        VfxUnit::publicMethods.RestoreFromInternalMementoKey
        )(id);


    (
        ((CommandUnit*)(system->*GetUnitByIndex)(rSystem::U_COMMAND))->*
        CommandUnit::publicMethods.RestoreFromInternalMementoKey
        )(id);


    (
        ((HudUnit*)(system->*GetUnitByIndex)(rSystem::U_HUD))->*
        HudUnit::publicMethods.RestoreFromInternalMementoKey
        )(id);

    (
        ((CameraUnit*)(system->*GetUnitByIndex)(rSystem::U_CAMERA))->*
        CameraUnit::publicMethods.RestoreFromInternalMementoKey
        )(id);

    (
        TrainingManager::staticMethods.GetSingleton()->*
        TrainingManager::publicMethods.RestoreFromInternalMementoKey
        )(id);

    CharaActor::staticMethods.ResetAfterMemento((charaUnit->*CharaUnit::publicMethods.GetActorByIndex)(0));
    CharaActor::staticMethods.ResetAfterMemento((charaUnit->*CharaUnit::publicMethods.GetActorByIndex)(1));
}

void fSystem::RecordAllToInternalMementos(rSystem* system, GameMementoKey::MementoID* id) {
    void* (rSystem:: * GetUnitByIndex)(unsigned int) = rSystem::publicMethods.GetUnitByIndex;
    (system->*rSystem::publicMethods.RecordToInternalMementoKey)(id);

    (
        ((CharaUnit*)(system->*GetUnitByIndex)(rSystem::U_CHARA))->*
        CharaUnit::publicMethods.RecordToInternalMementoKey
        )(id);

    (
        ((EffectUnit*)(system->*GetUnitByIndex)(rSystem::U_EFFECT))->*
        EffectUnit::publicMethods.RecordToInternalMementoKey
        )(id);

    (
        ((VfxUnit*)(system->*GetUnitByIndex)(rSystem::U_VFX))->*
        VfxUnit::publicMethods.RecordToInternalMementoKey
        )(id);

    (
        ((CommandUnit*)(system->*GetUnitByIndex)(rSystem::U_COMMAND))->*
        CommandUnit::publicMethods.RecordToInternalMementoKey
        )(id);

    (
        ((HudUnit*)(system->*GetUnitByIndex)(rSystem::U_HUD))->*
        HudUnit::publicMethods.RecordToInternalMementoKey
        )(id);

    (
        ((CameraUnit*)(system->*GetUnitByIndex)(rSystem::U_CAMERA))->*
        CameraUnit::publicMethods.RecordToInternalMementoKey
        )(id);

    (
        TrainingManager::staticMethods.GetSingleton()->*
        TrainingManager::publicMethods.RecordToInternalMementoKey
        )(id);
}