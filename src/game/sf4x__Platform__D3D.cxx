#include <windows.h>
#include <strsafe.h>
#include <detours.h>

#include "../overlay/overlay.h"

#include "Dimps__Platform__D3D.hxx"
#include "sf4x__Platform__D3D.hxx"

using fD3D = sf4x::Platform::D3D;
using rD3D = Dimps::Platform::D3D;

void fD3D::Install() {
    void (fD3D:: * _fDestroy)() = &Destroy;
    DetourAttach((PVOID*)&rD3D::privateMethods.Destroy, *(PVOID*)&_fDestroy);
    DetourAttach((PVOID*)&rD3D::staticMethods.RunD3DOperations, RunD3DOperations);
}

void WINAPI fD3D::RunD3DOperations(void* data) {
    rD3D::staticMethods.RunD3DOperations(data);
    DrawOverlay();
}

void fD3D::Destroy() {
    FreeOverlay();
    (this->*rD3D::privateMethods.Destroy)();
}