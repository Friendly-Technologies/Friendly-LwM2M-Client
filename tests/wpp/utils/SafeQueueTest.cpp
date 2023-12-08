#include "catch_amalgamated.hpp"
#include "SafeQueue.h"
#include <iostream>

TEST_CASE("SafeQueue Constructor", "[constructor]") {
    SafeQueue<int, 5> queue;
    REQUIRE(queue.is_empty());
    REQUIRE(queue.size() == 0);
}

TEST_CASE("SafeQueue push and pop", "[push][pop]") {
    SafeQueue<int, 5> queue;
    int values[5] = {1, 2, 3, 4, 5};

    SECTION("Push one element into the queue") {
        REQUIRE(queue.is_empty());
        REQUIRE(queue.push(values));
        REQUIRE(queue.size() == 1);
        REQUIRE(*queue.at(0) == values[0]);
    }

    SECTION("Push multiple elements into the queue as array") {
        REQUIRE(queue.is_empty());
        REQUIRE(queue.push(values, 5));
        for (int i = 0; i < 5; ++i) {
            REQUIRE(*queue.at(i) == values[i]);
        }
    }

    SECTION("Push multiple elements into the queue one by one") {
        REQUIRE(queue.is_empty());
        for (size_t i = 0; i < 5; ++i) {
            REQUIRE(queue.push(&values[i]));
        }
        for (int i = 0; i < 5; ++i) {
            REQUIRE(*queue.at(i) == values[i]);
        }
    }

    SECTION("Push into a full queue one element") {
        int value = 256;
        REQUIRE(queue.is_empty());
        REQUIRE(queue.push(values, 5));
        REQUIRE(queue.size() == 5);
        REQUIRE_FALSE(queue.push(&value));
    }

    SECTION("Push into a queue array of elements with overhead") {
        REQUIRE(queue.is_empty());
        REQUIRE(queue.push(values, 3));
        REQUIRE(queue.size() == 3);
        REQUIRE_FALSE(queue.push(values, 5));
        REQUIRE(queue.size() == 3);
        REQUIRE(queue.push(values, 2));
        REQUIRE(queue.size() == 5);
        REQUIRE_FALSE(queue.push(values, 5));
        REQUIRE(queue.size() == 5);
    }

    SECTION("Pop one element from the queue") {
        REQUIRE(queue.is_empty());
        queue.push(values, 5);
        REQUIRE(queue.size() == 5);
        REQUIRE(queue.pop());
        REQUIRE(queue.size() == 4);
        for (int i = 1; i < 5; ++i) {
            REQUIRE(*queue.at(i-1) == values[i]);
        }
    }

    SECTION("Pop elements from the queue one by one") {
        REQUIRE(queue.is_empty());
        queue.push(values, 5);
        REQUIRE(queue.size() == 5);
        REQUIRE(queue.pop());
        REQUIRE(queue.size() == 4);
        REQUIRE(queue.pop());
        REQUIRE(queue.size() == 3);
        REQUIRE(queue.pop());
        REQUIRE(queue.size() == 2);
        REQUIRE(queue.pop());
        REQUIRE(queue.size() == 1);
        REQUIRE(queue.pop());
        REQUIRE(queue.size() == 0);
    }

    SECTION("Pop elements from the queue as set") {
        REQUIRE(queue.is_empty());
        queue.push(values, 5);
        REQUIRE(queue.size() == 5);
        REQUIRE(queue.pop(3));
        REQUIRE(queue.size() == 2);
        for (int i = 3; i < 5; ++i) {
            REQUIRE(*queue.at(i-3) == values[i]);
        }
        REQUIRE(queue.pop(2));
        REQUIRE(queue.size() == 0);
    }

    SECTION("Pop from an empty queue") {
        REQUIRE(queue.is_empty());
        REQUIRE_FALSE(queue.pop());
    }

    SECTION("Pop from an empty queue") {
        REQUIRE(queue.is_empty());
        queue.push(values, 5);
    }
}

TEST_CASE("SafeQueue to_vector", "[to_vector]") {
    SafeQueue<int, 5> queue;

    SECTION("Convert empty queue to vector") {
        REQUIRE(queue.size() == 0);
        auto vec = queue.to_vector();
        REQUIRE(vec.size() == 0);
    }

    SECTION("Convert not empty queue to vector") {
        int values[7] = {1, 2, 3, 4, 5, 6, 7};
        REQUIRE(queue.push(values, 5));
        REQUIRE(queue.size() == 5);
        for (int i = 0; i < 5; ++i) {
            REQUIRE(*queue.at(i) == values[i]);
        }
        
        auto vec = queue.to_vector(5);
        REQUIRE(vec.size() == 5);
        REQUIRE(std::equal(vec.begin(), vec.end(), values));

        REQUIRE(queue.pop());
        REQUIRE(queue.pop());
        REQUIRE(queue.size() == 3);
        REQUIRE(queue.push(values + 5, 2));
        REQUIRE(queue.size() == 5);

        vec = queue.to_vector(5);
        REQUIRE(vec.size() == 5);
        REQUIRE(std::equal(vec.begin(), vec.end(), values + 2));
    }
}

TEST_CASE("SafeQueue element access", "[front][back][at]") {
    SafeQueue<int, 5> queue;
    int values[5] = {1, 2, 3, 4, 5};
    REQUIRE(queue.push(values, 5));

    SECTION("Access front element by ptr") {
        REQUIRE(*queue.front() == 1);
    }

    SECTION("Access back element by ptr") {
        REQUIRE(*queue.back() == 5);
    }

    SECTION("Access element at index by ptr") {
        REQUIRE(*queue.at(3) == 4);
    }

    SECTION("Access element at out of range index by ptr") {
        REQUIRE(queue.at(256) == NULL);
    }

    SECTION("Access front element by value") {
        int value = 0;
        REQUIRE(queue.front(&value));
        REQUIRE(value == 1);
    }

    SECTION("Access back element by value") {
        int value = 0;
        REQUIRE(queue.back(&value));
        REQUIRE(value == 5);
    }

    SECTION("Access element at index by value") {
        int value = 0;
        REQUIRE(queue.at(3, &value));
        REQUIRE(value == 4);
    }

    SECTION("Access element at out of range index by value") {
        int value = 0;
        REQUIRE_FALSE(queue.at(256, &value));
    }

    SECTION("Access element in empty queue by ptr") {
        SafeQueue<int, 5> emptyQueue;
        REQUIRE(emptyQueue.front() == NULL);
        REQUIRE(emptyQueue.back() == NULL);
        REQUIRE(emptyQueue.at(0) == NULL);
    }

    SECTION("Access element in empty queue by value") {
        SafeQueue<int, 5> emptyQueue;
        int value = 0;
        REQUIRE_FALSE(emptyQueue.front(&value));
        REQUIRE_FALSE(emptyQueue.back(&value));
        REQUIRE_FALSE(emptyQueue.at(0, &value));
    }
}

TEST_CASE("SafeQueue state checks", "[is_empty][is_full][size][empty_size][element_size]") {
    SafeQueue<int, 5> queue;
    int values[5] = {1, 2, 3, 4, 5};

    SECTION("Check empty state") {
        REQUIRE(queue.is_empty());
        queue.push(values);
        REQUIRE_FALSE(queue.is_empty());
    }

    SECTION("Check full state") {
        REQUIRE_FALSE(queue.is_full());
        REQUIRE(queue.push(values));
        REQUIRE_FALSE(queue.is_full());
        REQUIRE(queue.push(values, 4));
        REQUIRE(queue.is_full());
    }

    SECTION("Check size") {
        REQUIRE(queue.size() == 0);
        REQUIRE(queue.push(values, 3));
        REQUIRE(queue.size() == 3);
        REQUIRE(queue.push(values));
        REQUIRE(queue.size() == 4);
    }

    SECTION("Check empty size") {
        REQUIRE(queue.empty_size() == 5);
        REQUIRE(queue.push(values, 3));
        REQUIRE(queue.empty_size() == 2);
        REQUIRE(queue.push(values, 2));
        REQUIRE(queue.empty_size() == 0);
    }

    SECTION("Check element size") {
        struct TestStruct {
            int a;
            int b;
            char c[5];
            float f;
        };
        SafeQueue<TestStruct, 5> queueOfStructs;
        REQUIRE(queueOfStructs.element_size() == sizeof(TestStruct));
        REQUIRE(queue.element_size() == sizeof(int));
    }
}
