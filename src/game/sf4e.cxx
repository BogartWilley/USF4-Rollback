#include "sf4e.hxx"
#include "sf4e__GameEvents.hxx"
#include "sf4e__Platform.hxx"

void sf4e::Install() {
	sf4e::GameEvents::Install();
	sf4e::Platform::Install();
}