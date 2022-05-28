#include "overlay.h"

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_demo.cpp>

#include "../game/Dimps.hxx"
#include "../game/Dimps__Eva__TaskCore.hxx"
#include "../game/Dimps__Eva__TaskCoreRegistry.hxx"
#include "../game/Dimps__Game__Battle__Camera.hxx"
#include "../game/Dimps__Game__Battle__Chara__Actor.hxx"
#include "../game/Dimps__Game__Battle__Chara__Unit.hxx"
#include "../game/Dimps__Game__Battle__Command__Unit.hxx"
#include "../game/Dimps__Game__Battle__Effect.hxx"
#include "../game/Dimps__Game__Battle__GameManager.hxx"
#include "../game/Dimps__Game__Battle__Hud.hxx"
#include "../game/Dimps__Game__Battle__System.hxx"
#include "../game/Dimps__Game__Battle__Training.hxx"
#include "../game/Dimps__Game__Battle__Vfx.hxx"
#include "../game/Dimps__Game__GameMementoKey.hxx"
#include "../game/Dimps__Math.hxx"
#include "../game/sf4e__Event.hxx"


#define DEFAULT_ALPHA 0.87f

using CameraUnit = Dimps::Game::Battle::Camera::Unit;
using CharaActor = Dimps::Game::Battle::Chara::Actor;
using CharaUnit = Dimps::Game::Battle::Chara::Unit;
using CommandUnit = Dimps::Game::Battle::Command::Unit;
using EffectUnit = Dimps::Game::Battle::Effect::Unit;
using HudUnit = Dimps::Game::Battle::Hud::Unit;
using TrainingManager = Dimps::Game::Battle::Training::Manager;
using VfxUnit = Dimps::Game::Battle::Vfx::Unit;

using Dimps::Eva::TaskCore;
using Dimps::Eva::TaskCoreRegistry;
using Dimps::Game::Battle::Command::CommandImpl;
using Dimps::Game::Battle::GameManager;
using Dimps::Game::Battle::System;
using Dimps::Game::GameMementoKey;
using Dimps::Math::FixedPoint;
using Dimps::Math::FixedToFloat;
using fEventController = sf4e::Event::EventController;

using ImGui::Begin;
using ImGui::End;
using ImGui::NextColumn;
using ImGui::Text;

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool show_battle_system_window = false;
static bool show_command_window = false;
static bool show_demo_window = false;
static bool show_help_window = false;
static bool show_memento_window = false;
static bool show_task_window = false;

void DrawBattleSystemWindow(bool* pOpen) {
	ImGui::Begin(
		"Battle System",
		pOpen,
		ImGuiWindowFlags_None
	);

	FixedPoint tmp, tmp2, tmp3;
	System* system = System::staticMethods.GetSingleton();
	GameManager* manager = (system->*System::publicMethods.GetGameManager)();

	int isFight = (system->*System::publicMethods.IsFight)();
	ImGui::Text("Is fight: %d", isFight);
	ImGui::Text("Is leaving battle: %d", (system->*System::publicMethods.IsLeavingBattle)());
	if (manager != NULL) {
		(manager->*GameManager::publicMethods.GetRoundTime)(&tmp);
		ImGui::Text("Round time: %f", FixedToFloat(&tmp));
	}

	if (isFight) {
		CharaUnit* lpCharaUnit = (system->*System::publicMethods.GetCharaUnit)();

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("Battle Actor", tab_bar_flags))
		{
			for (int i = 0; i < 2; i++) {
				CharaActor* a = (lpCharaUnit->*CharaUnit::publicMethods.GetActorByIndex)(i);

				if (ImGui::BeginTabItem(i == 0 ? "Actor 0" : "Actor 1")) {
					ImGui::Columns(2, NULL, false);

					int actorID = (int)(a->*CharaActor::publicMethods.GetActorID)();
					char* actorCode = actorID > -1 ? Dimps::characterCodes[actorID] : "";

					Text("Actor ID:"); NextColumn();
					Text("%d", actorID); NextColumn();

					Text("Actor code:"); NextColumn();
					Text(actorCode); NextColumn();

					Text("Status:"); NextColumn();
					Text("%d", (a->*CharaActor::publicMethods.GetStatus)()); NextColumn();

					Text("Current side:"); NextColumn();
					Text("%d", (a->*CharaActor::publicMethods.GetCurrentSide)()); NextColumn();

					(a->*CharaActor::publicMethods.GetVitalityAmt_FixedPoint)(&tmp);
					(a->*CharaActor::publicMethods.GetVitalityMax_FixedPoint)(&tmp2);
					(a->*CharaActor::publicMethods.GetVitalityPct_FixedPoint)(&tmp3);
					Text("Vitality:"); NextColumn();
					Text("%f / %f (%fa%%)", FixedToFloat(&tmp), FixedToFloat(&tmp2), FixedToFloat(&tmp3) * 100); NextColumn();

					(a->*CharaActor::publicMethods.GetRevengeAmt_FixedPoint)(&tmp);
					(a->*CharaActor::publicMethods.GetRevengeMax_FixedPoint)(&tmp2);
					(a->*CharaActor::publicMethods.GetRevengePct_FixedPoint)(&tmp3);
					Text("Revenge:"); NextColumn();
					Text("%f / %f (%f%%)", FixedToFloat(&tmp), FixedToFloat(&tmp2), FixedToFloat(&tmp3) * 100); NextColumn();

					(a->*CharaActor::publicMethods.GetRecoverableVitalityAmt_FixedPoint)(&tmp);
					(a->*CharaActor::publicMethods.GetRecoverableVitalityMax_FixedPoint)(&tmp2);
					(a->*CharaActor::publicMethods.GetRecoverableVitalityPct_FixedPoint)(&tmp3);
					Text("Recoverable Vitality:"); NextColumn();
					Text("%f / %f (%f%%)", FixedToFloat(&tmp), FixedToFloat(&tmp2), FixedToFloat(&tmp3) * 100); NextColumn();

					(a->*CharaActor::publicMethods.GetSuperComboAmt_FixedPoint)(&tmp);
					(a->*CharaActor::publicMethods.GetSuperComboMax_FixedPoint)(&tmp2);
					(a->*CharaActor::publicMethods.GetSuperComboPct_FixedPoint)(&tmp3);
					Text("Super:"); NextColumn();
					Text("%f / %f (%f%%)", FixedToFloat(&tmp), FixedToFloat(&tmp2), FixedToFloat(&tmp3) * 100); NextColumn();

					(a->*CharaActor::publicMethods.GetSCTimeAmt_FixedPoint)(&tmp);
					(a->*CharaActor::publicMethods.GetSCTimeMax_FixedPoint)(&tmp2);
					(a->*CharaActor::publicMethods.GetSCTimePct_FixedPoint)(&tmp3);
					Text("Super clock:"); NextColumn();
					Text("%f / %f (%f%%)", FixedToFloat(&tmp), FixedToFloat(&tmp2), FixedToFloat(&tmp3) * 100); NextColumn();

					(a->*CharaActor::publicMethods.GetUCTimeAmt_FixedPoint)(&tmp);
					(a->*CharaActor::publicMethods.GetUCTimeMax_FixedPoint)(&tmp2);
					(a->*CharaActor::publicMethods.GetUCTimePct_FixedPoint)(&tmp3);
					Text("Ultra clock:"); NextColumn();
					Text("%f / %f (%f%%)", FixedToFloat(&tmp), FixedToFloat(&tmp2), FixedToFloat(&tmp3) * 100); NextColumn();

					(a->*CharaActor::publicMethods.GetDamage)(&tmp);
					Text("Damage:"); NextColumn();
					Text("%f", FixedToFloat(&tmp)); NextColumn();

					(a->*CharaActor::publicMethods.GetComboDamage)(&tmp);
					Text("Combo damage:"); NextColumn();
					Text("%f", FixedToFloat(&tmp)); NextColumn();

					ImGui::Columns(1);
					ImGui::EndTabItem();
				}
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

// Only 14 bytes are accounted for so far. Training mode memory and
// reload buttons don't seem to be in this specific buffer- which in
// a way makes sense, as this generally seems to be only "action"
// related buttons.
const char* GetButtonLabel(unsigned int bytePosition) {
	switch (bytePosition) {
	case 0: // 0x1
		return "Directions-Off";
	case 1: // 0x2
		return "Up";
	case 2: // 0x4
		return "Down";
	case 3: // 0x8
		return "Away";
	case 4: // 0x10
		return "Towards";
	case 5: // 0x20
		return "Buttons-Off";
	case 6: // 0x40
		return "Jab";
	case 7: // 0x80
		return "Strong";
	case 8: // 0x100
		return "Fierce";
	case 9: // 0x200
		return "Short";
	case 10: // 0x400
		return "Forward";
	case 11: // 0x800
		return "Roundhouse";
	case 12: // 0x1000
		return "Start";
	case 13: // 0x2000
		return "Select/Back";
	default:
		return "Unknown";
	}
}

void DrawCommandWindow(bool* pOpen) {
	Begin(
		"Command",
		pOpen,
		ImGuiWindowFlags_None
	);

	System* system = System::staticMethods.GetSingleton();	
	int isFight = (system->*System::publicMethods.IsFight)();
	Text("Is fight: %d", isFight);
	ImGui::Separator();

	if (isFight) {
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("Input Type", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("On")) {
				ImGui::Columns(3);
				Text("Button"); NextColumn(); Text("P1"); NextColumn(); Text("P2"); NextColumn();
				ImGui::Separator();

				CommandUnit* u = (CommandUnit*)(system->*System::publicMethods.GetUnitByIndex)(System::U_COMMAND);
				DWORD padData[2];
				for (int i = 0; i < 2; i++) {
					CommandImpl* impl = (u->*CommandUnit::publicMethods.GetCommandImplForEntry)(i);
					padData[i] = (impl->*CommandImpl::publicMethods.GetCurrentOnSwitches)();
				}

				for (unsigned int bytePosition = 0; bytePosition < 14; bytePosition++) {
					Text("%s (B%d)", GetButtonLabel(bytePosition), bytePosition); NextColumn();
					for (int i = 0; i < 2; i++) {
						Text((padData[i] & (1 << bytePosition)) ? "ON" : "OFF"); NextColumn();
					}
					ImGui::Separator();
				}

				ImGui::Columns(1);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Rising")) {
				ImGui::Columns(3);
				Text("Button"); NextColumn(); Text("P1"); NextColumn(); Text("P2"); NextColumn();
				ImGui::Separator();

				CommandUnit* u = (CommandUnit*)(system->*System::publicMethods.GetUnitByIndex)(System::U_COMMAND);
				DWORD padData[2];
				for (int i = 0; i < 2; i++) {
					CommandImpl* impl = (u->*CommandUnit::publicMethods.GetCommandImplForEntry)(i);
					padData[i] = (impl->*CommandImpl::publicMethods.GetCurrentRisingSwitches)();
				}

				for (unsigned int bytePosition = 0; bytePosition < 14; bytePosition++) {
					Text("%s (B%d)", GetButtonLabel(bytePosition), bytePosition); NextColumn();
					for (int i = 0; i < 2; i++) {
						Text((padData[i] & (1 << bytePosition)) ? "ON" : "OFF"); NextColumn();
					}
					ImGui::Separator();
				}

				ImGui::Columns(1);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Falling")) {
				ImGui::Columns(3);
				Text("Button"); NextColumn(); Text("P1"); NextColumn(); Text("P2"); NextColumn();
				ImGui::Separator();

				CommandUnit* u = (CommandUnit*)(system->*System::publicMethods.GetUnitByIndex)(System::U_COMMAND);
				DWORD padData[2];
				for (int i = 0; i < 2; i++) {
					CommandImpl* impl = (u->*CommandUnit::publicMethods.GetCommandImplForEntry)(i);
					padData[i] = (impl->*CommandImpl::publicMethods.GetCurrentFallingSwitches)();
				}

				for (unsigned int bytePosition = 0; bytePosition < 14; bytePosition++) {
					Text("%s (B%d)", GetButtonLabel(bytePosition), bytePosition); NextColumn();
					for (int i = 0; i < 2; i++) {
						Text((padData[i] & (1 << bytePosition)) ? "ON" : "OFF"); NextColumn();
					}
					ImGui::Separator();
				}

				ImGui::Columns(1);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	End();
}

void DrawHelpWindow(bool* pOpen) {
	ImGui::Begin(
		"ImGui Help",
		pOpen,
		ImGuiWindowFlags_None
	);

	ImGui::ShowUserGuide();

	ImGui::End();
}

void DrawMementoWindow(bool* pOpen) {
	ImGui::Begin(
		"Memento: Save/load",
		pOpen,
		ImGuiWindowFlags_None
	);

	if (ImGui::Button("Record all mementos")) {
		System* system = System::staticMethods.GetSingleton();
		(system->*System::publicMethods.RecordAllToInternalMementoKeys)();
	}

	if (ImGui::Button("Restore all mementos")) {
		System* system = System::staticMethods.GetSingleton();
		(system->*System::publicMethods.RestoreAllFromInternalMementoKeys)();
	}

	if (ImGui::Button("Manual: Record all mementos")) {
		GameMementoKey::MementoID id = { 1, 1 };
		System* system = System::staticMethods.GetSingleton();
		void* (System:: * GetUnitByIndex)(unsigned int) = System::publicMethods.GetUnitByIndex;
		BOOL canSave = (
			((CharaUnit*)(system->*System::publicMethods.GetUnitByIndex)(System::U_CHARA))->*
			CharaUnit::publicMethods.CanStoreMemento_MaybeActorExists
		)();

		if (canSave) {
			(system->*System::publicMethods.RecordToInternalMementoKey)(&id);

			(
				((CameraUnit*)(system->*GetUnitByIndex)(System::U_CAMERA))->*
				CameraUnit::publicMethods.RecordToInternalMementoKey
			)(&id);

			(
				((CharaUnit*)(system->*GetUnitByIndex)(System::U_CHARA))->*
				CharaUnit::publicMethods.RecordToInternalMementoKey
			)(&id);

			(
				((CommandUnit*)(system->*GetUnitByIndex)(System::U_COMMAND))->*
				CommandUnit::publicMethods.RecordToInternalMementoKey
			)(&id);

			(
				((EffectUnit*)(system->*GetUnitByIndex)(System::U_EFFECT))->*
				EffectUnit::publicMethods.RecordToInternalMementoKey
			)(&id);

			(
				((HudUnit*)(system->*GetUnitByIndex)(System::U_HUD))->*
				HudUnit::publicMethods.RecordToInternalMementoKey
			)(&id);

			(
				((VfxUnit*)(system->*GetUnitByIndex)(System::U_VFX))->*
				VfxUnit::publicMethods.RecordToInternalMementoKey
			)(&id);

			(
				TrainingManager::staticMethods.GetSingleton()->*
				TrainingManager::publicMethods.RecordToInternalMementoKey
			)(&id);
		}
	}

	if (ImGui::Button("Manual: Restore all mementos")) {
		GameMementoKey::MementoID id = { 1, 1 };
		System* system = System::staticMethods.GetSingleton();
		void* (System::* GetUnitByIndex)(unsigned int) = System::publicMethods.GetUnitByIndex;
		(system->*System::publicMethods.RestoreFromInternalMementoKey)(&id);

		(
			((CameraUnit*)(system->*GetUnitByIndex)(System::U_CAMERA))->*
			CameraUnit::publicMethods.RestoreFromInternalMementoKey
		)(&id);

		(
			((CharaUnit*)(system->*GetUnitByIndex)(System::U_CHARA))->*
			CharaUnit::publicMethods.RestoreFromInternalMementoKey
		)(&id);

		(
			((CommandUnit*)(system->*GetUnitByIndex)(System::U_COMMAND))->*
			CommandUnit::publicMethods.RestoreFromInternalMementoKey
		)(&id);

		(
			((EffectUnit*)(system->*GetUnitByIndex)(System::U_EFFECT))->*
			EffectUnit::publicMethods.RestoreFromInternalMementoKey
		)(&id);

		(
			((HudUnit*)(system->*GetUnitByIndex)(System::U_HUD))->*
			HudUnit::publicMethods.RestoreFromInternalMementoKey
		)(&id);

		(
			((VfxUnit*)(system->*GetUnitByIndex)(System::U_VFX))->*
			VfxUnit::publicMethods.RestoreFromInternalMementoKey
		)(&id);

		(
			TrainingManager::staticMethods.GetSingleton()->*
			TrainingManager::publicMethods.RestoreFromInternalMementoKey
		)(&id);
	}

	ImGui::End();
}

void DrawTaskWindow(bool* pOpen) {
	ImGui::Begin(
		"Tasks",
		pOpen,
		ImGuiWindowFlags_None
	);

	int numActiveCores = TaskCoreRegistry::staticMethods.GetNumActiveCores();
	Text("Number of active task cores: %d", numActiveCores);
	for (int i = 0; i < numActiveCores; i++) {
		TaskCore* core = TaskCoreRegistry::staticMethods.GetCoreByIndex(i);
		char* name = (core->*TaskCore::publicMethods.GetName)();
		Text("Core ID: %d, name: %s", i, name);
	}

	ImGui::End();
}

void InitializeOverlay(HWND hWnd, IDirect3DDevice9* lpDevice) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = DEFAULT_ALPHA;
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(lpDevice);
}

void DrawOverlay() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::IsMousePosValid() && ImGui::GetIO().MousePos.y < 200) {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Eva")) {
				if (ImGui::MenuItem("Tasks")) {
					show_task_window = true;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Event")) {
				ImGui::MenuItem("Allow Update", NULL, &fEventController::bUpdateAllowed);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Game")) {
				if (ImGui::MenuItem("Mementos")) {
					show_memento_window = true;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Battle System")) {
				if (ImGui::MenuItem("Overview")) {
					show_battle_system_window = true;
				}

				if (ImGui::MenuItem("Command")) {
					show_command_window = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Imgui")) {
				if (ImGui::MenuItem("Demo")) {
					show_demo_window = true;
				}

				if (ImGui::MenuItem("Help")) {
					show_help_window = true;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	if (show_battle_system_window) {
		DrawBattleSystemWindow(&show_battle_system_window);
	}

	if (show_command_window) {
		DrawCommandWindow(&show_command_window);
	}

	if (show_demo_window) {
		ImGui::ShowDemoWindow();
	}

	if (show_help_window) {
		DrawHelpWindow(&show_help_window);
	}

	if (show_memento_window) {
		DrawMementoWindow(&show_memento_window);
	}

	if (show_task_window) {
		DrawTaskWindow(&show_task_window);
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void FreeOverlay() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

LRESULT WINAPI OverlayWindowFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}
