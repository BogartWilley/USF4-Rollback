#include <random>
#include <time.h>

#include "sf4e.hxx"
#include "sf4e__Event.hxx"
#include "sf4e__Game.hxx"
#include "sf4e__GameEvents.hxx"
#include "sf4e__Pad.hxx"
#include "sf4e__Platform.hxx"
#include "sf4e__UserApp.hxx"

std::mt19937 sf4e::localRand;

void sf4e::Install() {
	localRand.seed(time(NULL));

	Event::Install();
	Game::Install();
	GameEvents::Install();
	Pad::Install();
	Platform::Install();
	UserApp::Install();
}