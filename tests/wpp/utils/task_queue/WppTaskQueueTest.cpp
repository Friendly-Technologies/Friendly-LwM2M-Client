#include "catch_amalgamated.hpp"
#include "WppTaskQueue.h"
#include <thread>

namespace wpp
{
    class WppClient
    {
    public:
        WppClient() {}
        ~WppClient() {}
    };
}

using namespace wpp;

static time_t handleTasks()
{
    WppClient client;
    time_t next_call = WppTaskQueue::handleEachTask(client);
    return next_call;
}

static void clearWppTaskQueue()
{
    WppTaskQueue::requestToRemoveEachTask();
    handleTasks();
}

static WppTaskQueue::task_t createDummyTask()
{
    return [](WppClient &, void *)
    { return true; };
}

TEST_CASE("WppTaskQueue: Task creating", "[addTask][addTaskWithCopy]")
{
    char str[] = "Hello, world!";

    clearWppTaskQueue();

    REQUIRE(WppTaskQueue::getTaskCnt() == 0);

    SECTION("Add Task without context")
    {
        for (size_t i = 0; i < 100; i++)
        {
            WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
            REQUIRE(taskId != WPP_ERR_TASK_ID);
            REQUIRE(WppTaskQueue::getTaskCnt() == i + 1);
            REQUIRE(WppTaskQueue::isTaskExist(taskId));
            REQUIRE(WppTaskQueue::isTaskIdle(taskId));
            REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId));
            REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId));
        }

        clearWppTaskQueue();
    }

    SECTION("Add Task with context")
    {
        for (size_t i = 0; i < 100; i++)
        {
            WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(str, WPP_TASK_DEF_DELAY_S, createDummyTask());
            REQUIRE(taskId != WPP_ERR_TASK_ID);
            REQUIRE(WppTaskQueue::getTaskCnt() == i + 1);
            REQUIRE(WppTaskQueue::isTaskExist(taskId));
            REQUIRE(WppTaskQueue::isTaskIdle(taskId));
            REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId));
            REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId));
        }

        clearWppTaskQueue();
    }

    SECTION("Add Task with copied context")
    {
        for (size_t i = 0; i < 100; i++)
        {
            WppTaskQueue::task_id_t taskId = WppTaskQueue::addTaskWithCopy(str, sizeof(str), WPP_TASK_DEF_DELAY_S, createDummyTask());
            REQUIRE(taskId != WPP_ERR_TASK_ID);
            REQUIRE(WppTaskQueue::getTaskCnt() == i + 1);
            REQUIRE(WppTaskQueue::isTaskExist(taskId));
            REQUIRE(WppTaskQueue::isTaskIdle(taskId));
            REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId));
            REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId));
        }

        clearWppTaskQueue();
    }
}

TEST_CASE("WppTaskQueue: Task removing", "[requestToRemoveTask][requestToRemoveEachTask][hardReset]")
{
    REQUIRE(WppTaskQueue::getTaskCnt() == 0);

    SECTION("Remove Task")
    {
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(10, createDummyTask());
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::getTaskCnt() == 1);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId));

        WppTaskQueue::requestToRemoveTask(taskId);
        REQUIRE(WppTaskQueue::isTaskShouldBeDeleted(taskId));
        REQUIRE(WppTaskQueue::getTaskCnt() == 1);

        clearWppTaskQueue();
    }

    SECTION("Remove each task")
    {
        WppTaskQueue::task_id_t taskId1 = WppTaskQueue::addTask(10, createDummyTask());
        WppTaskQueue::task_id_t taskId2 = WppTaskQueue::addTask(10, createDummyTask());
        REQUIRE(taskId1 != WPP_ERR_TASK_ID);
        REQUIRE(taskId2 != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::getTaskCnt() == 2);
        REQUIRE(WppTaskQueue::isTaskExist(taskId1));
        REQUIRE(WppTaskQueue::isTaskExist(taskId2));
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId1));
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId2));

        WppTaskQueue::requestToRemoveEachTask();
        REQUIRE(WppTaskQueue::isTaskShouldBeDeleted(taskId1));
        REQUIRE(WppTaskQueue::isTaskShouldBeDeleted(taskId2));
        REQUIRE(WppTaskQueue::getTaskCnt() == 2);

        clearWppTaskQueue();
    }

    SECTION("Hard reset")
    {
        char str[] = "Hello, world!";
        WppTaskQueue::task_id_t taskId1 = WppTaskQueue::addTask(10, createDummyTask());
        WppTaskQueue::task_id_t taskId2 = WppTaskQueue::addTaskWithCopy(str, sizeof(str), 10, createDummyTask());
        REQUIRE(taskId1 != WPP_ERR_TASK_ID);
        REQUIRE(taskId2 != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::getTaskCnt() == 2);
        REQUIRE(WppTaskQueue::isTaskExist(taskId1));
        REQUIRE(WppTaskQueue::isTaskExist(taskId2));
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId1));
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId2));

        WppTaskQueue::hardReset();
        REQUIRE(WppTaskQueue::getTaskCnt() == 0);

        clearWppTaskQueue();
    }
}

TEST_CASE("WppTaskQueue: Task state checking", "[isTaskExist][isTaskIdle][isTaskExecuting][isTaskShouldBeDeleted]")
{
    REQUIRE(WppTaskQueue::getTaskCnt() == 0);

    SECTION("Is task exist")
    {
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    SECTION("Is task idle")
    {
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskIdle(taskId));

        clearWppTaskQueue();
    }

    SECTION("Is task executing")
    {
        WppTaskQueue::task_t task = [](WppClient &client, void *ctx)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return true;
        };
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(0, task);
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId));
        std::thread my_thread([]()
                              { handleTasks(); });

        std::this_thread::sleep_for(std::chrono::seconds(1));
        REQUIRE(WppTaskQueue::isTaskExecuting(taskId));

        my_thread.join();
        REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId));

        clearWppTaskQueue();
    }

    SECTION("Is task should be deleted")
    {
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId));
        WppTaskQueue::requestToRemoveTask(taskId);
        REQUIRE(WppTaskQueue::isTaskShouldBeDeleted(taskId));

        clearWppTaskQueue();
    }
}

TEST_CASE("WppTaskQueue: Task executing")
{
    REQUIRE(WppTaskQueue::getTaskCnt() == 0);

    SECTION("Task with null context")
    {
        WppTaskQueue::task_t task = [](WppClient &client, void *ctx)
        {
            REQUIRE(ctx == NULL);
            return true;
        };
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, task);
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        std::thread my_thread([]()
                              { handleTasks(); });
        my_thread.join();

        clearWppTaskQueue();
    }

    SECTION("Task with context")
    {
        char str[] = "Hello, world!";
        WppTaskQueue::task_t task = [](WppClient &client, void *ctx)
        {
            REQUIRE(ctx != NULL);
            REQUIRE(strcmp((char *)ctx, "Hello, world!") == 0);
            return true;
        };
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(str, WPP_TASK_MIN_DELAY_S, task);
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        std::thread my_thread([]()
                              { handleTasks(); });
        my_thread.join();

        clearWppTaskQueue();
    }

    SECTION("Task with copied context")
    {
        char str[] = "Hello, world!";
        WppTaskQueue::task_t task = [](WppClient &client, void *ctx)
        {
            REQUIRE(ctx != NULL);
            REQUIRE(strcmp((char *)ctx, "Hello, world!") == 0);
            return true;
        };
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTaskWithCopy(str, sizeof(str), 0, task);
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        std::thread my_thread([]()
                              { handleTasks(); });
        my_thread.join();

        clearWppTaskQueue();
    }

    SECTION("Task with delay call")
    {
        WppTaskQueue::task_t task = [](WppClient &client, void *ctx)
        {
            return true;
        };
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(2, task);
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        std::thread my_thread([taskId]()
                              { 
            handleTasks();
            REQUIRE(WppTaskQueue::isTaskExist(taskId));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            handleTasks(); 
            REQUIRE(WppTaskQueue::isTaskExist(taskId));
            std::this_thread::sleep_for(std::chrono::seconds(2));
            handleTasks(); 
            REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId)); });
        my_thread.join();

        clearWppTaskQueue();
    }

    SECTION("Task with double call")
    {
        int callCnt = 2;
        WppTaskQueue::task_t task = [&callCnt](WppClient &client, void *ctx)
        {
            return !callCnt--;
        };
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, task);
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        handleTasks();
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        handleTasks();
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        handleTasks();
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }
}

TEST_CASE("WppTaskQueue: Error Handling and Edge Cases")
{
    char str[] = "Hello, world!";
    WppTaskQueue::task_id_t taskId;
    REQUIRE(WppTaskQueue::getTaskCnt() == 0);
    REQUIRE(WPP_TASK_DEF_CTX == NULL);
    REQUIRE(WPP_ERR_TASK_ID == 0);

    SECTION("Task delay")
    {
        REQUIRE(WPP_TASK_MIN_DELAY_S <= WPP_TASK_DEF_DELAY_S);
        REQUIRE(WPP_TASK_DEF_DELAY_S <= WPP_TASK_MAX_DELAY_S);

        taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        taskId = WppTaskQueue::addTask(WPP_TASK_MAX_DELAY_S + 1, createDummyTask());
        REQUIRE(taskId == WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId));

        taskId = WppTaskQueue::addTask(str, WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        taskId = WppTaskQueue::addTask(str, WPP_TASK_MAX_DELAY_S + 1, createDummyTask());
        REQUIRE(taskId == WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId));

        taskId = WppTaskQueue::addTaskWithCopy(str, sizeof(str), WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        taskId = WppTaskQueue::addTaskWithCopy(str, sizeof(str), WPP_TASK_MAX_DELAY_S + 1, createDummyTask());
        REQUIRE(taskId == WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    SECTION("Task with null context")
    {
        taskId = WppTaskQueue::addTask(NULL, WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    SECTION("Copy task with null context")
    {
        taskId = WppTaskQueue::addTaskWithCopy(NULL, sizeof(str), WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId == WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    SECTION("Copy task with 0 size")
    {
        taskId = WppTaskQueue::addTaskWithCopy(str, 0, WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId == WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    SECTION("Check state with incorrect task id")
    {
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(WPP_ERR_TASK_ID));
        REQUIRE_FALSE(WppTaskQueue::isTaskIdle(WPP_ERR_TASK_ID));
        REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(WPP_ERR_TASK_ID));
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(WPP_ERR_TASK_ID));

        clearWppTaskQueue();
    }

    SECTION("Remove incorrect task id")
    {
        taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        WppTaskQueue::requestToRemoveTask(WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId));

        clearWppTaskQueue();
    }

    // Additional sections for other edge cases can be added here
}
