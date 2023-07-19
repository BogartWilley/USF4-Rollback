#include <windows.h>
#include "Dimps__Platform.hxx"

namespace Platform = Dimps::Platform;
using Platform::dString;
using Platform::D3D;
using Platform::Main;
using Platform::UNK_ScaleformRelated;

dString::__publicMethods dString::publicMethods;
D3D::__privateMethods D3D::privateMethods;
D3D::__publicMethods D3D::publicMethods;
D3D::__staticMethods D3D::staticMethods;
Main::__privateMethods Main::privateMethods;
Main::__publicMethods Main::publicMethods;
Main::__staticMethods Main::staticMethods;
UNK_ScaleformRelated::__publicMethods UNK_ScaleformRelated::publicMethods;
UNK_ScaleformRelated::__staticMethods UNK_ScaleformRelated::staticMethods;

void Platform::Locate(HMODULE peRoot) {
    dString::Locate(peRoot);
    D3D::Locate(peRoot);
    Main::Locate(peRoot);
    UNK_ScaleformRelated::Locate(peRoot);
}

void dString::Locate(HMODULE peRoot) {
    unsigned int peRootOffset = (unsigned int)peRoot;
    *(PVOID*)&publicMethods.assign = (PVOID)(peRootOffset + 0x4890);
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
    *(PVOID*)&publicMethods.Destroy = (PVOID)(peRootOffset + 0x37e6c0);
    staticMethods.RunWindowFunc = (void(WINAPI*)(Main*, HWND, UINT, WPARAM, LPARAM))(peRootOffset + 0x37eb20);
}

void UNK_ScaleformRelated::Locate(HMODULE peRoot) {
    unsigned int peRootOffset = (unsigned int)peRoot;
    *(PVOID*)&publicMethods.GetNumFramesToSim = (PVOID)(peRootOffset + 0x2b65a0);
    *(PVOID*)&publicMethods.SetNumFramesToSim = (PVOID)(peRootOffset + 0x2b66e0);
    staticMethods.GetSingleton = (UNK_ScaleformRelated*(*)())(peRootOffset + 0x2b7660);
}