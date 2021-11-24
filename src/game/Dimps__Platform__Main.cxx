#include <windows.h>
#include <strsafe.h>
#include <detours.h>

#include "Dimps__platform__Main.hxx"

using Dimps::Platform::Main;

Main::__privateMethods Main::privateMethods;
Main::__publicMethods Main::publicMethods;
Main::__staticMethods Main::staticMethods;

void Main::Locate(HMODULE peRoot) {
    unsigned int peRootOffset = (unsigned int)peRoot;
    *(PVOID*)&publicMethods.Initialize = (PVOID)(peRootOffset + 0x37e3e0);
    staticMethods.RunWindowFunc = (void(WINAPI*)(Main*, HWND, UINT, WPARAM, LPARAM))(peRootOffset + 0x37eb20);
}
