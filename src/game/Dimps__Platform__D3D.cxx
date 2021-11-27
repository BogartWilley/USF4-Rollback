#include <windows.h>

#include "Dimps__Platform__D3D.hxx"

using Dimps::Platform::D3D;

D3D::__privateMethods D3D::privateMethods;
D3D::__publicMethods D3D::publicMethods;
D3D::__staticMethods D3D::staticMethods;

void D3D::Locate(HMODULE peRoot) {
    unsigned int peRootOffset = (unsigned int)peRoot;
    *(PVOID*)&privateMethods.Destroy = (PVOID)(peRootOffset + 0x372820);
    staticMethods.RunD3DOperations = (void(WINAPI*)(void*))(peRootOffset + 0x370cc0);
    staticMethods.GetSingleton = (D3D*(WINAPI*)())(peRootOffset + 0x371180);
}
