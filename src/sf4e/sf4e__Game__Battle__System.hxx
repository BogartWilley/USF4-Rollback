#pragma once
#include <utility>
#include <vector>

#include <GameNetworkingSockets/steam/steamnetworkingtypes.h>
#include <ggponet.h>

#include "../Dimps/Dimps__Game.hxx"
#include "../Dimps/Dimps__Game__Battle.hxx"
#include "../Dimps/Dimps__Game__Battle__System.hxx"
#include "../Dimps/Dimps__Math.hxx"

#include "../session/sf4e__SessionProtocol.hxx"

#include "sf4e__Platform.hxx"
#include "sf4e__Game__Battle__Hud.hxx"

#define NUM_SAVE_STATES (GGPO_MAX_PREDICTION_FRAMES + 2)

namespace sf4e {
	namespace Game {
		namespace Battle {
			using Dimps::Game::GameMementoKey;
			using Dimps::Math::FixedPoint;

			struct System : Dimps::Game::Battle::System
			{
				typedef struct AdditionalMemento {
					int nFirstCharaToSimulate;
					DWORD skipRelatedFlags_0xd8c;
					DWORD simulationFlags;
					FixedPoint transitionProgress;
					FixedPoint transitionSpeed;
					int transitionType;

					Dimps::Game::Battle::Network::Unit network;
					Hud::Announce::Unit::AdditionalMemento announce;
					Platform::GFxApp::AdditionalMemento gfxApp;
					Eva::TaskCore::AdditionalMemento updateCore;
				} AdditionalMemento;

				struct PlayerConnectionInfo {
					GGPOPlayerType       type;
					GGPOPlayerHandle     handle;
				};

				static bool bHaltAfterNext;
				static bool bRandomizeLocalInputsInGGPO;
				static bool bUpdateAllowed;
				static int nExtraFramesToSimulate;
				static int nNextBattleStartFlowTarget;

				static Dimps::Game::GameMementoKey::MementoID loadRequest;
				static Dimps::Game::GameMementoKey::MementoID saveRequest;

				static void Install();
				static void RestoreAllFromInternalMementos(Dimps::Game::Battle::System* system, GameMementoKey::MementoID* id);
				static void RecordAllToInternalMementos(Dimps::Game::Battle::System* system, GameMementoKey::MementoID* id);

				int GetMementoSize();
				int RecordToMemento(Memento* memento, GameMementoKey::MementoID* id);
				int RestoreFromMemento(Memento* memento, GameMementoKey::MementoID* id);

				void BattleUpdate();
				void CloseBattle();
				static void OnBattleFlow_BattleStart(System* s);
				void SysMain_HandleTrainingModeFeatures();
				void SysMain_UpdatePauseState();

				struct SaveState {
					bool used = false;
					std::vector<std::pair<GameMementoKey*, GameMementoKey>> keys;

					struct GlobalData {
						DWORD CurrentBattleFlow = 0;
						DWORD PreviousBattleFlow = 0;
						DWORD CurrentBattleFlowSubstate = 0;
						DWORD PreviousBattleFlowSubstate = 0;
						FixedPoint CurrentBattleFlowFrame = { 0, 0 };
						FixedPoint CurrentBattleFlowSubstateFrame = { 0, 0 };
						FixedPoint PreviousBattleFlowFrame = { 0, 0 };
						FixedPoint PreviousBattleFlowSubstateFrame = { 0, 0 };
						void (*BattleFlowSubstateCallable_aa9258)(Dimps::Game::Battle::System * s) = nullptr;
						void (*BattleFlowCallback_CallEveryFrame_aa9254)(Dimps::Game::Battle::System * s) = nullptr;

						Dimps::Game::Battle::GameManager gameManager = { 0 };
					};
					GlobalData d;

					SaveState();

					static void Save(SaveState* dst);
					static void Load(SaveState* src);
				};

				struct StateSnapshotMeta {
					bool sent;
					bool confirmed;
				};

				static void CaptureSnapshot(Dimps::Game::Battle::System* src);
				static std::map<int, std::pair<SessionProtocol::StateSnapshot, StateSnapshotMeta>> snapshotMap;
				static GGPOPlayerHandle localPlayerHandle;
				static PlayerConnectionInfo players[MAX_SF4E_PROTOCOL_USERS];
				static GGPOSession* ggpo;
				static SaveState saveStates[NUM_SAVE_STATES];

				static void StartGGPO(GGPOPlayer* players, int numPlayers, int port, int frameDelay, DWORD rngSeed);
				static void StartSpectating(unsigned short localport, int num_players, char* host_ip, unsigned short host_port, DWORD rngSeed);
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