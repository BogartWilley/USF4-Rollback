#include <windows.h>

#include "Dimps__Eva__TaskCore.hxx"

using Dimps::Eva::TaskCore;

TaskCore::__privateMethods TaskCore::privateMethods;
TaskCore::__publicMethods TaskCore::publicMethods;
TaskCore::__staticMethods TaskCore::staticMethods;

void Dimps::Eva::TaskCore::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)(&publicMethods.GetName) = (PVOID)(peRootOffset + 0x2d4200);
}