#include <windows.h>

#include "Dimps__Eva.hxx"
#include "Dimps__Game__Battle.hxx"
#include "Dimps__Game__Battle__Hud.hxx"

using Dimps::Eva::Task;
using Dimps::Game::Battle::IUnit;

namespace Hud = Dimps::Game::Battle::Hud;
using Hud::Unit;

Unit::__publicMethods Unit::publicMethods;
Unit::__staticMethods Unit::staticMethods;

Hud::Announce::Unit::__publicMethods Hud::Announce::Unit::publicMethods;
Hud::Cockpit::Unit::__publicMethods Hud::Cockpit::Unit::publicMethods;
Hud::Cockpit::View::__publicMethods Hud::Cockpit::View::publicMethods;
Hud::Cursor::Unit::__publicMethods Hud::Cursor::Unit::publicMethods;
Hud::Result::Unit::__publicMethods Hud::Result::Unit::publicMethods;
Hud::Subtitle::Unit::__publicMethods Hud::Subtitle::Unit::publicMethods;
Hud::Training::Unit::__publicMethods Hud::Training::Unit::publicMethods;

void (IUnit::* Hud::SharedHudUpdate)(Task** task);

void Hud::Locate(HMODULE peRoot) {
	Unit::Locate(peRoot);
	Announce::Unit::Locate(peRoot);
	Cockpit::Unit::Locate(peRoot);
	Cockpit::View::Locate(peRoot);
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
	staticMethods.Factory = (Unit*(*)(DWORD, DWORD))(peRootOffset + 0x1731a0);
}

void Hud::Announce::Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.HudAnnounce_Update = (PVOID)(peRootOffset + 0x180b90);
}

void Hud::Cockpit::Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.HudCockpit_Update = (PVOID)(peRootOffset + 0x173f00);
}

void Hud::Cockpit::View::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.RestoreFromInternalMementoKey = (PVOID)(peRootOffset + 0x174c70);
	*(PVOID*)&publicMethods.Update = (PVOID)(peRootOffset + 0x1764b0);
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

void Unit::ResetUnits(Unit* u) {
	*(Cockpit::Unit**)((unsigned int)u + 0x18) = 0x0;
	*(Notice::Unit**)((unsigned int)u + 0x1c) = 0x0;
	*(Announce::Unit**)((unsigned int)u + 0x20) = 0x0;
	*(Cursor::Unit**)((unsigned int)u + 0x24) = 0x0;
	*(Result::Unit**)((unsigned int)u + 0x28) = 0x0;
	*(Continue::Unit**)((unsigned int)u + 0x2c) = 0x0;
	*(Subtitle::Unit**)((unsigned int)u + 0x30) = 0x0;
	*(Training::Unit**)((unsigned int)u + 0x34) = 0x0;
}
