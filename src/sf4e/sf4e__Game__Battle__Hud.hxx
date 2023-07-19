#pragma once

#include <windows.h>

#include "../Dimps/Dimps__Eva.hxx"
#include "../Dimps/Dimps__Game.hxx"
#include "../Dimps/Dimps__Game__Battle__Hud.hxx"

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
					struct Unit : Dimps::Game::Battle::Hud::Announce::Unit {
						void HudAnnounce_Update(Task** task);
						static void Install();
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