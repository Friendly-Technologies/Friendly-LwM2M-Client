#include "catch_amalgamated.hpp"
#include "WppTaskQueue.h"
#include <thread>
#include <iostream>

namespace wpp {
    class WppClient {
    public:
        WppClient() {}
        ~WppClient() {}
    };
}

using namespace wpp;

static time_t handleTasks() {
    WppClient client;
    time_t next_call = WppTaskQueue::handleEachTask(client);
    return next_call;
}

static void clearWppTaskQueue() {
    WppTaskQueue::requestToRemoveEachTask();
    handleTasks();
}

static WppTaskQueue::task_t createDummyTask() {
    return [](WppClient&, WppTaskQueue::ctx_t) { return true; };
}

TEST_CASE("Task creating", "[addTask][addTaskWithCopy]") {
    char str[] = "Hello, world!";
    REQUIRE(WppTaskQueue::getTaskCnt() == 0);

    SECTION("Add Task without context") {
        for (size_t i = 0; i < 100; i++) {
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

    SECTION("Add Task with context") {
        for (size_t i = 0; i < 100; i++) {
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

    SECTION("Add Task with copied context") {
        for (size_t i = 0; i < 100; i++) {
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

TEST_CASE("Task removing", "[requestToRemoveTask][requestToRemoveEachTask]") {
    REQUIRE(WppTaskQueue::getTaskCnt() == 0);

    SECTION("Remove Task") {
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

    SECTION("Remove each task") {
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
}

TEST_CASE("Task state checking", "[isTaskExist][isTaskIdle][isTaskExecuting][isTaskShouldBeDeleted]") {
    REQUIRE(WppTaskQueue::getTaskCnt() == 0);

    SECTION("Is task exist") {
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    SECTION("Is task idle") {
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskIdle(taskId));

        clearWppTaskQueue();
    }

    SECTION("Is task executing") {
        WppTaskQueue::task_t task = [](WppClient& client, WppTaskQueue::ctx_t ctx) {
            std::cout << "!!!!!1 Task executing" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(10));
            std::cout << "!!!!!2 Task executing" << std::endl;
            return true;
        };

        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, task);
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId));
        REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId));
        std::thread my_thread([](){ handleTasks(); });
        std::cout << "!!!!! Thread created" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "!!!!! Checking task" << std::endl;
        REQUIRE(WppTaskQueue::isTaskExecuting(taskId));
        my_thread.join();
        std::cout << "!!!!! Thread joined" << std::endl;
        REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId));

        clearWppTaskQueue();
    }

    SECTION("Is task should be deleted") {
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId != WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId));
        WppTaskQueue::requestToRemoveTask(taskId);
        REQUIRE(WppTaskQueue::isTaskShouldBeDeleted(taskId));

        clearWppTaskQueue();
    }
}

TEST_CASE("Error Handling and Edge Cases") {
    char str[] = "Hello, world!";
    WppTaskQueue::task_id_t taskId;
    REQUIRE(WppTaskQueue::getTaskCnt() == 0);

    SECTION("Task delay") {
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

    SECTION("Task with null context") {
        taskId = WppTaskQueue::addTask(NULL, WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    SECTION("Copy task with null context") {
        taskId = WppTaskQueue::addTaskWithCopy(NULL, sizeof(str), WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId == WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    SECTION("Copy task with 0 size") {
        taskId = WppTaskQueue::addTaskWithCopy(str, 0, WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(taskId == WPP_ERR_TASK_ID);
        REQUIRE_FALSE(WppTaskQueue::isTaskExist(taskId));

        clearWppTaskQueue();
    }

    // Additional sections for other edge cases can be added here
}
