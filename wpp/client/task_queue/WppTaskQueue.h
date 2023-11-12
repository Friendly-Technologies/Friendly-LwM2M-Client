/*
 * WppManager.h
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_TASK_QUEUE_H_
#define WPP_TASK_QUEUE_H_

#include <list>
#include <functional>
#include <mutex>

#include "types.h"

#define WPP_TASK_MIN_DELAY_S  (time_t)1
#define WPP_TASK_DEF_DELAY_S  (time_t)10
#define WPP_TASK_MAX_DELAY_S  (time_t)(0xFFFFFFF)
#define WPP_ERR_TASK_ID 	  NULL

namespace wpp {

class WppClient;

/**
 * The queue of tasks does not guarantee timely calling of a task
 * with a fixed delay, all tasks in the queue are processed sequentially,
 * it is only guaranteed that the task will be called after the specified
 * delay. The queue should not be used for critical tasks. When a task is
 * created via addTask(), the party that creates the task must guarantee
 * the validity of the context_t ctx during the entire existence of the 
 * task. Tasks may be deleted not immediately, but with the next call to
 * handleEachTask(), but it is guaranteed that the deleted task will not
 * be executed. It is forbidden to use any delays in the task, because all
 * the tasks are executed sequentially in the same thread, this will cause
 * an execution delay for other tasks. WppTaskQueue should be used only for
 * tasks that require the WppClient context.
 * 
 * The implementation is completely thread-safe and allows calling its methods
 * from tasks.
 */
class WppTaskQueue {
	friend class WppClient;

public:
	using task_id_t = void *;
	using context_t = void *;
	using task_t = std::function<bool(WppClient&, context_t)>;

private:
	enum TaskState : uint8_t {
		IDLE = 1,
		EXECUTING = 2,
		SHOULD_BE_DELETED = 4,
		MAX_TASK_STATE = 0xFF
	};

	struct TaskInfo {
		task_t task;
		time_t delaySec;
		time_t nextCallTime;
		context_t ctx = NULL;
		size_t ctxSize = 0;
		TaskState state;
	};

private:
	WppTaskQueue();

public:
	~WppTaskQueue();

	/* ------------- Tasks management ------------- */
	/**
	 * Add task to queue, ctx passed to task by pointer without copy.
	 *
	 * @val ctx - User data ptr that will be passed to task, without coping.
	 * @val delaySec - Min time after which task will be run first time, and 
	 * 			    time beatween next calls of this task while it returns false.
	 * 				Minimum value is WPP_TASK_MIN_DELAY_S.
	 * @val task - Task for execute, while task returns false it will be called
	 * 			   with specified delay, after returning true task deleted from
	 * 			   queue.
	 * @return id of created task or WPP_ERR_TASK_ID
	 */
	static task_id_t addTask(context_t ctx, time_t delaySec, task_t task);

	/**
	 * Add task to queue, ctx passed to task by pointer with copy,
	 * allocated memory will be relesed after deleting task from queue.
	 *
	 * @val ctx - Ptr to user data that will be copied and passed to task.
	 * @val size - User data size that will be copied.
	 * @val delaySec - Min time after which task will be run first time, and 
	 * 			    time beatween next calls of this task while it returns false.
	 * 				Minimum value is WPP_TASK_MIN_DELAY_S.
	 * @val task - Task for execute, while task returns false it will be called,
	 * 			   with specified delay,after returning true task deleted from
	 * 			   the queue, and relese allocated memory for ctx.
	 * @return id of created task or WPP_ERR_TASK_ID
	 */
	static task_id_t addTaskWithCopy(context_t ctx, size_t size, time_t delaySec, task_t task);

	/**
	 * Returns count of tasks in the queue.
	 */
	static size_t getTaskCnt();

	/**
	 * Returns true if task exists in the queue.
	 */
	static bool isTaskExist(task_id_t id);

	/**
	 * Returns true if state corresponds to function.
	 */
	static bool isTaskIdle(task_id_t id);
	static bool isTaskExecuting(task_id_t id);
	static bool isTaskShouldBeDeleted(task_id_t id);

	/**
	 * This function does not immediately delete the task, it only marks
	 * it as one that should be deleted at the first opportunity, namely
	 * when executingEachTask() is called. At the same time, calling 
	 * requestToRemoveTask() guarantees that the task will no longer be executed. 
	 * There is only one exception if the requestToRemoveTask() call refers to a 
	 * task that is being executed right at this moment, then the deletion 
	 * will take place only after the execution is completed, so the user 
	 * must guarantee the validity of the context and the parameters passed
	 * through the capture before the execution of this task is completed.
	 * If after exiting this method the task is not in the EXECUTING state,
	 * the user can be sure that the task is not executed and will not be 
	 * executed again, and the context will no longer be used. If, after 
	 * exiting, the task is in EXECUTING, the user can wait until it is 
	 * completed by polling the isTaskExecuting() method.
	 */
	static void requestToRemoveTask(task_id_t id);

	/**
	 * This function does not immediately delete all tasks, it only marks
	 * them as one that should be deleted at the first opportunity, namely
	 * when executingEachTask() is called. At the same time, calling 
	 * requestToRemoveEachTask() guarantees that the tasks will no longer be executed. 
	 * There is only one exception if the requestToRemoveEachTask() calls during 
	 * task execution right at this moment, then the deletion will take place
	 * only after the execution is completed, so the user must guarantee the
	 * validity of the context and the parameters passed through the capture
	 * before the execution of this task is completed.
	 * If after exiting this method the task is not in the EXECUTING state,
	 * the user can be sure that the task is not executed and will not be 
	 * executed again, and the context will no longer be used. If, after 
	 * exiting, the task is in EXECUTING, the user can wait until it is 
	 * completed by polling the isTaskExecuting() method.
	 */
	static void requestToRemoveEachTask();

private:
	/**
	 * 
	 * Execute each task in the queue and delete it from queue if task returns false 
	 * or task state is SHOULD_BE_DELETED.
	 * @return time in sec to next task execution, if tasks are not exist then returns
	 *   	   WPP_TASK_MAX_DELAY_S.
	 */
	static time_t handleEachTask(WppClient& clien);

	/*
	 * Deletes from list task with state SHOULD_BE_DELETED.
	 */
	void deleteFinishedTasks();

	/*
	 * Returns minimum delay in sec to the next task executions.
	 */
	time_t updateNextCallTimeForTasks();

private:
	static WppTaskQueue	_instance;
	std::mutex _handleTaskGuard;
	std::mutex _taskQueueGuard;
	std::list<TaskInfo *> _tasks;
};

} /* namespace wpp */

#endif /* WPP_TASK_QUEUE_H_ */
