#include <random>
#include <time.h>

#include "../Dimps/Dimps__Eva.hxx"
#include "../Dimps/Dimps__Platform.hxx"

#include "sf4e.hxx"
#include "sf4e__Event.hxx"
#include "sf4e__Game.hxx"
#include "sf4e__GameEvents.hxx"
#include "sf4e__Pad.hxx"
#include "sf4e__Platform.hxx"
#include "sf4e__UserApp.hxx"

std::mt19937 sf4e::localRand;

using rIEmSpriteAction = Dimps::Eva::IEmSpriteAction;
using rIEmSpriteNode = Dimps::Eva::IEmSpriteNode;
using rIEmTaskFunctor = Dimps::Eva::IEmTaskFunctor;
using rTask = Dimps::Eva::Task;
using rTaskCore = Dimps::Eva::TaskCore;
using rAllocator = Dimps::Platform::Allocator;
using fIEmSpriteAction = sf4e::Eva::IEmSpriteAction;
using fTask = sf4e::Eva::Task;
using fTaskCore = sf4e::Eva::TaskCore;

void sf4e::Install() {
	localRand.seed(time(NULL));

	Event::Install();
	Game::Install();
	GameEvents::Install();
	Pad::Install();
	Platform::Install();
	UserApp::Install();
}

void fIEmSpriteAction::RecordToAdditionalMemento(rIEmSpriteAction* a, AdditionalMemento& m) {
	m.action = *a;
}

void fIEmSpriteAction::RestoreFromAdditionalMemento(rIEmSpriteAction* a, const AdditionalMemento& m) {
	*a = m.action;
}

void fTask::RecordToAdditionalMemento(rTask* t, AdditionalMemento& m) {
	// Copy _almost_ all the raw task state by copying all of it then
	// zeroing any pointers. While we don't strictly need to zero the
	// pointers, it'll turn incorrect accesses into null pointer
	// accesses rather than arbitrary memory accesses, which is at least
	// less dangerous.
	// it is more sound to not .
	memcpy_s(&m.rawTask, sizeof(rTask), t, sizeof(rTask));
	*rTask::GetCancelFunctor(&m.rawTask) = nullptr;
	*rTask::GetWorkFunctor(&m.rawTask) = nullptr;
	*rTask::GetNext(&m.rawTask) = nullptr;
	*rTask::GetPrevious(&m.rawTask) = nullptr;

	rIEmTaskFunctor* cancelFunctor = *rTask::GetCancelFunctor(t);
	m.hasCancelFunctor = cancelFunctor != nullptr;
	if (m.hasCancelFunctor) {
		size_t cancelFunctorSize = rIEmTaskFunctor::GetDescendantFunctorSizeByVtable(*(DWORD*)cancelFunctor);
		// There are literally thousands of functors, and only a handful of them
		// have sizes captured. TaskFunctorBuf should be large enough to capture
		// all of them, but asserting that with static analysis is borderline
		// impossible.
		assert(cancelFunctorSize > 0);
		assert(cancelFunctorSize <= sizeof(TaskFunctorBuf));
		memcpy_s(&m.cancelFunctor, cancelFunctorSize, cancelFunctor, cancelFunctorSize);
	}

	rIEmTaskFunctor* workFunctor = *rTask::GetWorkFunctor(t);
	m.hasWorkFunctor = workFunctor != nullptr;
	if (m.hasWorkFunctor) {
		size_t workFunctorSize = rIEmTaskFunctor::GetDescendantFunctorSizeByVtable(*(DWORD*)workFunctor);
		// There are literally thousands of functors, and only a handful of them
		// have sizes captured. TaskFunctorBuf should be large enough to capture
		// all of them, but asserting that with static analysis is borderline
		// impossible.
		assert(workFunctorSize > 0);
		assert(workFunctorSize <= sizeof(TaskFunctorBuf));
		memcpy_s(&m.workFunctor, workFunctorSize, workFunctor, workFunctorSize);
	}
}

void fTask::RestoreFromAdditionalMemento(rTask* t, const AdditionalMemento& m) {
	rAllocator* allocator = rAllocator::staticMethods.GetSingleton();

	// We shouldn't be attempting to restore into a task that has functors,
	// because we can't soundly delete them. We should only be attempting
	// to restore into newly acquired tasks which don't have functors
	// associated yet. Similarly, the raw task in the memento shouldn't
	// have any functors, or the memcpy below may create leaks.
	assert(*rTask::GetCancelFunctor(t) == nullptr);
	assert(*rTask::GetWorkFunctor(t) == nullptr);
	assert(*rTask::GetCancelFunctor((rTask*)&m.rawTask) == nullptr);
	assert(*rTask::GetWorkFunctor((rTask*)&m.rawTask) == nullptr);

	// Set aside the current next and prev tasks, copy the old state
	// directly into place, then re-link the next and prev tasks.
	rTask* next = *rTask::GetNext(t);
	rTask* prev = *rTask::GetPrevious(t);
	memcpy_s(t, sizeof(rTask), &m.rawTask, sizeof(rTask));
	*rTask::GetNext(t) = next;
	*rTask::GetPrevious(t) = prev;

	// The functors need to actually be allocated pointers, as the task cancellation
	// will attempt to free the memory that the IEmTaskFunctor* pointers are pointing
	// at. If the tasks just pointed at the memory in the AdditionalMemento, the
	// attempt to free would fail.
	if (m.hasCancelFunctor) {
		size_t cancelFunctorSize = rIEmTaskFunctor::GetDescendantFunctorSizeByVtable(*(DWORD*)&m.cancelFunctor);
		void* newCancelFunctor = (allocator->*rAllocator::publicMethods.Allocate)(cancelFunctorSize, 0, -1);
		memcpy_s(newCancelFunctor, cancelFunctorSize, &m.cancelFunctor, cancelFunctorSize);
		*rTask::GetCancelFunctor(t) = (rIEmTaskFunctor*)newCancelFunctor;
	}

	if (m.hasWorkFunctor) {
		size_t workFunctorSize = rIEmTaskFunctor::GetDescendantFunctorSizeByVtable(*(DWORD*)&m.workFunctor);
		void* newWorkFunctor = (allocator->*rAllocator::publicMethods.Allocate)(workFunctorSize, 0, -1);
		memcpy_s(newWorkFunctor, workFunctorSize, &m.workFunctor, workFunctorSize);
		*rTask::GetWorkFunctor(t) = (rIEmTaskFunctor*)newWorkFunctor;
	}
}

void fTaskCore::RecordToAdditionalMemento(rTaskCore* c, AdditionalMemento& m) {
	m.numUsed = (c->*rTaskCore::publicMethods.GetNumUsed)();
	size_t taskDataSize = *rTaskCore::GetTaskDataSize(c);
	assert(taskDataSize <= sizeof(fTaskCore::TaskDataBuf));

	int i;
	rTask* cursor;
	for (cursor = rTaskCore::GetTaskHead(c), i = 0; cursor != nullptr; cursor = *rTask::GetNext(cursor), i++) {
		fTask::RecordToAdditionalMemento(cursor, m.tasks[i]);
		memcpy_s(&m.taskdata[i], taskDataSize, *rTask::GetTaskData(cursor), taskDataSize);
	}
}

void fTaskCore::RestoreFromAdditionalMemento(rTaskCore* c, const AdditionalMemento& m) {
	rTask* cursor;
	for (
		cursor = rTaskCore::GetTaskHead(c);
		cursor != nullptr;
		cursor = *rTask::GetNext(cursor)
	) {
		rTask* victim = cursor;
		(c->*rTaskCore::publicMethods.CancelTask)(&victim);
	}
	(c->*rTaskCore::publicMethods.ReclaimCancelledTasks)();

	size_t taskDataSize = *rTaskCore::GetTaskDataSize(c);
	assert(taskDataSize <= sizeof(fTaskCore::TaskDataBuf));
	int i;
	for (i = 0; i < m.numUsed; i++) {
		rTask* newTask;
		(c->*rTaskCore::publicMethods.AllocateNewTask)(
			&newTask,
			*rTask::GetPriority((rTask*)&m.tasks[i].rawTask),
			0,
			0
		);
		fTask::RestoreFromAdditionalMemento(newTask, m.tasks[i]);
		memcpy_s(*rTask::GetTaskData(newTask), taskDataSize, &m.taskdata[i], taskDataSize);
	}
}
