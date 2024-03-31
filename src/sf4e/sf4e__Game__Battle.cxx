#include <windows.h>
#include <detours/detours.h>
#include "spdlog/spdlog.h"

#include "../Dimps/Dimps__Eva.hxx"
#include "../Dimps/Dimps__Game__Battle.hxx"

#include "sf4e__Game__Battle.hxx"
#include "sf4e__Game__Battle__Effect.hxx"
#include "sf4e__Game__Battle__Hud.hxx"
#include "sf4e__Game__Battle__System.hxx"
#include "sf4e__Game__Battle__Vfx.hxx"

namespace rBattle = Dimps::Game::Battle;
namespace fBattle = sf4e::Game::Battle;

using fIUnit = sf4e::Game::Battle::IUnit;
using rIUnit = Dimps::Game::Battle::IUnit;
using SoundHandle = Dimps::Game::Battle::Sound::SoundHandle;
using SoundReference = Dimps::Game::Battle::Sound::SoundReference;
using rSoundPlayerManager = Dimps::Game::Battle::Sound::SoundPlayerManager;
using fSoundPlayerManager = sf4e::Game::Battle::Sound::SoundPlayerManager;
bool fIUnit::bAllowHudUpdate = true;
bool fSoundPlayerManager::bTrackPlays = false;

void fBattle::Install() {
	Effect::Install();
	Hud::Install();
	Sound::SoundPlayerManager::Install();
	System::Install();
	Vfx::Install();
}

void fIUnit::SharedHudUpdate(Task** task) {
	if (bAllowHudUpdate) {
		(this->*rIUnit::publicMethods.SharedHudUpdate)(task);
	}
}

void fSoundPlayerManager::Install() {
	SoundHandle(fSoundPlayerManager:: * _fPlaySound)(
		SoundHandle, uint32_t, DWORD, int32_t, DWORD
	) = &PlaySound;
	DetourAttach((PVOID*)&rSoundPlayerManager::publicMethods.PlaySound, *(PVOID*)&_fPlaySound);
}

SoundHandle fSoundPlayerManager::PlaySound(
	SoundHandle cueSheetHandle,
	uint32_t cueIdx,
	DWORD param_3,
	int32_t flags,
	DWORD maybePosition
) {
	SoundReference cueSheetRef = SoundReference::FromHandle(cueSheetHandle);
	rSoundPlayerManager* _this = (rSoundPlayerManager*)this;

	if (bTrackPlays) {
		spdlog::warn(
			"Sound played: {} {} {} {} {} {}",
			cueSheetRef.position,
			cueSheetRef.useCount,
			cueIdx,
			param_3,
			flags,
			maybePosition
		);
	}

	SoundHandle out = (_this->*rSoundPlayerManager::publicMethods.PlaySound)(
		cueSheetHandle, cueIdx, param_3, flags, maybePosition
	);
	return out;
}
