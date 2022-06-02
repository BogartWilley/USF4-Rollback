#pragma once

#include <windows.h>

namespace Dimps {
	namespace Eva {
		void Locate(HMODULE peRoot);

		struct TaskCore {
			typedef struct __privateMethods {
				// TODO
			} __privateMethods;

			typedef struct __publicMethods {
				char* (TaskCore::* GetName)();
			} __publicMethods;

			typedef struct __staticMethods {
				// TODO
			} __staticMethods;

			static void Locate(HMODULE peRoot);
			static __privateMethods privateMethods;
			static __publicMethods publicMethods;
			static __staticMethods staticMethods;

			// Instance values here
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