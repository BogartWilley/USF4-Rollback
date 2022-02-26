#include "Dimps__GameEvents__RootEvent.hxx"
#include "sf4e__GameEvents__RootEvent.hxx"

using fRootEvent = sf4e::GameEvents::RootEvent;
using rRootEvent = Dimps::GameEvents::RootEvent;

char* fRootEvent::eventFlowDescription = R"(	Boot, 0, Title,										
LogoCapcom, 0, LogoNvidia, BLACK, 10.0f, BLACK, 10.0f			
tLogoCapcom, 1, Title, BLACK, 10.0f, BLACK, 10.0f			
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

void fRootEvent::Install() {
    *rRootEvent::eventFlowDefinition = fRootEvent::eventFlowDescription;
}
