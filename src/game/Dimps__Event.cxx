#include <windows.h>

#include "Dimps__Event.hxx"

namespace Event = Dimps::Event;
using Event::EventController;

EventController::__publicMethods EventController::publicMethods;

void Event::Locate(HMODULE peRoot) {
	EventController::Locate(peRoot);
}

void EventController::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.RunUpdate = (PVOID)(peRootOffset + 0x2aac80);
}