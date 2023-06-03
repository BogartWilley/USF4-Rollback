#include <windows.h>

#include "Dimps__Game__Battle__Command__CommandImpl.hxx"

using Dimps::Game::Battle::Command::CommandImpl;

CommandImpl::__privateMethods CommandImpl::privateMethods;
CommandImpl::__publicMethods CommandImpl::publicMethods;
CommandImpl::__staticMethods CommandImpl::staticMethods;

void CommandImpl::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)(&publicMethods.GetCurrentOnSwitches) = (PVOID)(peRootOffset + 0x19a460);
	*(PVOID*)(&publicMethods.GetCurrentRisingSwitches) = (PVOID)(peRootOffset + 0x19a470);
	*(PVOID*)(&publicMethods.GetCurrentFallingSwitches) = (PVOID)(peRootOffset + 0x19a480);
}