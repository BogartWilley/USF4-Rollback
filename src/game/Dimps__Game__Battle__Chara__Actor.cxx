#include <windows.h>

#include "Dimps__Game__Battle__Chara__Actor.hxx"

using Dimps::Game::Battle::Chara::Actor;

Actor::__privateMethods Actor::privateMethods;
Actor::__publicMethods Actor::publicMethods;
Actor::__staticMethods Actor::staticMethods;

void Actor::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;
	*(PVOID*)(&publicMethods.GetActorID) = (PVOID)(peRootOffset + 0x16c6a0);
	*(PVOID*)(&publicMethods.GetComboDamage) = (PVOID)(peRootOffset + 0x142410);
	*(PVOID*)(&publicMethods.GetCurrentSide) = (PVOID)(peRootOffset + 0x141d20);
	*(PVOID*)(&publicMethods.GetDamage) = (PVOID)(peRootOffset + 0x1423d0);
	*(PVOID*)(&publicMethods.GetStatus) = (PVOID)(peRootOffset + 0x142520);
}