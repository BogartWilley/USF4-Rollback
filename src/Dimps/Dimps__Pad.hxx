#pragma once

#include <windows.h>

namespace Dimps {
	namespace Pad {
		void Locate(HMODULE peRoot);

		struct System {
			static const int BUTTON_MAPPING_FIGHT;
			static const int BUTTON_MAPPING_MENU;

			struct PlayerEntry {
				BYTE pad[80];

				static int* DeviceType(PlayerEntry* e);
				static int* DeviceIndex(PlayerEntry* e);
				static int* AssignedController(PlayerEntry* e);
			};

			typedef struct __publicMethods {
				unsigned int(System::* GetButtons_RawOn)(int pindex);
				unsigned int(System::* GetButtons_MappedOn)(int pindex);
				unsigned int(System::* GetButtons_RawRising)(int pindex);
				unsigned int(System::* GetButtons_RawFalling)(int pindex);
				unsigned int(System::* GetButtons_RawRisingWithRepeat)(int pindex);
				int(System::* GetAllDeviceCount)();
				int(System::* GetOKDeviceCount)();
				char*(System::* GetDeviceName)(int dindex);
				int (System::* GetDeviceIndexForPlayer)(int pindex);
				int (System::* GetDeviceTypeForPlayer)(int pindex);
				int (System::* GetAssigmentStatusForPlayer)(int pindex);
				void (System::* AssociatePlayerAndGamepad)(int side, int deviceIndex);
				void (System::* SetSideHasAssignedController)(int side, int clearIfFalse);
				void (System::* SetDeviceTypeForPlayer)(int side, int deviceType);
				void (System::* SetActiveButtonMapping)(int buttonMapping);
			} __publicMethods;

			typedef struct __staticMethods {
				System* (*GetSingleton)();
			} __staticMethods;

			static void Locate(HMODULE peRoot);
			static __publicMethods publicMethods;
			static __staticMethods staticMethods;
		};
	}
}