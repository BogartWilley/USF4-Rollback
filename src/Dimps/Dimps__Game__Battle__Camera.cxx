#include <windows.h>

#include "Dimps__Game__Battle__Camera.hxx"

namespace Camera = Dimps::Game::Battle::Camera;
using Camera::Unit;

Unit::__privateMethods Unit::privateMethods;
Unit::__publicMethods Unit::publicMethods;
Unit::__staticMethods Unit::staticMethods;

void Camera::Locate(HMODULE peRoot) {
	Unit::Locate(peRoot);
}

void Unit::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)&publicMethods.RecordToInternalMementoKey = (PVOID)(peRootOffset + 0x1719d0);
	*(PVOID*)&publicMethods.RestoreFromInternalMementoKey = (PVOID)(peRootOffset + 0x1717d0);
}