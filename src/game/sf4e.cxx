#include "sf4e.hxx"
#include "sf4e__Event.hxx"
#include "sf4e__Game__Battle__System.hxx"
#include "sf4e__GameEvents.hxx"
#include "sf4e__Platform.hxx"

void sf4e::Install() {
	Event::Install();
	Game::Battle::System::Install();
	GameEvents::Install();
	Platform::Install();
}