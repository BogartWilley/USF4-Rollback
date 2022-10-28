#include <windows.h>
#include <detours/detours.h>

#include "Dimps__Event.hxx"
#include "sf4e__Event.hxx"

namespace rEvent = Dimps::Event;
namespace fEvent = sf4e::Event;
using rEventController = rEvent::EventController;
using fEventController = fEvent::EventController;

bool fEventController::bHaltAfterNext = false;
bool fEventController::bUpdateAllowed = true;

void fEvent::Install() {
	fEventController::Install();
}

void fEventController::Install() {
	void (fEventController:: * _fRunUpdate)() = &RunUpdate;
	DetourAttach((PVOID*)&rEventController::publicMethods.RunUpdate, *(PVOID*)&_fRunUpdate);
}

void fEventController::RunUpdate() {
	if (bUpdateAllowed) {
		(this->*rEventController::publicMethods.RunUpdate)();

		if (bHaltAfterNext) {
			bHaltAfterNext = false;
			bUpdateAllowed = false;
		}
	}
}