#include <windows.h>

#include "Dimps__Eva.hxx"

namespace Eva = Dimps::Eva;
using Dimps::Eva::Task;
using Dimps::Eva::TaskCore;
using Dimps::Eva::TaskCoreRegistry;

TaskCore::__publicMethods TaskCore::publicMethods;
TaskCoreRegistry::__staticMethods TaskCoreRegistry::staticMethods;

void Eva::Locate(HMODULE peRoot) {
	TaskCore::Locate(peRoot);
	TaskCoreRegistry::Locate(peRoot);
}

int Task::GetPriority(Task* t) {
	return *(int*)((unsigned int)t + 12);
}

void TaskCore::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	*(PVOID*)(&publicMethods.GetName) = (PVOID)(peRootOffset + 0x2d4200);
	*(PVOID*)(&publicMethods.GetNumUsed) = (PVOID)(peRootOffset + 0x28ecf0);
	*(PVOID*)(&publicMethods.GetTaskName) = (PVOID)(peRootOffset + 0x2d55c0);
}

Task** TaskCore::GetTaskBuffer(TaskCore* c) {
	return *(Task***)((unsigned int)c + 60);
}

void TaskCoreRegistry::Locate(HMODULE peRoot) {
	unsigned int peRootOffset = (unsigned int)peRoot;

	staticMethods.GetCoreByIndex = (TaskCore * (*)(int))(peRootOffset + 0x2d62b0);
	staticMethods.GetNumActiveCores = (int (*)())(peRootOffset + 0x2d69b0);
}