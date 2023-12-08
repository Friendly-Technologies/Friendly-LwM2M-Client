#include "catch_amalgamated.hpp"
#include "SafeQueue.h"

// Helper function to fill a SafeQueue with elements
template <typename T, size_t SIZE>
void fillQueue(SafeQueue<T, SIZE>& queue, const T& value, size_t count = SIZE) {
    for (size_t i = 0; i < count; ++i) {
        queue.push(&value);
    }
}

TEST_CASE("SafeQueue Constructor", "[constructor]") {
    SafeQueue<int, 5> queue;
    REQUIRE(queue.is_empty());
    REQUIRE(queue.size() == 0);
}

TEST_CASE("SafeQueue push and pop", "[push][pop]") {
    SafeQueue<int, 5> queue;
    int value = 42;

    SECTION("Push elements into the queue") {
        REQUIRE(queue.push(&value));
        REQUIRE(queue.size() == 1);
    }

    SECTION("Push into a full queue") {
        fillQueue(queue, value);
        REQUIRE_FALSE(queue.push(&value));  // Should fail as the queue is full
    }

    SECTION("Pop from the queue") {
        fillQueue(queue, value);
        REQUIRE(queue.pop());
        REQUIRE(queue.size() == 4);
    }

    SECTION("Pop from an empty queue") {
        REQUIRE_FALSE(queue.pop());  // Should fail as the queue is empty
    }
}

TEST_CASE("SafeQueue to_vector", "[to_vector]") {
    SafeQueue<int, 5> queue;
    int value = 42;
    fillQueue(queue, value);

    auto vec = queue.to_vector();
    REQUIRE(vec.size() == 1);
    REQUIRE(std::all_of(vec.begin(), vec.end(), [&value](int v) { return v == value; }));
}

TEST_CASE("SafeQueue element access", "[front][back][at]") {
    SafeQueue<int, 5> queue;
    int values[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        queue.push(&values[i]);
    }

    SECTION("Access front element") {
        REQUIRE(*queue.front() == 1);
    }

    SECTION("Access back element") {
        REQUIRE(*queue.back() == 5);
    }

    SECTION("Access element at index") {
        REQUIRE(*queue.at(3) == 4);
    }

    SECTION("Access element in empty queue") {
        SafeQueue<int, 5> emptyQueue;
        REQUIRE(emptyQueue.front() == nullptr);
        REQUIRE(emptyQueue.back() == nullptr);
        REQUIRE(emptyQueue.at(0) == nullptr);
    }
}

TEST_CASE("SafeQueue state checks", "[is_empty][is_full][size][empty_size][element_size]") {
    SafeQueue<int, 5> queue;
    int value = 42;

    SECTION("Check empty state") {
        REQUIRE(queue.is_empty());
        queue.push(&value);
        REQUIRE_FALSE(queue.is_empty());
    }

    SECTION("Check full state") {
        fillQueue(queue, value);
        REQUIRE(queue.is_full());
    }

    SECTION("Check size") {
        fillQueue(queue, value, 3);
        REQUIRE(queue.size() == 3);
    }

    SECTION("Check empty size") {
        fillQueue(queue, value, 3);
        REQUIRE(queue.empty_size() == 2);
    }

    SECTION("Check element size") {
        REQUIRE(queue.element_size() == sizeof(int));
    }
}
