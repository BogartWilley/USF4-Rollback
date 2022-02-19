#pragma once

#include <windows.h>
#include "Dimps__Platform__D3D.hxx"

namespace sf4e {
	namespace Platform {
		struct D3D : Dimps::Platform::D3D
		{
			static void Install();

			void Destroy();
			static void WINAPI RunD3DOperations(void* arg1);
		};
	}
}