#include "sf4x__Platform.hxx"

#include "sf4x__Platform__D3D.hxx"
#include "sf4x__Platform__Main.hxx"

void sf4x::Platform::Install() {
	sf4x::Platform::D3D::Install();
	sf4x::Platform::Main::Install();
}