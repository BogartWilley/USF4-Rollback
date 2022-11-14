#include <windows.h>

#include "Dimps__Event.hxx"

namespace Event = Dimps::Event;
using Event::EventBase;
using Event::EventController;

EventController::__publicMethods EventController::publicMethods;

void Event::Locate(HMODULE peRoot) {
	EventController::Locate(peRoot);
}

char* EventBase::GetName(EventBase* e) {
	return (char*)((unsigned int)e + 0xc);
}

EventController** EventBase::GetSourceController(EventBase* e) {
	return (EventController**)((unsigned int)e + 0x8);
}

void EventController::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.CreateEventWithFlow = (PVOID)(peRootOffset + 0x2aab50);
	*(PVOID*)&publicMethods.QueueEvent = (PVOID)(peRootOffset + 0x2ab3b0);
	*(PVOID*)&publicMethods.RunUpdate = (PVOID)(peRootOffset + 0x2aac80);
}
