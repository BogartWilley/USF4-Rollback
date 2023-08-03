#pragma once

#include <windows.h>

#include "../Dimps/Dimps__Eva.hxx"
#include "../Dimps/Dimps__Game.hxx"
#include "../Dimps/Dimps__Game__Battle__Hud.hxx"

#include "sf4e.hxx"
#include "sf4e__Game.hxx"

namespace sf4e {
	namespace Game {
		namespace Battle {
			namespace Hud {
				using Dimps::Eva::Task;
				using Dimps::Game::GameMementoKey;

				void Install();
				extern bool bAllowHudUpdate;
				
				struct Unit : Dimps::Game::Battle::Hud::Unit {
					static void Install();
					static Dimps::Game::Battle::Hud::Unit* Factory(DWORD arg1, DWORD arg2);
				};

				namespace Announce {
					struct Round : Dimps::Game::Battle::Hud::Announce::Round {
						struct AdditionalMemento {
							bool enabled;
							WORD currentRound;
						};

						static void RecordToAdditionalMemento(Dimps::Game::Battle::Hud::Announce::Round* n, AdditionalMemento& m);
						static void RestoreFromAdditionalMemento(Dimps::Game::Battle::Hud::Announce::Round* n, const AdditionalMemento& m);
					};

					struct Unit : Dimps::Game::Battle::Hud::Announce::Unit {
						typedef struct AdditionalMemento {
							Dimps::Game::Battle::Hud::Announce::Announcement queuedAnnouncements[4];
							int numQueuedAnnouncements;
							Dimps::Game::Sprite::Control* activeControl;

							Round::AdditionalMemento round;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento finalRound;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento extraRound;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento fight;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento ko;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento doubleKo;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento time;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento draw;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento perfect;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento win;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento lose;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento wins1;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento wins2;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento success;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento clear;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento readyGo;
							sf4e::Game::Sprite::SingleNodeControl::AdditionalMemento edition;
						} AdditionalMemento;

						void HudAnnounce_Update(Task** task);
						static void Install();
						static void RecordToAdditionalMemento(Dimps::Game::Battle::Hud::Announce::Unit* u, AdditionalMemento& m);
						static void RestoreFromAdditionalMemento(Dimps::Game::Battle::Hud::Announce::Unit* u, const AdditionalMemento& m);
					};
				}

				namespace Cockpit {
					struct Unit : Dimps::Game::Battle::Hud::Cockpit::Unit {
						void HudCockpit_Update(Task** task);
						static void Install();
					};

					struct View : Dimps::Game::Battle::Hud::Cockpit::View {
						void RestoreFromInternalMementoKey(GameMementoKey::MementoID* id);
						static void Install();
					};
				}

				namespace Cursor {
					struct Unit : Dimps::Game::Battle::Hud::Cursor::Unit {
						void HudCursor_Update(Task** task);
						static void Install();
					};
				}

				namespace Result {
					struct Unit : Dimps::Game::Battle::Hud::Result::Unit {
						void HudResult_Update(Task** task);
						static void Install();
					};
				}

				namespace Subtitle {
					struct Unit : Dimps::Game::Battle::Hud::Subtitle::Unit {
						void HudSubtitle_Update(Task** task);
						static void Install();
					};
				}

				namespace Training {
					struct Unit : Dimps::Game::Battle::Hud::Training::Unit {
						void HudTraining_Update(Task** task);
						static void Install();
					};
				}
			}
		}
	}
}