#include <windows.h>
#include "Dimps__Platform.hxx"
#include "Dimps__Platform__D3D.hxx"
#include "Dimps__Platform__Main.hxx"

void Dimps::Platform::Locate(HMODULE peRoot) {
	Dimps::Platform::D3D::Locate(peRoot);
	Dimps::Platform::Main::Locate(peRoot);
}