#include <windows.h>

#include "Dimps__Game__Battle__System.hxx"
#include "Dimps__Math.hxx"

using Dimps::Game::Battle::System;
using Dimps::Math::FixedPoint;

System::__publicMethods System::publicMethods;
System::__staticMethods System::staticMethods;
System::__staticVars System::staticVars;

void System::Locate(HMODULE peRoot) {
    unsigned int peRootOffset = (unsigned int)peRoot;
    *(PVOID*)&publicMethods.BattleUpdate = (void (*)(DWORD))(peRootOffset + 0x1d6b10);
    *(PVOID*)&publicMethods.GetCharaUnit = (PVOID)(peRootOffset + 0x163510);
    *(PVOID*)&publicMethods.GetGameManager = (PVOID)(peRootOffset + 0x1d9950);
    *(PVOID*)&publicMethods.GetUnitByIndex = (PVOID)(peRootOffset + 0x1d9720);
    *(PVOID*)&publicMethods.SysMain_HandleTrainingModeFeatures = (PVOID)(peRootOffset + 0x1dab30);
    *(PVOID*)&publicMethods.IsFight = (PVOID)(peRootOffset + 0x1d9f60);
    *(PVOID*)&publicMethods.IsLeavingBattle = (PVOID)(peRootOffset + 0x1d6a70);
    *(PVOID*)&publicMethods.RecordAllToInternalMementoKeys = (PVOID)(peRootOffset + 0x1d7f80);
    *(PVOID*)&publicMethods.RestoreAllFromInternalMementoKeys = (PVOID)(peRootOffset + 0x1d8020);

    *(PVOID*)&publicMethods.RecordToInternalMementoKey = (PVOID)(peRootOffset + 0x1daab0);
    *(PVOID*)&publicMethods.RestoreFromInternalMementoKey = (PVOID)(peRootOffset + 0x1d90d0);

    *(PVOID*)&publicMethods.GetTaskCore = (PVOID)(peRootOffset + 0x1d9930);

    staticMethods.GetSingleton = (System* (*)())(peRootOffset + 0x1dba30);

    staticVars.CurrentBattleFlow = (DWORD*)(peRootOffset + 0x664994);
    staticVars.PreviousBattleFlow = (DWORD*)(peRootOffset + 0x664998);
    staticVars.CurrentBattleFlowSubstate = (DWORD*)(peRootOffset + 0x66499c);
    staticVars.PreviousBattleFlowSubstate = (DWORD*)(peRootOffset + 0x6649a0);
    staticVars.BattleFlowCallback_CallEveryFrame_aa9254 = (void(**)(System*))(peRootOffset + 0x6a9254);
    staticVars.BattleFlowSubstateCallable_aa9258 = (void(**)(System*))(peRootOffset + 0x6a9258);
    staticVars.PreviousBattleFlowSubstateFrame = (FixedPoint*)(peRootOffset + 0x6a925c);
    staticVars.PreviousBattleFlowFrame = (FixedPoint*)(peRootOffset + 0x6a9260);
    staticVars.CurrentBattleFlowFrame = (FixedPoint*)(peRootOffset + 0x6a9264);
    staticVars.CurrentBattleFlowSubstateFrame = (FixedPoint*)(peRootOffset + 0x6a9268);
}