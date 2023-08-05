#include <windows.h>
#include <detours/detours.h>

#include "../Dimps/Dimps.hxx"
#include "../Dimps/Dimps__Event.hxx"
#include "../Dimps/Dimps__Game.hxx"
#include "../Dimps/Dimps__GameEvents.hxx"
#include "../Dimps/Dimps__Platform.hxx"
#include "sf4e__Event.hxx"
#include "sf4e__GameEvents.hxx"

using Dimps::Game::Request;

namespace rGameEvents = Dimps::GameEvents;
using rEventBase = Dimps::Event::EventBase;
using rEventController = Dimps::Event::EventController;
using rMainMenu = rGameEvents::MainMenu;
using rRootEvent = rGameEvents::RootEvent;
using rVsBattle = rGameEvents::VsBattle;
using rVsCharaSelect = rGameEvents::VsCharaSelect;
using rVsMode = rGameEvents::VsMode;
using rVsPreBattle = rGameEvents::VsPreBattle;
using rVsStageSelect = rGameEvents::VsStageSelect;

namespace fGameEvents = sf4e::GameEvents;
using fMainMenu = fGameEvents::MainMenu;
using fRootEvent = fGameEvents::RootEvent;
using fVsBattle = fGameEvents::VsBattle;
using fVsCharaSelect = fGameEvents::VsCharaSelect;
using fVsMode = fGameEvents::VsMode;
using fVsPreBattle = fGameEvents::VsPreBattle;
using fVsStageSelect = fGameEvents::VsStageSelect;

rMainMenu* fMainMenu::instance = nullptr;
rVsMode* fVsMode::instance = nullptr;
void (*fVsPreBattle::OnTasksRegistered)() = nullptr;

bool fVsBattle::bForceNextMatchOnline = false;
bool fVsBattle::bOverrideNextRandomSeed = false;
bool fVsBattle::bTerminateOnNextLeftBattle = false;
DWORD fVsBattle::nextMatchRandomSeed = 0xffffffff;
bool fVsPreBattle::bSkipToVersus = false;

char* fRootEvent::eventFlowDescription = R"(	Boot, 0, Title,										
LogoCapcom, 0, LogoNvidia, BLACK, 10.0f, BLACK, 10.0f			
LogoCapcom, 1, Title, BLACK, 10.0f, BLACK, 10.0f			
	LogoNvidia, 0, LogoDolby, BLACK, 10.0f, BLACK, 10.0f			
	LogoNvidia, 1, Title, BLACK, 10.0f, BLACK, 10.0f			
LogoDolby, 0, LogoCRI, BLACK, 10.0f, BLACK, 10.0f			
	LogoDolby, 1, Title, BLACK, 10.0f, BLACK, 10.0f			
	LogoCRI, 0, LogoScaleform, BLACK, 10.0f, BLACK, 10.0f			
	LogoCRI, 1, Title, BLACK, 10.0f, BLACK, 10.0f			
	LogoScaleform, 0, Opening, BLACK, 10.0f, BLACK, 10.0f			
	LogoScaleform, 1, Title, BLACK, 10.0f, BLACK, 10.0f			
	Title, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	Title, 1, DemoBattle, BLACK, 30.0f, BLACK, 30.0f			
	DemoBattle, 0, LogoCapcom, BLACK, 30.0f, BLACK, 30.0f			
	DemoBattle, 1, Title, BLACK, 30.0f, BLACK, 30.0f			
	Opening, 0, Title, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 0, Title, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 1, ArcadeMode, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 2, VSMode, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 3, NetworkMode, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 4, TrainingMode, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 5, Option, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 6, Marketplace, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 7, PlayerData, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 8, TrialMode, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 9, BonusStageMode1, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 10, BonusStageMode2, BLACK, 30.0f, BLACK, 30.0f			
	MainMenu, 11, PlayerTitleSetting, BLACK, 30.0f, WHITE, 30.0f			
	MainMenu, 12, PlayerIconSetting, BLACK, 30.0f, WHITE, 30.0f			
MainMenu, 14, Benchmark, BLACK, 30.0f, BLACK, 30.0f			
	Benchmark, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
MainMenu, 13, Manual, BLACK, 30.0f, BLACK, 30.0f			
	Manual, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	PlayerTitleSetting, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	PlayerTitleSetting, 1, Option, BLACK, 30.0f, BLACK, 30.0f			
	PlayerTitleSetting, 2, PlayerData, BLACK, 30.0f, BLACK, 30.0f			
	PlayerTitleSetting, 3, NetworkMode, BLACK, 30.0f, BLACK, 30.0f			
	PlayerTitleSetting, 4, PlayerIconSetting, BLACK, 30.0f, BLACK, 30.0f			
	PlayerIconSetting, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	PlayerIconSetting, 1, Option, BLACK, 30.0f, BLACK, 30.0f			
	PlayerIconSetting, 2, PlayerData, BLACK, 30.0f, BLACK, 30.0f			
	PlayerIconSetting, 3, NetworkMode, BLACK, 30.0f, BLACK, 30.0f			
	PlayerIconSetting, 4, PlayerTitleSetting, BLACK, 30.0f, BLACK, 30.0f			
	ArcadeMode, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	VSMode, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	NetworkMode, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	TrainingMode, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	Option, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	Option, 1, PlayerTitleSetting, BLACK, 30.0f, BLACK, 30.0f			
	Option, 2, PlayerIconSetting, BLACK, 30.0f, BLACK, 30.0f			
	Marketplace, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
PlayerData, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	PlayerData, 1, ReplayChannel, BLACK, 30.0f, BLACK, 30.0f	
	PlayerData, 2, PlayerTitleSetting, BLACK, 30.0f, BLACK, 30.0f			
	PlayerData, 3, PlayerIconSetting, BLACK, 30.0f, BLACK, 30.0f			
	PlayerData, 4, LocalBattleLog, BLACK, 30.0f, BLACK, 30.0f			
	LocalBattleLog, 0, PlayerData, BLACK, 30.0f, BLACK, 30.0f			
	LocalBattleLog, 1, MainMenu, BLACK, 30.0f, BLACK, 30.0f	
	ReplayChannel, 0, PlayerData, BLACK, 30.0f, BLACK, 30.0f	
	ReplayChannel, 1, MainMenu, BLACK, 30.0f, BLACK, 30.0f	
	TrialMode, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	RivalBattleMode, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	BossBattleMode, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	BonusStageMode1, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	BonusStageMode2, 0, MainMenu, BLACK, 30.0f, BLACK, 30.0f			
	Signout, 0, Title, BLACK, 30.0f, BLACK, 30.0f			
	StorageNotice, 0, Title, BLACK, 30.0f, BLACK, 30.0f		
)";
rVsCharaSelect* fVsCharaSelect::instance;

bool fVsStageSelect::forceTimerOnNextStageSelect = false;
rVsStageSelect* fVsStageSelect::instance;

void fGameEvents::Install() {
	MainMenu::Install();
	RootEvent::Install();
	VsBattle::Install();
	VsCharaSelect::Install();
	VsMode::Install();
	VsPreBattle::Install();
	VsStageSelect::Install();
}

void fMainMenu::Install() {
	void* (fMainMenu:: * _fDestroy)(DWORD) = &Destroy;
	DetourAttach((PVOID*)&rMainMenu::publicMethods.Destroy, *(PVOID*)&_fDestroy);
	DetourAttach((PVOID*)&rMainMenu::staticMethods.Factory, &Factory);
}

void fMainMenu::GoToVersusBattle() {
	Dimps::Event::EventController* ec = *Dimps::Event::EventBase::GetSourceController(instance);
	(ec->*Dimps::Event::EventController::publicMethods.CreateEventWithFlow)(2, 0, 0, 0, 1);
}

rMainMenu* fMainMenu::Factory(DWORD arg1, DWORD arg2, DWORD arg3) {
	rMainMenu* out = rMainMenu::staticMethods.Factory(arg1, arg2, arg3);
	instance = out;
	return out;
}

void* fMainMenu::Destroy(DWORD arg1) {
	rMainMenu* _this = (rMainMenu*)this;
	if (instance == this) {
		instance = NULL;
	}
	else {
		MessageBox(NULL, TEXT("MainMenu not tracked that was destroyed!"), NULL, MB_OK);
	}

	return (_this->*rMainMenu::publicMethods.Destroy)(arg1);
}

void fRootEvent::Install() {
	*rRootEvent::eventFlowDefinition = fRootEvent::eventFlowDescription;
}

void fVsBattle::Install() {
	int (fVsBattle:: * _fCheckAndMaybeExitBasedOnBattleType)() = &CheckAndMaybeExitBasedOnExitType;
	void (fVsBattle:: * _fPrepareBattleRequest)() = &PrepareBattleRequest;
	DetourAttach(
		(PVOID*)&fVsBattle::privateMethods.CheckAndMaybeExitBasedOnExitType,
		*(PVOID*)&_fCheckAndMaybeExitBasedOnBattleType
	);
	DetourAttach(
		(PVOID*)&fVsBattle::privateMethods.PrepareBattleRequest,
		*(PVOID*)&_fPrepareBattleRequest
	);
}


int fVsBattle::CheckAndMaybeExitBasedOnExitType() {
	if (bTerminateOnNextLeftBattle) {
		bTerminateOnNextLeftBattle = false;
		rEventController* c = *rEventBase::GetSourceController(this);
		(c->*rEventController::publicMethods.EnterTerminalState)(0, 0);
		return 1;
	}

	return (this->*rVsBattle::privateMethods.CheckAndMaybeExitBasedOnExitType)();
}

void fVsBattle::PrepareBattleRequest() {
	(this->*rVsBattle::privateMethods.PrepareBattleRequest)();
	Request* r = *rVsBattle::GetRequest(this);
	if (r) {
		if (bForceNextMatchOnline) {
			(r->*Request::publicMethods.SetIsOnlineBattle)(TRUE);
		}
		if (bOverrideNextRandomSeed) {
			(r->*Request::publicMethods.SetRandomSeed)(nextMatchRandomSeed);
		}
	}
	bForceNextMatchOnline = false;
	bOverrideNextRandomSeed = false;
	nextMatchRandomSeed = 0xffffffff;
}

void fVsCharaSelect::Install() {
	void* (fVsCharaSelect:: * _fDestroy)(DWORD) = &Destroy;
	DetourAttach((PVOID*)&rVsCharaSelect::publicMethods.Destroy, *(PVOID*)&_fDestroy);
	DetourAttach((PVOID*)&rVsCharaSelect::staticMethods.Factory, &Factory);
}

rVsCharaSelect* fVsCharaSelect::Factory(DWORD arg1, DWORD arg2, DWORD arg3) {
	rVsCharaSelect* out = rVsCharaSelect::staticMethods.Factory(arg1, arg2, arg3);
	instance = out;
	return out;
}

void* fVsCharaSelect::Destroy(DWORD arg1) {
	rVsCharaSelect* _this = (rVsCharaSelect*)this;
	if (instance == this) {
		instance = NULL;
	}
	else {
		MessageBox(NULL, TEXT("VsCharaSelect not tracked that was destroyed!"), NULL, MB_OK);
	}
	
	return (_this->*rVsCharaSelect::publicMethods.Destroy)(arg1);
}

void fVsMode::Install() {
	void* (fVsMode:: * _fDestroy)(DWORD) = &Destroy;
	DetourAttach((PVOID*)&rVsMode::publicMethods.Destroy, *(PVOID*)&_fDestroy);
	DetourAttach((PVOID*)&rVsMode::staticMethods.Factory, &Factory);
}

rVsMode* fVsMode::Factory(DWORD arg1, DWORD arg2, DWORD arg3) {
	rVsMode* out = rVsMode::staticMethods.Factory(arg1, arg2, arg3);
	instance = out;
	return out;
}

void* fVsMode::Destroy(DWORD arg1) {
	rVsMode* _this = (rVsMode*)this;
	if (instance == this) {
		instance = NULL;
	}
	else {
		MessageBox(NULL, TEXT("VsMode that was not tracked was destroyed!"), NULL, MB_OK);
	}

	return (_this->*rVsMode::publicMethods.Destroy)(arg1);
}

void fVsPreBattle::Install() {
	void (fVsPreBattle:: * _fRegisterTasks)() = &RegisterTasks;
	DetourAttach((PVOID*)&fVsPreBattle::publicMethods.RegisterTasks, *(PVOID*)&_fRegisterTasks);
}

void fVsPreBattle::RegisterTasks() {
	rVsPreBattle* _this = (rVsPreBattle*)this;
	if (bSkipToVersus) {
		sf4e::Event::EventController::ReplaceNextEvent("VersusFromChr");
		bSkipToVersus = false;
	}
	(_this->*rVsPreBattle::publicMethods.RegisterTasks)();
	if (OnTasksRegistered) {
		OnTasksRegistered();
		OnTasksRegistered = false;
	}
}

void fVsStageSelect::Install() {
	void* (fVsStageSelect::* _fDestroy)(DWORD) = &Destroy;
	DetourAttach((PVOID*)&rVsStageSelect::publicMethods.Destroy, *(PVOID*)&_fDestroy);
	DetourAttach((PVOID*)&rVsStageSelect::staticMethods.Factory, &Factory);
}

rVsStageSelect* fVsStageSelect::Factory(DWORD arg1, DWORD arg2, DWORD arg3) {
	rVsStageSelect* out = rVsStageSelect::staticMethods.Factory(arg1, arg2, arg3);
	instance = out;
	if (forceTimerOnNextStageSelect) {
		rVsStageSelect::GetState(instance)->flags |= StageSelectState::SSSF_TIMER_ENABLED;
	}
	return out;
}

void* fVsStageSelect::Destroy(DWORD arg1) {
	rVsStageSelect* _this = (rVsStageSelect*)this;
	if (instance == this) {
		instance = NULL;
	}
	else {
		MessageBox(NULL, TEXT("VsStageSelect not tracked that was destroyed!"), NULL, MB_OK);
	}
	
	return (_this->*rVsStageSelect::publicMethods.Destroy)(arg1);
}
