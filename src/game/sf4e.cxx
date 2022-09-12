#include "sf4e.hxx"
#include "sf4e__Event.hxx"
#include "sf4e__Game.hxx"
#include "sf4e__GameEvents.hxx"
#include "sf4e__Pad.hxx"
#include "sf4e__Platform.hxx"
#include "sf4e__UserApp.hxx"

void sf4e::Install() {
	Event::Install();
	Game::Install();
	GameEvents::Install();
	Pad::Install();
	Platform::Install();
	UserApp::Install();
}