#pragma once

#include <windows.h>

namespace Dimps {
	namespace Eva {
		void Locate(HMODULE peRoot);

		struct EmRandom {
			void* vtbl;
			DWORD seed;
		};

		struct Task {
			char pad[96];

			static int GetPriority(Task* t);
		};

		struct TaskCore {
			typedef struct __publicMethods {
				char* (TaskCore::* GetName)();
				int (TaskCore::* GetNumUsed)();
				char* (TaskCore::* GetTaskName)(Task** t);
			} __publicMethods;

			static void Locate(HMODULE peRoot);
			static Task** GetTaskBuffer(TaskCore* c);
			static __publicMethods publicMethods;
		};

		struct TaskCoreRegistry {
			typedef struct __staticMethods {
				TaskCore* (*GetCoreByIndex)(int index);
				int (*GetNumActiveCores)();
			} __staticMethods;

			static void Locate(HMODULE peRoot);
			static __staticMethods staticMethods;
		};
	}
}