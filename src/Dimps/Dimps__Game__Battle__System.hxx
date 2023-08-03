#pragma once

#include <windows.h>

#include "Dimps__Eva.hxx"
#include "Dimps__Game.hxx"
#include "Dimps__Game__Battle.hxx"
#include "Dimps__Game__Battle__Chara.hxx"
#include "Dimps__Math.hxx"

namespace Dimps {
    namespace Game {
        namespace Battle {
            using Dimps::Math::FixedPoint;

            struct System
            {
                enum BattleFlow {
                    BF__START_DEMO = 0,
                    BF__READY = 1,
                    BF__FIGHT = 2,
                    BF__FINISH = 3,
                    BF__ROUND_RESULT = 4,
                    BF__MATCH_RESULT = 5,
                    BF__DRAW_RESULT = 6,
                    BF__BONUS_RESULT = 7,
                    BF__CONTINUE = 8,
                    BF__GAME_OVER = 9,
                    BF__BTL_START = 10,
                    BF__BTL_OVER = 11,
                    BF__MATCH_START = 12,
                    BF__MATCH_OVER = 13,
                    BF__ROUND_START = 14,
                    BF__ROUND_OVER = 15,
                    BF__IDLE = 16,
                    BF__RESTART = 17,
                };

                enum UnitIndex {
                    U_CHARA = 0,
                    U_STAGE = 1,
                    U_VFX = 2,
                    U_HUD = 3,
                    U_CAMERA = 4,
                    U_LIGHT = 5,
                    U_SOUND = 6,
                    U_EFFECT = 7,
                    U_COMMAND = 8,
                    U_NETWORK = 9,
                    U_COM = 10,
                    U_PAUSE = 11
                };

                enum TaskCoreIndex {
                    TCI_UPDATE = 0,
                    TCI_RENDER = 1
                };

                enum BattleExitType {
                    // A non-arcade battle has ended, or an arcade battle has
                    // ended in the player's favor. This may mean displaying
                    // a menu to the users (like versus mode), or it may not
                    // (like network mode).
                    BET_RESULT = 0x0,

                    // Only used by Arcade mode, to return to the main menu
                    // when the player doesn't continue.
                    BET_GAMEOVER = 0x1,

                    // Only used by Arcade mode, to return to the character
                    // select screen when the player continues.
                    BET_CONTINUE = 0x2,

                    // A user paused and selected "exit to main menu", "stop
                    // replay", or otherwise backed completely out of the game.
                    BET_PAUSE_TOMAINMENU = 0x3,

                    // A user paused and selected "exit to character select",
                    // from any of the modes that support that.
                    BET_PAUSE_TOCHARACTER = 0x4,

                    // In trials, the user selected "to next trial" from the
                    // pause menu.
                    BET_TRIAL_TONEXTTRIAL = 0x7,

                    BET_UNDEFINED = 0xffffffff
                };

                enum SystemSimulationFlags {
                    // Stub: check CheckAnyMatchingSimulationFlag @ 0x005d9b80
                    // for more- calls to that are likely helpful for getting
                    // the semantics here.
                    SSF_UNK_0x1 = 0x1,
                };

                enum SystemTransitionType {
                    STT_NONE = 0,
                    STT_FADEINFROMBLACK = 1,
                    STT_FADEOUTTOBLACK = 2
                };

                typedef struct Memento {
                    char pad[0x700];
                } Memento;

                typedef struct __staticVars {
                    DWORD* CurrentBattleFlow;
                    DWORD* PreviousBattleFlow;
                    DWORD* CurrentBattleFlowSubstate;
                    DWORD* PreviousBattleFlowSubstate;
                    FixedPoint* CurrentBattleFlowFrame;
                    FixedPoint* CurrentBattleFlowSubstateFrame;
                    FixedPoint* PreviousBattleFlowSubstateFrame;
                    FixedPoint* PreviousBattleFlowFrame;
                    void(**BattleFlowCallback_CallEveryFrame_aa9254)(System* sys);
                    void(**BattleFlowSubstateCallable_aa9258)(System* sys);
                } __staticVars;

                typedef struct __publicMethods {
                    void (System::* BattleUpdate)();

                    Chara::Unit* (System::* GetCharaUnit)();
                    GameManager* (System::* GetGameManager)();

                    // Realistically this is an IUnit*, but since I don't have complete
                    // typings for now, just mark it as void* and rely on the caller to
                    // cast based on the unit index.
                    void* (System::* GetUnitByIndex)(unsigned int index);

                    void (System::* SysMain_HandleTrainingModeFeatures)();
                    Eva::TaskCore* (System::* GetTaskCore)(unsigned int index);
                    bool (System::* IsFight)();
                    bool (System::* IsLeavingBattle)();
                    void (System::* RecordAllToInternalMementoKeys)();
                    void (System::* RestoreAllFromInternalMementoKeys)();

                    void (System::* RecordToInternalMementoKey)(GameMementoKey::MementoID* id);
                    void (System::* RestoreFromInternalMementoKey)(GameMementoKey::MementoID* id);

                    int (System::* GetGameMode)();
                } __publicMethods;

                typedef struct __mementoableMethods {
                    int (System::* GetMementoSize)();
                    int (System::* RecordToMemento)(Memento* memento, GameMementoKey::MementoID* id);
                    int (System::* RestoreFromMemento)(Memento* memento, GameMementoKey::MementoID* id);
                } __mementoableMethods;

                typedef struct __staticMethods {
                    System* (*GetSingleton)();
                    void (*OnBattleFlow_BattleStart)(System* s);
                    void (*SetBattleFlow)(System* s, int newFlow);
                } __staticMethods;

                // When IMementoable methods are called, the base of the `this`
                // pointer needs to point at the IMementoable vtable contained
                // in the System object, which is _not_ the base of the object-
                // it's at offset 0x4. These helpers can make things a little
                // more clear when invoking IMementoable methods.
                static System* ToMementoable(System* s);
                static System* FromMementoable(System* s);

                static void Locate(HMODULE peRoot);
                static int* GetBattleExitType(System* s);
                static int* GetFirstCharaToSimulate(System* s);

                static Eva::EmRandom* GetRandom(System* s);
                static Request** GetRequest(System* s);
                static DWORD* GetSkipRelatedFlags_0xd8c(System* s);
                static DWORD* GetSimulationFlags(System* s);
                static FixedPoint* GetTransitionProgress(System* s);
                static FixedPoint* GetTransitionSpeed(System* s);
                static int* GetTransitionType(System* s);
                static __mementoableMethods mementoableMethods;
                static __publicMethods publicMethods;
                static __staticMethods staticMethods;
                static __staticVars staticVars;
            };
        }
    }
}