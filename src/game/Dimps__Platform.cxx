#include <windows.h>
#include "Dimps__Platform.hxx"

namespace Platform = Dimps::Platform;
using Platform::D3D;
using Platform::Main;

D3D::__privateMethods D3D::privateMethods;
D3D::__publicMethods D3D::publicMethods;
D3D::__staticMethods D3D::staticMethods;
Main::__privateMethods Main::privateMethods;
Main::__publicMethods Main::publicMethods;
Main::__staticMethods Main::staticMethods;

void Platform::Locate(HMODULE peRoot) {
    D3D::Locate(peRoot);
    Main::Locate(peRoot);
}

void D3D::Locate(HMODULE peRoot) {
    unsigned int peRootOffset = (unsigned int)peRoot;
    *(PVOID*)&privateMethods.Destroy = (PVOID)(peRootOffset + 0x372820);
    staticMethods.RunD3DOperations = (void(WINAPI*)(void*))(peRootOffset + 0x370cc0);
    staticMethods.GetSingleton = (D3D * (WINAPI*)())(peRootOffset + 0x371180);
}

void Main::Locate(HMODULE peRoot) {
    unsigned int peRootOffset = (unsigned int)peRoot;
    *(PVOID*)&publicMethods.Initialize = (PVOID)(peRootOffset + 0x37e3e0);
    staticMethods.RunWindowFunc = (void(WINAPI*)(Main*, HWND, UINT, WPARAM, LPARAM))(peRootOffset + 0x37eb20);
}
