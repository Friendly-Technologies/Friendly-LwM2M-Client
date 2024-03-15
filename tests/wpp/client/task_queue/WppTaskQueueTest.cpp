#include "catch_amalgamated.hpp"
#include "WppTaskQueue.h"

using namespace wpp;

TEST_CASE("WppTaskQueue Methods", "[WppTaskQueue]") {
    
    SECTION("Add Task") {
        std::string myText = "Hello, world!";
        // Arrange
        WppTaskQueue::task_t sampleTask = [](WppClient&, WppTaskQueue::ctx_t) { return true; };
        time_t delaySec = 10;
        // Act
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(delaySec, sampleTask);
        WppTaskQueue::task_id_t taskId2 = WppTaskQueue::addTask(&myText, 20, sampleTask);
        WppTaskQueue::task_id_t taskId3 = WppTaskQueue::addTaskWithCopy(&myText,20, 20, sampleTask);
        // Assert
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        REQUIRE(WppTaskQueue::isTaskIdle(taskId));
        REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId2));
        REQUIRE_FALSE(WppTaskQueue::isTaskShouldBeDeleted(taskId3));
        REQUIRE(WppTaskQueue::getTaskCnt()==3);
    }

    SECTION("Remove Task") {
        // Arrange
        WppTaskQueue::task_t sampleTask = [](WppClient&, WppTaskQueue::ctx_t) { return true; };
        time_t delaySec = 10;
        WppTaskQueue::task_id_t taskId = WppTaskQueue::addTask(delaySec, sampleTask);
        // Act
        WppTaskQueue::requestToRemoveTask(taskId);
        WppTaskQueue::requestToRemoveEachTask();
        // Assert
        REQUIRE(WppTaskQueue::isTaskExist(taskId));
        REQUIRE(WppTaskQueue::isTaskIdle(taskId));
        REQUIRE_FALSE(WppTaskQueue::isTaskExecuting(taskId));
        REQUIRE(WppTaskQueue::isTaskShouldBeDeleted(taskId));
    }
}
