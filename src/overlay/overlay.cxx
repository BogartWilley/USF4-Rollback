#include "overlay.h"

#include <algorithm>
#include <vector>
#include <windows.h>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_demo.cpp>

#include "../game/Dimps.hxx"
#include "../game/Dimps__Eva.hxx"
#include "../game/Dimps__Game.hxx"
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
#include "../game/Dimps__GameEvents.hxx"
#include "../game/Dimps__Math.hxx"
#include "../game/Dimps__Pad.hxx"

#include "../game/sf4e__Event.hxx"
#include "../game/sf4e__Game.hxx"
#include "../game/sf4e__GameEvents.hxx"
#include "../game/sf4e__Game__Battle__System.hxx"
#include "../game/sf4e__Game__Battle__Vfx.hxx"
#include "../game/sf4e__Pad.hxx"


#define DEFAULT_ALPHA 0.87f

namespace rVfx = Dimps::Game::Battle::Vfx;

using CameraUnit = Dimps::Game::Battle::Camera::Unit;
using CharaActor = Dimps::Game::Battle::Chara::Actor;
using CharaUnit = Dimps::Game::Battle::Chara::Unit;
using CommandUnit = Dimps::Game::Battle::Command::Unit;
using EffectUnit = Dimps::Game::Battle::Effect::Unit;
using HudUnit = Dimps::Game::Battle::Hud::Unit;
using TrainingManager = Dimps::Game::Battle::Training::Manager;
using PadSystem = Dimps::Pad::System;
using VfxUnit = rVfx::Unit;

using Dimps::Eva::Task;
using Dimps::Eva::TaskCore;
using Dimps::Eva::TaskCoreRegistry;
using Dimps::Game::Battle::Command::CommandImpl;
using Dimps::Game::Battle::GameManager;
using Dimps::Game::Battle::System;
using Dimps::Game::Battle::Vfx::ColorFade;
using Dimps::Game::Battle::Vfx::ColorFadeUnit;
using Dimps::Game::GameMementoKey;
using Dimps::GameEvents::VsCharaSelect;
using Dimps::Math::FixedPoint;
using Dimps::Math::FixedToFloat;

using fEventController = sf4e::Event::EventController;
using fKey = sf4e::Game::GameMementoKey;
using fSystem = sf4e::Game::Battle::System;
using fColorFade = sf4e::Game::Battle::Vfx::ColorFade;
using fVsCharaSelect = sf4e::GameEvents::VsCharaSelect;
using fPadSystem = sf4e::Pad::System;

using ImGui::Begin;
using ImGui::BeginMainMenuBar;
using ImGui::BeginMenu;
using ImGui::BeginTabBar;
using ImGui::BeginTabItem;
using ImGui::Button;
using ImGui::CheckboxFlags;
using ImGui::Columns;
using ImGui::End;
using ImGui::EndFrame;
using ImGui::EndMainMenuBar;
using ImGui::EndTabBar;
using ImGui::EndTabItem;
using ImGui::MenuItem;
using ImGui::NewFrame;
using ImGui::NextColumn;
using ImGui::Separator;
using ImGui::Text;

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool show_chara_window = false;
static bool show_command_window = false;
static bool show_demo_window = false;
static bool show_event_window = false;
static bool show_help_window = false;
static bool show_log_window = false;
static bool show_memento_window = false;
static bool show_pad_window = false;
static bool show_system_window = false;
static bool show_task_window = false;
static bool show_vfx_window = false;
static bool show_vscharaselect_window = false;
static int nExtraFramesToSimulate = 1;

// Defined by the ImGui demo.
static ExampleAppLog debugLog;

void DrawCharaWindow(bool* pOpen) {
	Begin(
		"Chara",
		pOpen,
		ImGuiWindowFlags_None
	);

	FixedPoint tmp, tmp2, tmp3;
	System* system = System::staticMethods.GetSingleton();
	GameManager* manager = (system->*System::publicMethods.GetGameManager)();

	int isFight = (system->*System::publicMethods.IsFight)();
	Text("Is fight: %d", isFight);
	Text("Is leaving battle: %d", (system->*System::publicMethods.IsLeavingBattle)());
	if (manager != NULL) {
		(manager->*GameManager::publicMethods.GetRoundTime)(&tmp);
		Text("Round time: %f", FixedToFloat(&tmp));
	}

	if (isFight) {
		CharaUnit* lpCharaUnit = (system->*System::publicMethods.GetCharaUnit)();

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (BeginTabBar("Battle Actor", tab_bar_flags))
		{
			for (int i = 0; i < 2; i++) {
				CharaActor* a = (lpCharaUnit->*CharaUnit::publicMethods.GetActorByIndex)(i);

				if (BeginTabItem(i == 0 ? "Actor 0" : "Actor 1")) {
					Columns(2, NULL, false);

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

					Columns(1);
					EndTabItem();
				}
			}

			EndTabBar();
		}
	}

	End();
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
	Separator();

	if (isFight) {
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (BeginTabBar("Input Type", tab_bar_flags))
		{
			if (BeginTabItem("On")) {
				Columns(3);
				Text("Button"); NextColumn(); Text("P1"); NextColumn(); Text("P2"); NextColumn();
				Separator();

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
					Separator();
				}

				Columns(1);
				EndTabItem();
			}

			if (BeginTabItem("Rising")) {
				Columns(3);
				Text("Button"); NextColumn(); Text("P1"); NextColumn(); Text("P2"); NextColumn();
				Separator();

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
					Separator();
				}

				Columns(1);
				EndTabItem();
			}

			if (BeginTabItem("Falling")) {
				Columns(3);
				Text("Button"); NextColumn(); Text("P1"); NextColumn(); Text("P2"); NextColumn();
				Separator();

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
					Separator();
				}

				Columns(1);
				EndTabItem();
			}

			EndTabBar();
		}
	}

	End();
}

void DrawEventWindow(bool* pOpen) {
	Begin(
		"Event",
		pOpen,
		ImGuiWindowFlags_None
	);

	Text("Halt after next: %s", fEventController::bHaltAfterNext ? "true" : "false");
	Text("Update allowed: %s", fEventController::bUpdateAllowed ? "true" : "false");
	if (Button("Pause")) {
		fEventController::bUpdateAllowed = false;
	}
	if (Button("Play")) {
		fEventController::bUpdateAllowed = true;
	}
	if (Button("Halt after next")) {
		fEventController::bHaltAfterNext = true;
	}
	if (Button("Step")) {
		fEventController::bUpdateAllowed = true;
		fEventController::bHaltAfterNext = true;
	}

	End();
}

void DrawPadTable(DWORD* padData) {
	Columns(3);
	Text("Button"); NextColumn(); Text("P1"); NextColumn(); Text("P2"); NextColumn();
	Separator();

	for (unsigned int bytePosition = 0; bytePosition < 14; bytePosition++) {
		Text("B%d", bytePosition); NextColumn();
		for (int i = 0; i < 2; i++) {
			Text((padData[i] & (1 << bytePosition)) ? "ON" : "OFF"); NextColumn();
		}
		Separator();
	}
	Columns(1);
}

void DrawPadWindow(bool* pOpen) {
	Begin(
		"Pad",
		pOpen,
		ImGuiWindowFlags_None
	);

	PadSystem* p = PadSystem::staticMethods.GetSingleton();
	DWORD padData[2];

	if (BeginTabBar("Pad types", ImGuiTabBarFlags_None)) {
		if (BeginTabItem("On")) {
			for (int i = 0; i < 2; i++) {
				padData[i] = (p->*PadSystem::publicMethods.GetButtons_On)(i);
			}

			DrawPadTable(padData);
			EndTabItem();
		}

		if (BeginTabItem("Rising")) {
			for (int i = 0; i < 2; i++) {
				padData[i] = (p->*PadSystem::publicMethods.GetButtons_Rising)(i);
			}

			DrawPadTable(padData);
			EndTabItem();
		}

		if (BeginTabItem("Falling")) {
			for (int i = 0; i < 2; i++) {
				padData[i] = (p->*PadSystem::publicMethods.GetButtons_Falling)(i);
			}

			DrawPadTable(padData);
			EndTabItem();
		}

		if (BeginTabItem("Repeat")) {
			for (int i = 0; i < 2; i++) {
				padData[i] = (p->*PadSystem::publicMethods.GetButtons_RisingWithRepeat)(i);
			}

			DrawPadTable(padData);
			EndTabItem();
		}

		if (BeginTabItem("Mapped")) {
			for (int i = 0; i < 2; i++) {
				padData[i] = (p->*PadSystem::publicMethods.GetButtons_Mapped)(i);
			}

			DrawPadTable(padData);
			EndTabItem();
		}

		EndTabBar();
	}

	End();
}

bool compareTasks(Task* a, Task* b) {
	return Task::GetPriority(a) < Task::GetPriority(b);
}

void DrawSystemTaskPanel(TaskCore* core) {
	if (core == NULL) {
		Text("Core not yet allocated");
		return;
	}

	Task** taskCursor = TaskCore::GetTaskBuffer(core);
	int taskCount = (core->*TaskCore::publicMethods.GetNumUsed)();
	std::vector<Task*> taskVec(taskCursor, taskCursor + taskCount);
	std::sort(taskVec.begin(), taskVec.end(), compareTasks);

	Columns(2);
	Text("Priority"); NextColumn(); Text("Name"); NextColumn();
	for (auto taskIter = taskVec.begin(); taskIter != taskVec.end(); taskIter++) {
		Task* t = *taskIter;
		Text("%x", Task::GetPriority(t)); NextColumn();
		Text("%s", (core->*TaskCore::publicMethods.GetTaskName)(&t)); NextColumn();
	}
	Columns(1);
}

void DrawSystemWindow(bool* pOpen) {
	static int selectedForwardSimFrame = 0;

	Begin(
		"System",
		pOpen,
		ImGuiWindowFlags_None
	);

	System* system = System::staticMethods.GetSingleton();
	System::__publicMethods& methods = System::publicMethods;
	int isFight = (system->*System::publicMethods.IsFight)();

	if (BeginTabBar("System tabs", ImGuiTabBarFlags_None)) {
		if (BeginTabItem("Simulation control")) {
			Text("Halt after next: %s", fSystem::bHaltAfterNext ? "true" : "false");
			Text("Update allowed: %s", fSystem::bUpdateAllowed ? "true" : "false");
			if (Button("Pause")) {
				fSystem::bUpdateAllowed = false;
			}
			if (Button("Play")) {
				fSystem::bUpdateAllowed = true;
			}
			if (Button("Halt after next")) {
				fSystem::bHaltAfterNext = true;
			}
			if (Button("Step")) {
				fSystem::bUpdateAllowed = true;
				fSystem::bHaltAfterNext = true;
			}

			EndTabItem();
		}

		if (BeginTabItem("Forward Simulation")) {
			ImGui::InputInt("Num frames to skip", &nExtraFramesToSimulate);
			if (nExtraFramesToSimulate < 1) {
				nExtraFramesToSimulate = 1;
			}
			if (nExtraFramesToSimulate > fPadSystem::PLAYBACK_MAX) {
				nExtraFramesToSimulate = fPadSystem::PLAYBACK_MAX;
			}
			if (Button("Simulate")) {
				fSystem::nExtraFramesToSimulate = nExtraFramesToSimulate;
			}
			for (int p = 0; p < 2; p++) {
				const char* headerLabel = p == 0 ? "P1 inputs during skip" : "P2 inputs during skip";
				if (ImGui::CollapsingHeader(headerLabel)) {
					ImGui::BeginChild("left pane", ImVec2(150, 0), true);
					for (int i = 0; i < nExtraFramesToSimulate; i++)
					{
						char label[128];
						sprintf(label, "Frame %d", i);
						if (ImGui::Selectable(label, selectedForwardSimFrame == i)) {
							selectedForwardSimFrame = i;
						}
					}
					ImGui::EndChild();
					ImGui::SameLine();

					ImGui::BeginChild("right pane");
					unsigned int* padData = &fPadSystem::playbackData[p][selectedForwardSimFrame];
					CheckboxFlags("Up", padData, 0x1); NextColumn();
					CheckboxFlags("Down", padData, 0x2); NextColumn();
					CheckboxFlags("Left", padData, 0x4); NextColumn();
					CheckboxFlags("Right", padData, 0x8); NextColumn();
					CheckboxFlags("LP", padData, 0x10); NextColumn();
					CheckboxFlags("MP", padData, 0x20); NextColumn();
					CheckboxFlags("LK", padData, 0x40); NextColumn();
					CheckboxFlags("MK", padData, 0x80); NextColumn();
					CheckboxFlags("HP", padData, 0x400); NextColumn();
					CheckboxFlags("HK", padData, 0x800); NextColumn();
					ImGui::EndChild();
				}
			}
			EndTabItem();
		}

		if (BeginTabItem("Update tasks")) {
			TaskCore* updateCore = (system->*methods.GetTaskCore)(System::TCI_UPDATE);
			DrawSystemTaskPanel(updateCore);
			EndTabItem();
		}

		if (BeginTabItem("Render tasks")) {
			TaskCore* renderCore = (system->*methods.GetTaskCore)(System::TCI_RENDER);
			DrawSystemTaskPanel(renderCore);
			EndTabItem();
		}
		EndTabBar();
	}

	End();
}

void DrawVfxWindow(bool* pOpen) {
	Begin(
		"Vfx",
		pOpen,
		ImGuiWindowFlags_None
	);

	System* system = System::staticMethods.GetSingleton();
	int isFight = (system->*System::publicMethods.IsFight)();
	if (!isFight) {
		Text("Is fight: %d", isFight);
		End();
		return;
	}

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	VfxUnit* unit = (VfxUnit*)(system->*System::publicMethods.GetUnitByIndex)(System::U_VFX);
	rVfx::IContainer* (VfxUnit:: * GetContainerByType)(DWORD) = VfxUnit::publicMethods.GetContainerByType;
	rVfx::ObjectContainer* cObject = (rVfx::ObjectContainer*)(unit->*GetContainerByType)(VfxUnit::CT_OBJECT);
	rVfx::ParticleContainer* cParticle = (rVfx::ParticleContainer*)(unit->*GetContainerByType)(VfxUnit::CT_PARTICLE);
	rVfx::TraceContainer* cTrace = (rVfx::TraceContainer*)(unit->*GetContainerByType)(VfxUnit::CT_TRACE);
	ColorFadeUnit* colorFadeUnit = ColorFadeUnit::staticMethods.GetSingleton();

	if (isFight) {
		if (BeginTabBar("Container Type", tab_bar_flags))
		{
			if (BeginTabItem("Object")) {

				if (BeginTabBar("Object Type", tab_bar_flags)) {
					if (BeginTabItem("Reserved")) {
						Columns(2, NULL, false);

						for (unsigned int i = 0; i < rVfx::ObjectContainer::RESERVED_OBJECT_COUNT; i++) {
							DWORD handle = rVfx::ObjectContainer::GenerateFakeHandle(i, true);
							rVfx::Object* o = (cObject->*rVfx::ObjectContainer::publicMethods.GetObjectFromHandle)(handle);
							Text("Object %d:", i); NextColumn();
							if (o) {
								Text("%x , Name: %s", (unsigned int)o, rVfx::Object::GetNameTmp(o)->c_str());
								NextColumn();
							}
							else {
								Text("DEAD"); NextColumn();
							}
						}

						Columns(1);
						EndTabItem();
					}
					if (BeginTabItem("Loose")) {
						Columns(2, NULL, false);

						for (unsigned int i = 0; i < rVfx::ObjectContainer::DEFAULT_LOOSE_OBJECT_COUNT; i++) {
							DWORD handle = rVfx::ObjectContainer::GenerateFakeHandle(i, false);
							rVfx::Object* o = (cObject->*rVfx::ObjectContainer::publicMethods.GetObjectFromHandle)(handle);
							Text("Object %d:", i); NextColumn();
							if (o) {
								Text("%x , Name: %s", (unsigned int)o, rVfx::Object::GetNameTmp(o)->c_str());
								NextColumn();
							}
							else {
								Text("DEAD"); NextColumn();
							}
						}

						Columns(1);
						EndTabItem();
					}
					EndTabBar();
				}

				EndTabItem();
			}

			if (BeginTabItem("Particle")) {
				Columns(2, NULL, false);

				for (unsigned int i = 0; i < rVfx::ParticleContainer::DEFAULT_PARTICLE_COUNT; i++) {
					DWORD handle = rVfx::ParticleContainer::GenerateFakeHandle(i);
					rVfx::Particle* p = (cParticle->*rVfx::ParticleContainer::publicMethods.GetParticleFromHandle)(handle);
					Text("Particle %d:", i); NextColumn();
					if (p) {
						Text("%x , Name: %s", (unsigned int)p, rVfx::Particle::GetNameTmp(p)->c_str());
						NextColumn();
					}
					else {
						Text("DEAD"); NextColumn();
					}
				}

				Columns(1);
				EndTabItem();
			}

			if (BeginTabItem("Trace")) {
				Columns(2, NULL, false);

				for (unsigned int i = 0; i < rVfx::TraceContainer::DEFAULT_TRACE_COUNT; i++) {
					DWORD handle = rVfx::TraceContainer::GenerateFakeHandle(i);
					rVfx::Trace* t = (cTrace->*rVfx::TraceContainer::publicMethods.GetTraceFromHandle)(handle);
					Text("Trace %d:", i); NextColumn();
					if (t) {
						Text("%x , Name: %s", (unsigned int)t, rVfx::Trace::GetNameTmp(t)->c_str());
						NextColumn();
					}
					else {
						Text("DEAD"); NextColumn();
					}
				}

				Columns(1);
				EndTabItem();
			}

			if (BeginTabItem("ColorFade")) {
				Text("Highest observed ColorFadeData count: %d", fColorFade::HIGHEST_OBSERVED_FADES);
				ImGui::SameLine();
				if (Button("Clear")) {
					fColorFade::HIGHEST_OBSERVED_FADES = 0;
				}

				Columns(2, NULL, false);

				for (unsigned int i = 0; i < 2; i++) {
					ColorFade* fade = (colorFadeUnit->*ColorFadeUnit::publicMethods.GetFade)(i);
					auto fadeList = ColorFade::GetList(fade);
					Text("Size %d:", fadeList->numUsed); NextColumn(); NextColumn();

					int j = 0;
					for (auto iter = fadeList->root->next; iter != fadeList->root; iter = iter->next) {
						Text("  Data @ index %d:", j); NextColumn(); NextColumn();
						Text("    Resource offset: %p", iter->data.resourcePtr); NextColumn();
						Text("    Flags: %x", iter->data.flags); NextColumn();
						j++;
					}
				}

				Columns(1);
				EndTabItem();
			}
			EndTabBar();
		}
	}

	End();
}

void DrawVsCharaPlayerPanel(VsCharaSelect::PlayerConditions* c) {
	Text("Last selected character: %s", VsCharaSelect::PlayerConditions::GetSelectedCharaAbbrev(c));
	Text("Current hovered character: %s", VsCharaSelect::PlayerConditions::GetHoveredCharaAbbrev(c));
	Text("Color: %d", *VsCharaSelect::PlayerConditions::GetColor(c));
	Text("Costume: %d", *VsCharaSelect::PlayerConditions::GetCostume(c));
	Text("Personal action: %d", *VsCharaSelect::PlayerConditions::GetPersonalAction(c));
	Text("Win quote: %d", *VsCharaSelect::PlayerConditions::GetWinQuote(c));
	Text("Edition: %d", *VsCharaSelect::PlayerConditions::GetEdition(c));
	Text("Ultra combo: %d", *VsCharaSelect::PlayerConditions::GetUltraCombo(c));
	Text("Handicap: %d", *VsCharaSelect::PlayerConditions::GetHandicap(c));
}

void DrawVsCharaSelectWindow(bool* pOpen) {
	Begin(
		"VsCharaSelect",
		pOpen,
		ImGuiWindowFlags_None
	);

	if (fVsCharaSelect::instance == NULL) {
		Text("No instance");
		End();
		return;
	}


	VsCharaSelect::CharaSelectState* state = VsCharaSelect::GetState(fVsCharaSelect::instance);
	Text("Instance: %p", fVsCharaSelect::instance);
	Text("Flags: %x", state->flags);
	if (BeginTabBar("VsCharaSelect tabs", ImGuiTabBarFlags_None)) {
		if (BeginTabItem("Player 1")) {
			DrawVsCharaPlayerPanel(&state->playerConditions[0]);
			EndTabItem();
		}

		if (BeginTabItem("Player 2")) {
			DrawVsCharaPlayerPanel(&state->playerConditions[1]);
			EndTabItem();
		}
		EndTabBar();
	}

	End();
}

void DrawHelpWindow(bool* pOpen) {
	Begin(
		"ImGui Help",
		pOpen,
		ImGuiWindowFlags_None
	);

	ImGui::ShowUserGuide();

	End();
}

void DrawMementoWindow(bool* pOpen) {
	static DWORD targetID = 2;
	Begin(
		"Memento: Save/load",
		pOpen,
		ImGuiWindowFlags_None
	);

	if (BeginTabBar("Memento Tabs", ImGuiTabBarFlags_None)) {
		if (BeginTabItem("Auto-Immediate")) {
			if (Button("Record all to memento 1 immediately")) {
				System* system = System::staticMethods.GetSingleton();
				(system->*System::publicMethods.RecordAllToInternalMementoKeys)();
			}

			if (Button("Restore all from memento 1 immediately")) {
				System* system = System::staticMethods.GetSingleton();
				(system->*System::publicMethods.RestoreAllFromInternalMementoKeys)();
			}

			EndTabItem();
		}

		if (BeginTabItem("Manual")) {
			ImGui::InputInt("Target memento ID", (int*)&targetID);

			Text("Record: ");
			ImGui::SameLine();
			if (Button("Request##Record")) {
				GameMementoKey::MementoID mid = { targetID, targetID };
				fSystem::saveRequest = mid;
			}
			ImGui::SameLine();
			if (Button("Immediate##Record")) {
				GameMementoKey::MementoID mid = { targetID, targetID };
				System* system = System::staticMethods.GetSingleton();
				fSystem::RecordAllToInternalMementos(system, &mid);
			}

			Text("Restore: ");
			ImGui::SameLine();
			if (Button("Request##Restore")) {
				GameMementoKey::MementoID mid = { targetID, targetID };
				fSystem::loadRequest = mid;
			}
			ImGui::SameLine();
			if (Button("Immediate##Restore")) {
				GameMementoKey::MementoID mid = { targetID, targetID };
				System* system = System::staticMethods.GetSingleton();
				fSystem::RestoreAllFromInternalMementos(system, &mid);
			}

			EndTabItem();
		}


		if (BeginTabItem("Debug")) {
			auto keyEnd = fKey::trackedKeys.end();
			int keyIdx = 0;
			for (auto keyIter = fKey::trackedKeys.begin(); keyIter != keyEnd; keyIter++) {
				Text("Key %d: ", keyIdx);
				for (int i = 0; i < (*keyIter)->numMementos; i++) {
					ImGui::SameLine();
					Text("Memento %d: %d, %d", i, (*keyIter)->metadata[i].id.lo, (*keyIter)->metadata[i].id.hi);
				}
				keyIdx++;
			}
			EndTabItem();
		}

		ImGui::EndTabBar();
	}

	End();
}

void DrawTaskWindow(bool* pOpen) {
	Begin(
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

	End();
}

void GameMementoKeySizeLogger(GameMementoKey* k, int oldSize) {
	debugLog.AddLog(
		"GameMementoKey @ %p w/ mementoable %p (vtbl %x) had differing size: Old %d, new %d\n",
		k,
		k->mementoableObject,
		*(DWORD*)k->mementoableObject,
		oldSize,
		k->sizeAllocated
	);
}

void InitializeOverlay(HWND hWnd, IDirect3DDevice9* lpDevice) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = DEFAULT_ALPHA;
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(lpDevice);
	sf4e::Game::GameMementoKey::SizeLogger = GameMementoKeySizeLogger;
}

void DrawOverlay() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();

	if (ImGui::IsMousePosValid() && ImGui::GetIO().MousePos.y < 200) {
		if (BeginMainMenuBar()) {
			if (BeginMenu("Eva")) {
				if (MenuItem("Tasks")) {
					show_task_window = true;
				}
				ImGui::EndMenu();
			}

			if (BeginMenu("Event")) {
				if (MenuItem("Overview")) {
					show_event_window = true;
				}
				ImGui::EndMenu();
			}

			if (BeginMenu("Game")) {
				if (MenuItem("Mementos")) {
					show_memento_window = true;
				}
				ImGui::EndMenu();
			}

			if (BeginMenu("GameEvents")) {
				if (MenuItem("VsCharaSelect")) {
					show_vscharaselect_window = true;
				}
				ImGui::EndMenu();
			}

			if (BeginMenu("Battle")) {
				if (MenuItem("Chara")) {
					show_chara_window = true;
				}

				if (MenuItem("Command")) {
					show_command_window = true;
				}

				if (MenuItem("System")) {
					show_system_window = true;
				}

				if (MenuItem("Vfx")) {
					show_vfx_window = true;
				}

				ImGui::EndMenu();
			}

			if (BeginMenu("Pad")) {
				if (MenuItem("System")) {
					show_pad_window = true;
				}

				ImGui::EndMenu();
			}

			if (BeginMenu("Imgui")) {
				if (MenuItem("Demo")) {
					show_demo_window = true;
				}

				if (MenuItem("Help")) {
					show_help_window = true;
				}

				ImGui::EndMenu();
			}


			if (MenuItem("Log")) {
				show_log_window = true;
			}

			EndMainMenuBar();
		}
	}

	if (show_chara_window) {
		DrawCharaWindow(&show_chara_window);
	}

	if (show_command_window) {
		DrawCommandWindow(&show_command_window);
	}

	if (show_event_window) {
		DrawEventWindow(&show_event_window);
	}

	if (show_log_window) {
		debugLog.Draw("Debug Log", &show_log_window);
	}

	if (show_pad_window) {
		DrawPadWindow(&show_pad_window);
	}

	if (show_system_window) {
		DrawSystemWindow(&show_chara_window);
	}

	if (show_vfx_window) {
		DrawVfxWindow(&show_vfx_window);
	}

	if (show_vscharaselect_window) {
		DrawVsCharaSelectWindow(&show_vscharaselect_window);
	}

	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
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

	EndFrame();
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
