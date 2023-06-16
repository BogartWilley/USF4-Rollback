#pragma once
#include <utility>
#include <vector>

#include <GameNetworkingSockets/steam/steamnetworkingtypes.h>
#include <ggponet.h>

#include "../Dimps/Dimps__Game.hxx"
#include "../Dimps/Dimps__Game__Battle__System.hxx"
#include "../Dimps/Dimps__Math.hxx"

#define NUM_SAVE_STATES (GGPO_MAX_PREDICTION_FRAMES + 2)

namespace sf4e {
	namespace Game {
		namespace Battle {
			using Dimps::Game::GameMementoKey;
			using Dimps::Math::FixedPoint;

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


				struct SaveState {
					bool used;
					std::vector<std::pair<GameMementoKey*, GameMementoKey>> keys;

					DWORD CurrentBattleFlow;
					DWORD PreviousBattleFlow;
					DWORD CurrentBattleFlowSubstate;
					DWORD PreviousBattleFlowSubstate;
					FixedPoint CurrentBattleFlowFrame;
					FixedPoint CurrentBattleFlowSubstateFrame;
					FixedPoint PreviousBattleFlowFrame;
					FixedPoint PreviousBattleFlowSubstateFrame;
					void (*BattleFlowSubstateCallable_aa9258)(Dimps::Game::Battle::System* s);
					void (*BattleFlowCallback_CallEveryFrame_aa9254)(Dimps::Game::Battle::System* s);

					SaveState();
					static void Save(SaveState* dst);
					static void Load(SaveState* src);
				};

				static GGPOPlayer players[2];
				static GGPOSession* ggpo;
				static SaveState saveStates[NUM_SAVE_STATES];

				static void StartGGPO(int remotePosition, const SteamNetworkingIPAddr* remoteAddr);
				static bool ggpo_on_event_callback(GGPOEvent* info);
				static bool ggpo_begin_game_callback(const char*);
				static bool ggpo_advance_frame_callback(int);
				static bool ggpo_load_game_state_callback(unsigned char*, int);
				static bool ggpo_save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int);
				static void ggpo_free_buffer(void* buffer);
				static bool ggpo_log_game_state(char* filename, unsigned char* buffer, int);
			};
		}
	}
}