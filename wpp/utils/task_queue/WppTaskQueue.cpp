#include "WppTaskQueue.h"

#include <algorithm>
#include <cstring>
#include "WppPlatform.h"
#include "WppLogs.h"

namespace wpp {

WppGuard WppTaskQueue::_taskQueueGuard;
WppGuard WppTaskQueue::_handleTaskGuard;
WppTaskQueue WppTaskQueue::_instance;


WppTaskQueue::WppTaskQueue() : _nextTaskId(WPP_ERR_TASK_ID) {}

WppTaskQueue::~WppTaskQueue() {
	hardReset();
}

/* ------------- Tasks management ------------- */
WppTaskQueue::task_id_t WppTaskQueue::addTask(time_t delaySec, task_t task) {
	return addTask(WPP_TASK_DEF_CTX, delaySec, task);
}

WppTaskQueue::task_id_t WppTaskQueue::addTask(void *ctx, time_t delaySec, task_t task) {
	if (delaySec < WPP_TASK_MIN_DELAY_S || WPP_TASK_MAX_DELAY_S < delaySec) return WPP_ERR_TASK_ID;

	_taskQueueGuard.lock();

	task_id_t id = _instance.getNextTaskId();
	if (id == WPP_ERR_TASK_ID) {
		WPP_LOGE(TAG_WPP_TASK, "Can't add task becouse task id is WPP_ERR_TASK_ID, looks like all task ids are busy.");
		_taskQueueGuard.unlock();
		return WPP_ERR_TASK_ID;
	}

	TaskInfo *newTask = new TaskInfo;
	newTask->id = id;
	newTask->task = task;
	newTask->delaySec = delaySec;
	newTask->nextCallTime = WppPlatform::getTime() + newTask->delaySec;
	newTask->ctx = ctx;
	newTask->ctxSize = 0;
	newTask->state = IDLE;

	_instance._tasks.push_back(newTask);
	_taskQueueGuard.unlock();
	
	return id;
}

WppTaskQueue::task_id_t WppTaskQueue::addTaskWithCopy(const void *ctx, size_t size, time_t delaySec, task_t task) {
	if (!ctx || !size || delaySec < WPP_TASK_MIN_DELAY_S || WPP_TASK_MAX_DELAY_S < delaySec) return WPP_ERR_TASK_ID;

	_taskQueueGuard.lock();

	task_id_t id = _instance.getNextTaskId();
	if (id == WPP_ERR_TASK_ID) {
		WPP_LOGE(TAG_WPP_TASK, "Can't add task becouse task id is WPP_ERR_TASK_ID, looks like all task ids are busy.");
		_taskQueueGuard.unlock();
		return WPP_ERR_TASK_ID;
	}

	TaskInfo *newTask = new TaskInfo;
	newTask->id = id;
	newTask->task = task;
	newTask->delaySec = delaySec;
	newTask->nextCallTime = WppPlatform::getTime() + newTask->delaySec;
	newTask->ctx = new uint8_t[size];
	memcpy(newTask->ctx, ctx, size);
	newTask->ctxSize = size;
	newTask->state = IDLE;

	_instance._tasks.push_back(newTask);
	_taskQueueGuard.unlock();

	return id;
}

size_t WppTaskQueue::getTaskCnt() {
	return _instance._tasks.size();
}

bool WppTaskQueue::isTaskExist(task_id_t id) {
	_taskQueueGuard.lock();

	auto task = std::find_if(_instance._tasks.begin(), _instance._tasks.end(), [id](TaskInfo *task) { return task->id == id; });
	bool isExist = task != _instance._tasks.end();

	_taskQueueGuard.unlock();
	return isExist;
}

bool WppTaskQueue::isTaskIdle(task_id_t id) {
	_taskQueueGuard.lock();

	auto task = std::find_if(_instance._tasks.begin(), _instance._tasks.end(), [id](TaskInfo *task) { return task->id == id; });
	if (task == _instance._tasks.end()) {
		_taskQueueGuard.unlock();
		return false;
	}
	bool isIdle = (*task)->state & IDLE;

	_taskQueueGuard.unlock();
	return isIdle;
}

bool WppTaskQueue::isTaskExecuting(task_id_t id) {
	_taskQueueGuard.lock();

	auto task = std::find_if(_instance._tasks.begin(), _instance._tasks.end(), [id](TaskInfo *task) { return task->id == id; });
	if (task == _instance._tasks.end()) {
		_taskQueueGuard.unlock();
		return false;
	}
	bool isExecuting = (*task)->state & EXECUTING;

	_taskQueueGuard.unlock();
	return isExecuting;
}

bool WppTaskQueue::isTaskShouldBeDeleted(task_id_t id) {
	_taskQueueGuard.lock();

	auto task = std::find_if(_instance._tasks.begin(), _instance._tasks.end(), [id](TaskInfo *task) { return task->id == id; });
	if (task == _instance._tasks.end()) {
		_taskQueueGuard.unlock();
		return false;
	}
	bool isShouldBeDeleted = (*task)->state & SHOULD_BE_DELETED;

	_taskQueueGuard.unlock();
	return isShouldBeDeleted;
}

void WppTaskQueue::requestToRemoveTask(task_id_t id) {
	_taskQueueGuard.lock();

	auto task = std::find_if(_instance._tasks.begin(), _instance._tasks.end(), [id](TaskInfo *task) { return task->id == id; });
	if (task == _instance._tasks.end()) {
		_taskQueueGuard.unlock();
		return;
	}
	(*task)->state = (TaskState)((*task)->state | SHOULD_BE_DELETED);

	_taskQueueGuard.unlock();
}

void WppTaskQueue::requestToRemoveEachTask() {
	_taskQueueGuard.lock();
	for (auto task : _instance._tasks) task->state = (TaskState)(task->state | SHOULD_BE_DELETED);
	_taskQueueGuard.unlock();
}

void WppTaskQueue::hardReset() {
	_handleTaskGuard.lock();
	_taskQueueGuard.lock();

	for (auto task : _instance._tasks) {
		if (task->ctxSize > 0) {
			delete[] (uint8_t *)(task->ctx);
			task->ctxSize = 0;
		}
		delete task;
	}
	_instance._tasks.clear();

	_handleTaskGuard.unlock();
	_taskQueueGuard.unlock();
}

time_t WppTaskQueue::handleEachTask(WppClient& client) {
	_handleTaskGuard.lock();

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
		bool isFinished = task->task(client, task->ctx);

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
	time_t nextCallInterval = _instance.updateNextCallTimeForTasks();
	_handleTaskGuard.unlock();
	return nextCallInterval;
}

void WppTaskQueue::deleteFinishedTasks() {
	_taskQueueGuard.lock();
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
	_taskQueueGuard.unlock();
}


time_t WppTaskQueue::updateNextCallTimeForTasks() {
	_taskQueueGuard.lock();
	time_t nextCallInterval = WPP_TASK_MAX_DELAY_S;
	for (auto task: _tasks) {
		time_t taskCallInterval = std::max(task->nextCallTime - WppPlatform::getTime(), (time_t)0);
		if (taskCallInterval < nextCallInterval) nextCallInterval = taskCallInterval;
	}
	_taskQueueGuard.unlock();
	return nextCallInterval;
}

WppTaskQueue::task_id_t WppTaskQueue::getNextTaskId() {
	task_id_t baseId = _nextTaskId;
	task_id_t newId = WPP_ERR_TASK_ID;
	bool isExist = true;

	do {
		newId = _nextTaskId++;
		if (newId == WPP_ERR_TASK_ID) continue;
		auto task = std::find_if(_instance._tasks.begin(), _instance._tasks.end(), [newId](TaskInfo *task) { return task->id == newId; });
		isExist = task != _instance._tasks.end();
	} while (isExist && baseId != _nextTaskId);

	if (baseId == _nextTaskId) newId = WPP_ERR_TASK_ID;
	
	return newId;
}

} // namespace wpp