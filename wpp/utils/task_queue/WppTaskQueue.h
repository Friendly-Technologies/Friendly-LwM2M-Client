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

#include "WppTypes.h"
#include "WppGuard.h"

#define WPP_TASK_MIN_DELAY_S  (time_t)0
#define WPP_TASK_DEF_DELAY_S  (time_t)1
#define WPP_TASK_MAX_DELAY_S  (time_t)(0xFFFFFFF)

#define WPP_TASK_DEF_CTX	  NULL
#define WPP_ERR_TASK_ID 	  0

namespace wpp {

class WppClient;

/**
 * @class WppTaskQueue
 * @brief Represents a task queue.
 * 
 * The queue of tasks does not guarantee timely calling of a task
 * with a fixed delay, all tasks in the queue are processed sequentially,
 * it is only guaranteed that the task will be called after the specified
 * delay. The queue should not be used for critical tasks. When a task is
 * created via addTask(), the party that creates the task must guarantee
 * the validity of the ctx_t ctx during the entire existence of the 
 * task. Tasks may be deleted not immediately, but with the next call to
 * handleEachTask(), but it is guaranteed that the deleted task will not
 * be executed. It is forbidden to use any delays in the task, because all
 * the tasks are executed sequentially in the same thread, this will cause
 * an execution delay for other tasks. WppTaskQueue should be used only for
 * tasks that require the WppClient context.
 * 
 * The implementation is completely thread-safe and allows calling its methods
 * from tasks.
 * 
 * @note The WppTaskQueue class is a singleton.
 */
class WppTaskQueue {
public:
	using task_id_t = uint32_t;
	/**
	 * Keep in mind that while std::function itself is always copy able,
	 * it might hold a callable object (like a lambda) that captures
	 * variables which may not be copy able. If you try to copy a
	 * std::function that holds a non-copyable callable, it will compile,
	 * but will throw a std::bad_function_call exception at runtime if
	 * you try to call the copied std::function.
	 */
	using task_t = std::function<bool(WppClient&, void *)>;

private:
	enum TaskState : uint8_t {
		IDLE = 1,
		EXECUTING = 2,
		SHOULD_BE_DELETED = 4,
		MAX_TASK_STATE = 0xFF
	};

	struct TaskInfo {
		task_id_t id;
		task_t task;
		time_t delaySec;
		time_t nextCallTime;
		void *ctx = NULL;
		size_t ctxSize = 0;
		TaskState state;
	};

private:
	WppTaskQueue();

public:
	~WppTaskQueue();

	/* ------------- Tasks management ------------- */
	/**
	 * @brief Add task to queue, ctx that passed to task equals to NULL.
	 *
	 * @param delaySec - Min time after which task will be run first time, and 
	 * 			    time beatween next calls of this task while it returns false.
	 * 				Minimum value is WPP_TASK_MIN_DELAY_S, max value is WPP_TASK_MAX_DELAY_S.
	 * @param task - Task for execute, while task returns false it will be called
	 * 			   with specified delay, after returning true task deleted from
	 * 			   queue.
	 * @return id of created task or WPP_ERR_TASK_ID
	 */
	static task_id_t addTask(time_t delaySec, task_t task);

	/**
	 * @brief Add task to queue, ctx passed to task by pointer without copy.
	 *
	 * @param ctx - User data ptr that will be passed to task, without coping.
	 * 			  User must guarantee the validity of the context during the
	 * 			  entire existence of the task.
	 * @param delaySec - Min time after which task will be run first time, and 
	 * 			       time beatween next calls of this task while it returns false.
	 * 				   Minimum value is WPP_TASK_MIN_DELAY_S, max value is WPP_TASK_MAX_DELAY_S.
	 * @param task - Task for execute, while task returns false it will be called
	 * 			   with specified delay, after returning true task deleted from
	 * 			   queue.
	 * @return id of created task or WPP_ERR_TASK_ID
	 */
	static task_id_t addTask(void *ctx, time_t delaySec, task_t task);

	/**
	 * @brief Add task to queue, ctx passed to task by pointer with copy,
	 * allocated memory will be relesed after deleting task from queue.
	 *
	 * @param ctx - Ptr to user data that will be copied and passed to task.
	 * @param size - User data size that will be copied.
	 * @param delaySec - Min time after which task will be run first time, and 
	 * 			       time beatween next calls of this task while it returns false.
	 * 				   Minimum value is WPP_TASK_MIN_DELAY_S, max value is WPP_TASK_MAX_DELAY_S.
	 * @param task - Task for execute, while task returns false it will be called,
	 * 			   with specified delay,after returning true task deleted from
	 * 			   the queue, and relese allocated memory for ctx.
	 * @return id of created task or WPP_ERR_TASK_ID
	 */
	static task_id_t addTaskWithCopy(const void *ctx, size_t size, time_t delaySec, task_t task);

	/**
	 * @brief Returns count of tasks in the queue.
	 * Tasks count does not immediately updated 
	 * after request to remove task.
	 * 
	 */
	static size_t getTaskCnt();

	/**
	 * @brief Returns true if task exists in the queue.
	 */
	static bool isTaskExist(task_id_t id);

	/**
	 * @brief Returns true if state corresponds to function.
	 */
	static bool isTaskIdle(task_id_t id);
	static bool isTaskExecuting(task_id_t id);
	static bool isTaskShouldBeDeleted(task_id_t id);

	/**
	 * @brief This function does not immediately delete the task, it only marks
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
	 * @brief This function does not immediately delete all tasks, it only marks
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

	/**
	 * @brief Blocks task handling, calls of other methods, and deletes all tasks
	 * from the queue, after that returns control over the queue.
	 */
	static void hardReset();

	/**
	 * @brief Execute each task in the queue and delete it from queue if task returns false 
	 * or task state is SHOULD_BE_DELETED. This method is called by the WppClient in its loop() method.
	 * But it can be also called by the user if he wants to process immediately.
	 * @return time in sec to next task execution, if tasks are not exist then returns
	 *   	   WPP_TASK_MAX_DELAY_S.
	 */
	static time_t handleEachTask(WppClient& client);

private:
	/**
	 * @brief Deletes from list task with state SHOULD_BE_DELETED.
	 */
	void deleteFinishedTasks();

	/**
	 * @brief Returns minimum delay in sec to the next task executions.
	 */
	time_t updateNextCallTimeForTasks();

	/**
	 * @brief Returns next task id.
	 */
	task_id_t getNextTaskId();

private:
	static WppGuard _handleTaskGuard;
	static WppGuard _taskQueueGuard;
	static WppTaskQueue	_instance;
	/**
	 * @brief The next task id.
	 */
	task_id_t _nextTaskId;
	/**
	 * @brief Adding, removing and moving the elements within the list or
	 * across several lists does not invalidate the iterators or
	 * references. An iterator is invalidated only when the
	 * corresponding element is deleted.
	 */
	std::list<TaskInfo *> _tasks;
};

} /* namespace wpp */

#endif /* WPP_TASK_QUEUE_H_ */
