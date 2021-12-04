#pragma once

#include <windows.h>

#include "Dimps__Game__Battle__Chara__Unit.hxx"

namespace Dimps {
	namespace Game {
		namespace Battle {
			struct System
			{
				enum BattleFlow {
					ROUND_ACTIVE = 2,
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

				typedef struct __privateMethods {
					// TODO
				} __privateMethods;

				typedef struct __publicMethods {
					Chara::Unit* (System::* GetCharaUnit)();
					bool (System::* IsFight)();
					bool (System::* IsLeavingBattle)();
					void (System::* StoreMemento)();
					void (System::* RestoreMemento)();
				} __publicMethods;

				typedef struct __staticMethods {
					System* (WINAPI* GetSingleton)();
				} __staticMethods;

				static void Locate(HMODULE peRoot);
				static __privateMethods privateMethods;
				static __publicMethods publicMethods;
				static __staticMethods staticMethods;

				// Instance values here
			};
		}
	}
}