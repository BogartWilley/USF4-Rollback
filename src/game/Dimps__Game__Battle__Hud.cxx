#include <windows.h>

#include "Dimps__Eva.hxx"
#include "Dimps__Game__Battle.hxx"
#include "Dimps__Game__Battle__Hud.hxx"

using Dimps::Eva::Task;
using Dimps::Game::Battle::IUnit;

namespace Hud = Dimps::Game::Battle::Hud;
using Hud::Unit;

Unit::__privateMethods Unit::privateMethods;
Unit::__publicMethods Unit::publicMethods;
Unit::__staticMethods Unit::staticMethods;

Hud::Announce::Unit::__publicMethods Hud::Announce::Unit::publicMethods;
Hud::Cockpit::Unit::__publicMethods Hud::Cockpit::Unit::publicMethods;
Hud::Cursor::Unit::__publicMethods Hud::Cursor::Unit::publicMethods;
Hud::Result::Unit::__publicMethods Hud::Result::Unit::publicMethods;
Hud::Subtitle::Unit::__publicMethods Hud::Subtitle::Unit::publicMethods;
Hud::Training::Unit::__publicMethods Hud::Training::Unit::publicMethods;

void (IUnit::* Hud::SharedHudUpdate)(Task** task);

void Hud::Locate(HMODULE peRoot) {
	Unit::Locate(peRoot);
	Announce::Unit::Locate(peRoot);
	Cockpit::Unit::Locate(peRoot);
	Cursor::Unit::Locate(peRoot);
	Result::Unit::Locate(peRoot);
	Subtitle::Unit::Locate(peRoot);
	Training::Unit::Locate(peRoot);

	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&SharedHudUpdate = (PVOID)(peRootOffset + 0x18ae90);
}

void Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.RecordToInternalMementoKey = (PVOID)(peRootOffset + 0x172e60);
	*(PVOID*)&publicMethods.RestoreFromInternalMementoKey = (PVOID)(peRootOffset + 0x172e90);
}

void Hud::Announce::Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.HudAnnounce_Update = (PVOID)(peRootOffset + 0x180b90);
}

void Hud::Cockpit::Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.HudCockpit_Update = (PVOID)(peRootOffset + 0x173f00);
}

void Hud::Cursor::Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.HudCursor_Update = (PVOID)(peRootOffset + 0x18c130);
}

void Hud::Result::Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.HudResult_Update = (PVOID)(peRootOffset + 0x1834d0);
}

void Hud::Subtitle::Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.HudSubtitle_Update = (PVOID)(peRootOffset + 0x18e2c0);
}

void Hud::Training::Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.HudTraining_Update = (PVOID)(peRootOffset + 0x18f290);
}