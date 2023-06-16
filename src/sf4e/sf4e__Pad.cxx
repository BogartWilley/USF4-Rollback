#include <windows.h>
#include <detours/detours.h>

#include "../Dimps/Dimps__Pad.hxx"
#include "sf4e__Pad.hxx"

namespace rPad = Dimps::Pad;
using rSystem = rPad::System;

namespace fPad = sf4e::Pad;
using fSystem = fPad::System;

fSystem::Inputs fSystem::playbackData[PLAYBACK_MAX][2];
int fSystem::playbackFrame = -1;

void fPad::Install() {
	System::Install();
}

void fSystem::Install() {
    unsigned int (fSystem:: * _fGetButtons_MappedOn)(int) = &GetButtons_MappedOn;
    unsigned int (fSystem:: * _fGetButtons_RawOn)(int) = &GetButtons_RawOn;
    DetourAttach((PVOID*)&rSystem::publicMethods.GetButtons_MappedOn, *(PVOID*)&_fGetButtons_MappedOn);
    DetourAttach((PVOID*)&rSystem::publicMethods.GetButtons_RawOn, *(PVOID*)&_fGetButtons_RawOn);
}

unsigned int fSystem::GetButtons_MappedOn(int pindex) {
    if (playbackFrame > -1) {
        return playbackData[playbackFrame][pindex].mappedOn;
    }

    rSystem* _this = (rSystem*)this;
    return (this->*rSystem::publicMethods.GetButtons_MappedOn)(pindex);
}

unsigned int fSystem::GetButtons_RawOn(int pindex) {
    if (playbackFrame > -1) {
        return playbackData[playbackFrame][pindex].rawOn;
    }

    rSystem* _this = (rSystem*)this;
    return (this->*rSystem::publicMethods.GetButtons_RawOn)(pindex);
}
