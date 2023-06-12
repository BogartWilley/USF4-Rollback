#pragma once

#include <windows.h>

#include "Dimps__Eva.hxx"
#include "Dimps__Game.hxx"
#include "Dimps__Game__Battle.hxx"

namespace Dimps {
	namespace Game {
		namespace Battle {
			namespace Hud {
				using Dimps::Eva::Task;
				using Dimps::Game::Battle::IUnit;
				using Dimps::Game::GameMementoKey;

				void Locate(HMODULE peRoot);

				// Dimps::Game::Battle::Hud::Continue::Unit and
				// Dimps::Game::Battle::Hud::Notice::Unit have identical
				// implementations of their update tasks, despite the fact
				// that their only shared ancestors (IMessageReceivable and
				// IUnit) don't define the method or the View object that
				// the method delegates to. Since the implementation ends up
				// being compiled to the same machine code, the compiler
				// deduplicated the implementation, which makes it impossible
				// to safely represent the original source implementation with
				// two separate pointers in each Unit class. Instead, _one_
				// pointer to the deduplicated implementation is stored here,
				// in the nearest containing namespace.
				//
				// Strictly speaking, the polymorphism here is wrong- there's no
				// actual way to express the deduplication performed by the
				// compiler- but it is at least functional.
				extern void (IUnit::* SharedHudUpdate)(Task** task);

				namespace Announce {
					struct Unit : IUnit {
						typedef struct __publicMethods {
							void (Unit::* HudAnnounce_Update)(Task** task);
						} __publicMethods;

						static void Locate(HMODULE peRoot);
						static __publicMethods publicMethods;
					};
				}

				namespace Cockpit {
					struct Unit : IUnit {
						typedef struct __publicMethods {
							void (Unit::* HudCockpit_Update)(Task** task);
						} __publicMethods;

						static void Locate(HMODULE peRoot);
						static __publicMethods publicMethods;
					};
				}

				namespace Continue {
					struct Unit : IUnit {};
				}

				namespace Cursor {
					struct Unit : IUnit {
						typedef struct __publicMethods {
							void (Unit::* HudCursor_Update)(Task** task);
						} __publicMethods;

						static void Locate(HMODULE peRoot);
						static __publicMethods publicMethods;
					};
				}

				namespace Notice {
					struct Unit : IUnit {};
				}

				namespace Result {
					struct Unit : IUnit {
						typedef struct __publicMethods {
							void (Unit::* HudResult_Update)(Task** task);
						} __publicMethods;

						static void Locate(HMODULE peRoot);
						static __publicMethods publicMethods;
					};
				}

				namespace Subtitle {
					struct Unit : IUnit {
						typedef struct __publicMethods {
							void (Unit::* HudSubtitle_Update)(Task** task);
						} __publicMethods;

						static void Locate(HMODULE peRoot);
						static __publicMethods publicMethods;
					};
				}

				namespace Training {
					struct Unit : IUnit {
						typedef struct __publicMethods {
							void (Unit::* HudTraining_Update)(Task** task);
						} __publicMethods;

						static void Locate(HMODULE peRoot);
						static __publicMethods publicMethods;
					};
				}

				struct Unit : IUnit
				{
					typedef struct __publicMethods {
						void (Unit::* RecordToInternalMementoKey)(GameMementoKey::MementoID* id);
						void (Unit::* RestoreFromInternalMementoKey)(GameMementoKey::MementoID* id);
					} __publicMethods;

					typedef struct __staticMethods {
						Unit* (*Factory)(DWORD arg1, DWORD arg2);
					} __staticMethods;

					static void Locate(HMODULE peRoot);
					static void ResetUnits(Unit* u);

					static __publicMethods publicMethods;
					static __staticMethods staticMethods;
				};
			}
		}
	}
}