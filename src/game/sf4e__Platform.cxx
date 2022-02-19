#include "sf4e__Platform.hxx"

#include "sf4e__Platform__D3D.hxx"
#include "sf4e__Platform__Main.hxx"

void sf4e::Platform::Install() {
	sf4e::Platform::D3D::Install();
	sf4e::Platform::Main::Install();
}