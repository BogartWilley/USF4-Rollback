#include <windows.h>
#include <detours/detours.h>

#include "Dimps__Pad.hxx"
#include "sf4e__Pad.hxx"

namespace rPad = Dimps::Pad;
using rSystem = rPad::System;

namespace fPad = sf4e::Pad;
using fSystem = fPad::System;

unsigned int fSystem::playbackData[2][PLAYBACK_MAX];
int fSystem::playbackFrame = -1;

void fPad::Install() {
	System::Install();
}

void fSystem::Install() {
    int (fSystem:: * _fGetButtons_Mapped)(int) = &GetButtons_Mapped;
    DetourAttach((PVOID*)&rSystem::publicMethods.GetButtons_Mapped, *(PVOID*)&_fGetButtons_Mapped);
}

int fSystem::GetButtons_Mapped(int pindex) {
    if (playbackFrame > -1) {
        return playbackData[pindex][playbackFrame];
    }

    rSystem* _this = (rSystem*)this;
    return (this->*rSystem::publicMethods.GetButtons_Mapped)(pindex);
}
