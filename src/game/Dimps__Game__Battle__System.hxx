#pragma once

#include <windows.h>

#include "Dimps__Game__Battle__Chara__Unit.hxx"

namespace Dimps {
	namespace Game {
		namespace Battle {
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