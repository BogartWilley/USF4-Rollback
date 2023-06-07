#pragma once

#include "../Dimps/Dimps__Game.hxx"
#include "../Dimps/Dimps__Game__Battle__System.hxx"

namespace sf4e {
	namespace Game {
		namespace Battle {
			struct System : Dimps::Game::Battle::System
			{
				static bool bHaltAfterNext;
				static bool bUpdateAllowed;
				static int nExtraFramesToSimulate;
				static int nNextBattleStartFlowTarget;
				static Dimps::Game::GameMementoKey::MementoID loadRequest;
				static Dimps::Game::GameMementoKey::MementoID saveRequest;

				static void Install();
				static void RestoreAllFromInternalMementos(Dimps::Game::Battle::System* system, Dimps::Game::GameMementoKey::MementoID* id);
				static void RecordAllToInternalMementos(Dimps::Game::Battle::System* system, Dimps::Game::GameMementoKey::MementoID* id);

				void BattleUpdate();
				static void OnBattleFlow_BattleStart(System* s);
				void SysMain_HandleTrainingModeFeatures();
			};
		}
	}
}