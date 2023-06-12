#include <utility>

#include <windows.h>
#include <detours/detours.h>
#include <GameNetworkingSockets/steam/steamnetworkingsockets.h>
#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include <ggponet.h>
#include <spdlog/spdlog.h>

#include "../Dimps/Dimps__Game.hxx"
#include "../Dimps/Dimps__Game__Battle.hxx"
#include "../Dimps/Dimps__Game__Battle__Camera.hxx"
#include "../Dimps/Dimps__Game__Battle__Chara.hxx"
#include "../Dimps/Dimps__Game__Battle__Command.hxx"
#include "../Dimps/Dimps__Game__Battle__Effect.hxx"
#include "../Dimps/Dimps__Game__Battle__Hud.hxx"
#include "../Dimps/Dimps__Game__Battle__System.hxx"
#include "../Dimps/Dimps__Game__Battle__Training.hxx"
#include "../Dimps/Dimps__Game__Battle__Vfx.hxx"
#include "../Dimps/Dimps__Pad.hxx"

#include "sf4e__Game.hxx"
#include "sf4e__GameEvents.hxx"
#include "sf4e__Game__Battle.hxx"
#include "sf4e__Game__Battle__Hud.hxx"
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
using rKey = Dimps::Game::GameMementoKey;
using fKey = sf4e::Game::GameMementoKey;
using rPadSystem = Dimps::Pad::System;
using fPadSystem = sf4e::Pad::System;

using fSystem = sf4e::Game::Battle::System;
using fVsBattle = sf4e::GameEvents::VsBattle;

bool fSystem::bHaltAfterNext = false;
bool fSystem::bUpdateAllowed = true;
int fSystem::nExtraFramesToSimulate = 0;
int fSystem::nNextBattleStartFlowTarget = -1;

GGPOSession* fSystem::ggpo;
GGPOPlayer fSystem::players[2];
GGPOPlayerHandle playerHandles[2];
fSystem::SaveState fSystem::saveStates[10];
rKey::MementoID GGPO_MEMENTO_ID = { 1, 1 };

GameMementoKey::MementoID fSystem::loadRequest = { 0xffffffff, 0xffffffff };
GameMementoKey::MementoID fSystem::saveRequest = { 0xffffffff, 0xffffffff };

void fSystem::Install() {
    void (fSystem:: * _fBattleUpdate)() = &BattleUpdate;
    void (fSystem:: * _fSysMain_HandleTrainingModeFeatures)() = &SysMain_HandleTrainingModeFeatures;
    DetourAttach((PVOID*)&rSystem::publicMethods.BattleUpdate, *(PVOID*)&_fBattleUpdate);
    DetourAttach((PVOID*)&rSystem::publicMethods.SysMain_HandleTrainingModeFeatures, *(PVOID*)&_fSysMain_HandleTrainingModeFeatures);
    DetourAttach((PVOID*)&rSystem::staticMethods.OnBattleFlow_BattleStart, OnBattleFlow_BattleStart);
}

void fSystem::BattleUpdate() {
    rSystem* _this = (rSystem*)this;
    rSystem::__publicMethods& sysMethods = rSystem::publicMethods;
    rPadSystem* p = rPadSystem::staticMethods.GetSingleton();
    rPadSystem::__publicMethods& padMethods = rPadSystem::publicMethods;

    if (!bUpdateAllowed) {
        return;
    }

    if (ggpo && *rSystem::staticVars.CurrentBattleFlow != BF__IDLE) {
        for (int i = 0; i < 2; i++) {
            if (players[i].type == GGPO_PLAYERTYPE_LOCAL) {
                DWORD inputs = (p->*padMethods.GetButtons_Mapped)(i);
                GGPOErrorCode result = ggpo_add_local_input(ggpo, playerHandles[i], &inputs, sizeof(DWORD));
                if (GGPO_SUCCEEDED(result)) {
                    DWORD ggpoInputs[2] = { 0 };
                    int disconnect_flags = 0;
                    result = ggpo_synchronize_input(ggpo, (void*)ggpoInputs, sizeof(DWORD) * 2, &disconnect_flags);
                    if (GGPO_SUCCEEDED(result)) {
                        fPadSystem::playbackFrame = 0;
                        fPadSystem::playbackData[0][0] = ggpoInputs[0];
                        fPadSystem::playbackData[1][0] = ggpoInputs[1];
                        (_this->*sysMethods.BattleUpdate)();
                        fPadSystem::playbackFrame = -1;
                        GGPOErrorCode err = ggpo_advance_frame(ggpo);
                        if (!GGPO_SUCCEEDED(err)) {
                            MessageBoxA(NULL, "sf4e system could not advance frame after normal sim! Will likely crash!", NULL, MB_OK);
                        }
                    }
                }
            }
        }
    }
    else {
        (_this->*rSystem::publicMethods.BattleUpdate)();
    }

    if (nExtraFramesToSimulate > 0) {
        for (int i = 0; i < nExtraFramesToSimulate; i++) {
            fPadSystem::playbackFrame = i;
            (_this->*sysMethods.BattleUpdate)();
        }
        fPadSystem::playbackFrame = -1;
        nExtraFramesToSimulate = 0;
    }

    if (bHaltAfterNext) {
        bHaltAfterNext = false;
        bUpdateAllowed = false;
    }
}

void fSystem::OnBattleFlow_BattleStart(System* s) {
    if (nNextBattleStartFlowTarget > -1) {
        rSystem::staticMethods.SetBattleFlow(s, nNextBattleStartFlowTarget);
        nNextBattleStartFlowTarget = -1;
        return;
    }

    return rSystem::staticMethods.OnBattleFlow_BattleStart(s);
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


void fSystem::RestoreAllFromInternalMementos(rSystem* system, rKey::MementoID * id) {
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

    // Intentionally omit the reset of the Network unit. All in-game inputs
    // are passed into and read back out of the network unit, regardless
    // of whether or not the match is local or network. The network unit's
    // reset is used to zero the inputs of the first frame after a memento
    // is loaded in training mode, for no real practical reason.
}

void fSystem::RecordAllToInternalMementos(rSystem* system, GameMementoKey::MementoID* id) {
    // This method exists entirely to work around the check that actors are
    // movable before the training mode mementos are saveable. This could be
    // replaced just by no-oping the `jz` instruction at 0x5d7fa0, but this
    // is probably more legible.
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


void fSystem::StartGGPO(int remotePosition, const SteamNetworkingIPAddr* remoteAddr) {
    GGPOSessionCallbacks cb = { 0 };
    cb.begin_game = ggpo_begin_game_callback;
    cb.advance_frame = ggpo_advance_frame_callback;
    cb.load_game_state = ggpo_load_game_state_callback;
    cb.save_game_state = ggpo_save_game_state_callback;
    cb.free_buffer = ggpo_free_buffer;
    cb.on_event = ggpo_on_event_callback;
    cb.log_game_state = ggpo_log_game_state;

    char ipbuffer[32];
    int localPosition = remotePosition == 1 ? 0 : 1;
    int localPort = 23456 + 1 + localPosition;
    GGPOErrorCode result = ggpo_start_session(&ggpo, &cb, "sf4e", 2, sizeof(int), localPort);
    ggpo_set_disconnect_timeout(ggpo, 3000);
    ggpo_set_disconnect_notify_start(ggpo, 1000);

    for (int i = 0; i < 2; i++) {
        // GGPO is one-indexed.
        players[i].player_num = i + 1;
        if (i == remotePosition) {
            players[i].type = GGPO_PLAYERTYPE_REMOTE;
            remoteAddr->ToString(players[i].u.remote.ip_address, 32, false);
            players[i].u.remote.port = 23456 + 1 + remotePosition;
        }
        else {
            players[i].type = GGPO_PLAYERTYPE_LOCAL;
        }

        result = ggpo_add_player(ggpo, players + i, &playerHandles[i]);

        if (players[i].type == GGPO_PLAYERTYPE_LOCAL) {
            ggpo_set_frame_delay(ggpo, playerHandles[i], 3);
        }
    }

    nNextBattleStartFlowTarget = BF__MATCH_START;
    fVsBattle::bTerminateOnNextLeftBattle = true;
}

bool fSystem::ggpo_begin_game_callback(const char*)
{
    return true;
}

bool fSystem::ggpo_advance_frame_callback(int)
{
    DWORD inputs[2] = { 0 };
    int disconnect_flags = 0;

    // Make sure we fetch new inputs from GGPO and use those to update
    // the game state instead of reading from the keyboard.
    ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int) * 2, &disconnect_flags);
    fPadSystem::playbackFrame = 0;
    fPadSystem::playbackData[0][0] = inputs[0];
    fPadSystem::playbackData[0][1] = inputs[1];

    // Disable HUD updates- since we don't have save states for them,
    // simulating them in rollbacks would result in animations jumping around.
    // If the HUD relies on iterative updates, this may break things.
    Hud::bAllowHudUpdate = false;
    IUnit::bAllowHudUpdate = false;

    rSystem* system = rSystem::staticMethods.GetSingleton();
    // It's important that this calls the _original_, undetoured method-
    // if it called fSystem::BattleUpdate, it'd be restricted to the same
    // update-halting that the detoured method is.
    (system->*rSystem::publicMethods.BattleUpdate)();

    GGPOErrorCode err = ggpo_advance_frame(ggpo);
    if (!GGPO_SUCCEEDED(err)) {
        MessageBoxA(NULL, "sf4e system could not advance frame after callback! Will likely crash!", NULL, MB_OK);
    }

    // Reenable the HUD updates.
    Hud::bAllowHudUpdate = true;
    IUnit::bAllowHudUpdate = true;
    fPadSystem::playbackFrame = -1;
    return true;
}

bool fSystem::ggpo_load_game_state_callback(unsigned char* buffer, int len)
{
    SaveState* state = (SaveState*)buffer;
    std::pair<rKey*, rKey> tmp[NUM_MEMENTO_KEYS_IN_SAVE_STATE];

    // Copy the state of all the keys we'll replace in advance,
    // for later use.
    for (int i = 0; i < NUM_SAVE_STATES; i++) {
        tmp[i] = state->keys[i];
    }

    SaveState::Load(state);

    // If the memento key data from the source state were left in the key,
    // the next save would result in invalidating the memento key data and
    // the `SaveState()` pointing at invalid memory.
    //
    // Replace the mementos installed by the `Load()` call with the state
    // at the beginning of the callback.
    for (int i = 0; i < NUM_SAVE_STATES; i++) {
        if (tmp[i].first) {
            *tmp[i].first = tmp[i].second;
        }
    }
    return true;
}

bool fSystem::ggpo_save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int)
{
    // No GGPO callback allocates data- SF4e preallocates and
    // manages all its savestates. Consequently the memory
    // utilization of _GGPO_ is technically zero- but GGPO
    // errors with an assertion if the length is zero.
    *len = 1;

    // Find an empty position in our array, and store if we can
    // find one.
    for (int i = 0; i < NUM_SAVE_STATES; i++) {
        if (saveStates[i].used) {
            continue;
        }

        SaveState::Save(&saveStates[i]);
        *buffer = (unsigned char*)&saveStates[i];
        *checksum = 0;

        // Now that we've saved once, existing keys can be
        // reinitialized, but there shouldn't be any new keys.
        fKey::bWarnOnNewKeys = true;

        return true;
    }

    // No empty position in the array- either there aren't enough available
    // states, or the states aren't being released or tracked correctly.
    *buffer = nullptr;
    spdlog::error("FATAL: Could not store GGPO state!");
    MessageBoxA(NULL, "FATAL: Could not store GGPO state! Will likely crash! Attach a debugger here!", NULL, MB_OK);
    return false;
}

bool fSystem::ggpo_log_game_state(char* filename, unsigned char* buffer, int)
{
    return true;
}

void fSystem::ggpo_free_buffer(void* buffer)
{
    SaveState* victim = (SaveState*)buffer;
    rSystem* system = rSystem::staticMethods.GetSingleton();
    SaveState tmp;

    SaveState::Save(&tmp);

    // Calls to clear SF4's mementos delegate those calls to the mementoable
    // object. If the mementoable object pointer isn't valid, the key can't
    // be cleared. This isn't relevant to SF4's training mode, because clearing
    // is only ever done on re-initialization after a save, but manually
    // clearing keys when releasing the state is necessary for GGPO to avoid
    // memory leaks.
    // 
    // Load the victim state into the engine. Once the victim state is loaded,
    // the mementoable object pointers in each key are valid, and the key can
    // be safely cleared.
    SaveState::Load(victim);
    victim->used = false;
    for (int i = 0; i < NUM_MEMENTO_KEYS_IN_SAVE_STATE; i++) {
        if (victim->keys[i].first) {
            (victim->keys[i].first->*rKey::publicMethods.ClearKey)();
            memset(victim->keys[i].first, 0, sizeof(rKey));
            victim->keys[i].first = nullptr;
        }
    }
    
    // Reload the state at the start of the function, then free the
    // temporary save state too. We don't need to handle clearing this
    // save state- the next save will clear it, and the next load will
    // respect it.
    SaveState::Load(&tmp);
}

bool fSystem::ggpo_on_event_callback(GGPOEvent* info) {
    int progress;
    switch (info->code) {
    case GGPO_EVENTCODE_CONNECTED_TO_PEER:
        spdlog::info("GGPO: Connected!");
        break;
    case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
        progress = 100 * info->u.synchronizing.count / info->u.synchronizing.total;
        spdlog::info("GGPO: Synchronizing: {}", progress);
        break;
    case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
        spdlog::info("GGPO: Synchronized with peer");
        break;
    case GGPO_EVENTCODE_RUNNING:
        spdlog::info("GGPO: Running");
        break;
    case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
        spdlog::info("GGPO: GGPO_EVENTCODE_CONNECTION_INTERRUPTED");
        break;
    case GGPO_EVENTCODE_CONNECTION_RESUMED:
        spdlog::info("GGPO: GGPO_EVENTCODE_CONNECTION_RESUMED");
        break;
    case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
        MessageBoxA(NULL, "Disconnected from peer!", NULL, MB_OK);
        break;
    case GGPO_EVENTCODE_TIMESYNC:
        Sleep(1000 * info->u.timesync.frames_ahead / 60);
        break;
    }
    return true;
}

void fSystem::SaveState::Save(SaveState* dst) {
    rSystem* system = rSystem::staticMethods.GetSingleton();
    int keyCount = 0;

    dst->used = true;

    RecordAllToInternalMementos(system, &GGPO_MEMENTO_ID);
    for (auto iter = fKey::trackedKeys.begin(); iter != fKey::trackedKeys.end(); iter++) {
        dst->keys[keyCount] = std::make_pair(*iter, **iter);

        // If we leave the data in the source key, reinitialization
        // of the source key will end up freeing _our_ data. Make
        // absolutely sure to zero the source key. Ideally, we could
        // just replace the key's state with the state the key had
        // before the call to RecordAll... but the mementos won't
        // be tracked until after that call.
        memset(*iter, 0, sizeof(rKey));

        keyCount++;
    }
    for (keyCount; keyCount < NUM_MEMENTO_KEYS_IN_SAVE_STATE; keyCount++) {
        dst->keys[keyCount].first = nullptr;
    }

    dst->CurrentBattleFlow = *rSystem::staticVars.CurrentBattleFlow;
    dst->PreviousBattleFlow = *rSystem::staticVars.PreviousBattleFlow;
    dst->CurrentBattleFlowSubstate = *rSystem::staticVars.CurrentBattleFlowSubstate;
    dst->PreviousBattleFlowSubstate = *rSystem::staticVars.PreviousBattleFlowSubstate;
    dst->CurrentBattleFlowFrame = *rSystem::staticVars.CurrentBattleFlowFrame;
    dst->CurrentBattleFlowSubstateFrame = *rSystem::staticVars.CurrentBattleFlowSubstateFrame;
    dst->PreviousBattleFlowFrame = *rSystem::staticVars.PreviousBattleFlowFrame;
    dst->PreviousBattleFlowSubstateFrame = *rSystem::staticVars.PreviousBattleFlowSubstateFrame;
    dst->BattleFlowSubstateCallable_aa9258 = *rSystem::staticVars.BattleFlowSubstateCallable_aa9258;
    dst->BattleFlowCallback_CallEveryFrame_aa9254 = *rSystem::staticVars.BattleFlowCallback_CallEveryFrame_aa9254;
}

void fSystem::SaveState::Load(SaveState* src) {
    rSystem* system = rSystem::staticMethods.GetSingleton();

    *rSystem::staticVars.CurrentBattleFlow = src->CurrentBattleFlow;
    *rSystem::staticVars.PreviousBattleFlow = src->PreviousBattleFlow;
    *rSystem::staticVars.CurrentBattleFlowSubstate = src->CurrentBattleFlowSubstate;
    *rSystem::staticVars.PreviousBattleFlowSubstate = src->PreviousBattleFlowSubstate;
    *rSystem::staticVars.CurrentBattleFlowFrame = src->CurrentBattleFlowFrame;
    *rSystem::staticVars.CurrentBattleFlowSubstateFrame = src->CurrentBattleFlowSubstateFrame;
    *rSystem::staticVars.PreviousBattleFlowFrame = src->PreviousBattleFlowFrame;
    *rSystem::staticVars.PreviousBattleFlowSubstateFrame = src->PreviousBattleFlowSubstateFrame;
    *rSystem::staticVars.BattleFlowSubstateCallable_aa9258 = src->BattleFlowSubstateCallable_aa9258;
    *rSystem::staticVars.BattleFlowCallback_CallEveryFrame_aa9254 = src->BattleFlowCallback_CallEveryFrame_aa9254;

    // Place each memento back into its position.
    for (int i = 0; i < NUM_MEMENTO_KEYS_IN_SAVE_STATE; i++) {
        if (src->keys[i].first) {
            *src->keys[i].first = src->keys[i].second;
        }
    }

    // Force the system to reload from the replaced mementos.
    RestoreAllFromInternalMementos(system, &GGPO_MEMENTO_ID);
}