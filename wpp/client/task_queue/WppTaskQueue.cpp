#include "WppTaskQueue.h"

#include <algorithm>
#include <cstring>
#include "WppPlatform.h"
#include "WppLogs.h"

namespace wpp {

WppTaskQueue WppTaskQueue::_instance;
std::mutex WppTaskQueue::_handleTaskGuard;
std::mutex WppTaskQueue::_taskQueueGuard;

WppTaskQueue::WppTaskQueue() {
	_handleTaskGuard.unlock();
	_taskQueueGuard.unlock();
}

WppTaskQueue::~WppTaskQueue() {
	std::lock_guard<std::mutex> handleLock(_handleTaskGuard);
	std::lock_guard<std::mutex> queueLock(_taskQueueGuard);

	for (auto task : _tasks) {
		if (task->ctxSize > 0) {
			delete[] (uint8_t *)(task->ctx);
			task->ctxSize = 0;
		}
		delete task;
	}
	_tasks.clear();
}

/* ------------- Tasks management ------------- */
WppTaskQueue::task_id_t WppTaskQueue::addTask(time_t delaySec, task_t task) {
	return addTask(WPP_TASK_DEF_CTX, delaySec, task);
}

WppTaskQueue::task_id_t WppTaskQueue::addTask(ctx_t ctx, time_t delaySec, task_t task) {
	if (delaySec < WPP_TASK_MIN_DELAY_S || WPP_TASK_MAX_DELAY_S < delaySec) return WPP_ERR_TASK_ID;

	std::lock_guard<std::mutex> lock(_taskQueueGuard);

	TaskInfo *newTask = new TaskInfo;
	newTask->task = task;
	newTask->delaySec = std::max(delaySec, WPP_TASK_MIN_DELAY_S);
	newTask->nextCallTime = WppPlatform::getTime() + newTask->delaySec;
	newTask->ctx = ctx;
	newTask->ctxSize = 0;
	newTask->state = IDLE;

	_instance._tasks.push_back(newTask);
	return newTask;
}

WppTaskQueue::task_id_t WppTaskQueue::addTaskWithCopy(ctx_t ctx, size_t size, time_t delaySec, task_t task) {
	if (!ctx || !size || delaySec < WPP_TASK_MIN_DELAY_S || WPP_TASK_MAX_DELAY_S < delaySec) return WPP_ERR_TASK_ID;

	std::lock_guard<std::mutex> lock(_taskQueueGuard);

	TaskInfo *newTask = new TaskInfo;
	newTask->task = task;
	newTask->delaySec = std::max(delaySec, WPP_TASK_MIN_DELAY_S);
	newTask->nextCallTime = WppPlatform::getTime() + newTask->delaySec;
	newTask->ctx = new uint8_t[size];
	memcpy(newTask->ctx, ctx, size);
	newTask->ctxSize = size;
	newTask->state = IDLE;

	_instance._tasks.push_back(newTask);
	return newTask;
}

size_t WppTaskQueue::getTaskCnt() {
	return _instance._tasks.size();
}

bool WppTaskQueue::isTaskExist(task_id_t id) {
	std::lock_guard<std::mutex> lock(_taskQueueGuard);

	auto task = std::find(_instance._tasks.begin(), _instance._tasks.end(), (TaskInfo *)id);
	return task != _instance._tasks.end();
}

bool WppTaskQueue::isTaskIdle(task_id_t id) {
	std::lock_guard<std::mutex> lock(_taskQueueGuard);

	auto task = std::find(_instance._tasks.begin(), _instance._tasks.end(), (TaskInfo *)id);
	if (task == _instance._tasks.end()) return false;
	return (*task)->state & IDLE;
}

bool WppTaskQueue::isTaskExecuting(task_id_t id) {
	std::lock_guard<std::mutex> lock(_taskQueueGuard);

	auto task = std::find(_instance._tasks.begin(), _instance._tasks.end(), (TaskInfo *)id);
	if (task == _instance._tasks.end()) return false;
	return (*task)->state & EXECUTING;
}

bool WppTaskQueue::isTaskShouldBeDeleted(task_id_t id) {
	std::lock_guard<std::mutex> lock(_taskQueueGuard);

	auto task = std::find(_instance._tasks.begin(), _instance._tasks.end(), (TaskInfo *)id);
	if (task == _instance._tasks.end()) return false;
	return (*task)->state & SHOULD_BE_DELETED;
}

void WppTaskQueue::requestToRemoveTask(task_id_t id) {
	std::lock_guard<std::mutex> queueLock(_taskQueueGuard);

	auto task = std::find(_instance._tasks.begin(), _instance._tasks.end(), (TaskInfo *)id);
	if (task == _instance._tasks.end()) return;
	
	(*task)->state = (TaskState)((*task)->state | SHOULD_BE_DELETED);
}

void WppTaskQueue::requestToRemoveEachTask() {
	std::lock_guard<std::mutex> queueLock(_taskQueueGuard);
	for (auto task : _instance._tasks) task->state = (TaskState)(task->state | SHOULD_BE_DELETED);
}

time_t WppTaskQueue::handleEachTask(WppClient& clien) {
	std::lock_guard<std::mutex> handleLock(_handleTaskGuard);

	_taskQueueGuard.lock();
	// We create copy for be sure that adding new task not corrupted begin()
	// and end() iterators inside _tasks list.
	std::list<TaskInfo *> tasksCopy = _instance._tasks;
	_taskQueueGuard.unlock();

	for (auto task : tasksCopy) {
		// Be sure that we do not override SHOULD_BE_DELETED state
		_taskQueueGuard.lock();
		if (task->state & SHOULD_BE_DELETED || task->nextCallTime > WppPlatform::getTime()) {
			_instance._taskQueueGuard.unlock();
			continue;
		}
		task->state = EXECUTING;
		_taskQueueGuard.unlock();

		// Here state can be changed to SHOULD_BE_DELETED but it is
		// not matter becouse we have already set EXECUTING state look
		// at description of requestToRemoveTask() and requestToRemoveEachTask()
		bool isFinished = task->task(clien, task->ctx);

		// Be sure that we do not override SHOULD_BE_DELETED state
		_taskQueueGuard.lock();
		if (isFinished || task->state & SHOULD_BE_DELETED) {
			task->state = SHOULD_BE_DELETED;
		} else {
			task->state = IDLE;
			task->nextCallTime = WppPlatform::getTime() + task->delaySec;
		}
		_taskQueueGuard.unlock();
	}

	_instance.deleteFinishedTasks();
	return _instance.updateNextCallTimeForTasks();
}

void WppTaskQueue::deleteFinishedTasks() {
	std::lock_guard<std::mutex> queueLock(_taskQueueGuard);
	for (auto task = _tasks.begin(); task != _tasks.end();) {
		if (((*task)->state & SHOULD_BE_DELETED) == 0) {
			task++;
			continue;
		}
		if ((*task)->ctxSize > 0) {
			delete[] (uint8_t *)((*task)->ctx);
			(*task)->ctxSize = 0;
		}
		delete (*task);
		task = _tasks.erase(task);
	}
}


time_t WppTaskQueue::updateNextCallTimeForTasks() {
	std::lock_guard<std::mutex> queueLock(_taskQueueGuard);
	time_t nextCallInterval = WPP_TASK_MAX_DELAY_S;
	for (auto task: _tasks) {
		time_t taskCallInterval = std::max(task->nextCallTime - WppPlatform::getTime(), (time_t)0);
		if (taskCallInterval < nextCallInterval) nextCallInterval = taskCallInterval;
	}
	return nextCallInterval;
}

} // namespace wpp