#include "catch_amalgamated.hpp"
#include "InstSubject.h"

using namespace wpp;

namespace wpp {

class Instance : public InstSubject {
public:
    Instance() {}
    ~Instance() {}

    void operationNotify(Instance &inst, const ResLink &resId, ItemOp::TYPE type) {
        InstSubject::operationNotify(inst, resId, type);
    }

    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    void blockOperationNotify(Instance &inst, const ResLink &resId, ItemOp::TYPE type, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) {
        InstSubject::blockOperationNotify(inst, resId, type, buff, blockNum, isLastBlock);
    }
    #endif

    void eventNotify(Instance &inst, EVENT_ID_T eventId) {
        InstSubject::eventNotify(inst, eventId);
    }
};

class InstOpObserverTest : public InstOpObserver {
public:
    int resourceWriteCount = 0;
    int resourceExecuteCount = 0;
    int resourcesReplacedCount = 0;

    void resourceWrite(Instance &inst, const ResLink &resource) override {
        InstOpObserver::resourceWrite(inst, resource);
        resourceWriteCount++;
    }
    void resourceExecute(Instance &inst, const ResLink &resource) override {
        InstOpObserver::resourceExecute(inst, resource);
        resourceExecuteCount++;
    }
    // void resourcesReplaced(Instance &inst) override {
    //     InstOpObserver::resourcesReplaced(inst);
    //     resourcesReplacedCount++;
    // }
};

class InstEventObserverTest : public InstEventObserver {
public:
    int instEventCount = 0;

    void instEvent(Instance &inst, EVENT_ID_T eventId) override {
        InstEventObserver::instEvent(inst, eventId);
        instEventCount++;
    }
};

#ifdef LWM2M_RAW_BLOCK1_REQUESTS
class InstBlockOpObserverTest : public InstBlockOpObserver {
public:
    int resourceBlockWriteCount = 0;
    int resourceBlockExecuteCount = 0;

    void resourceBlockWrite(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) override {
        InstBlockOpObserver::resourceBlockWrite(inst, resource, buff, blockNum, isLastBlock);
        resourceBlockWriteCount++;
    }
    void resourceBlockExecute(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) override {
        InstBlockOpObserver::resourceBlockExecute(inst, resource, buff, blockNum, isLastBlock);
        resourceBlockExecuteCount++;
    }
};
#endif

}

TEST_CASE("InstSubject: subscribe/unsubscribe", "[opSubscribe][opUnsubscribe][blockOpSubscribe][blockOpUnsubscribe][eventSubscribe][eventUnsubscribe]") {
    Instance inst;
    InstOpObserverTest opObserver;
    InstEventObserverTest eventObserver;
    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    InstBlockOpObserverTest blockOpObserver;
    #endif

    SECTION("Instance operation subscribe/unsubscribe") {
        inst.opSubscribe(&opObserver);
        inst.opSubscribe(&opObserver);
        inst.opSubscribe(NULL);
        inst.operationNotify(inst, {}, ItemOp::TYPE::READ);
        inst.opUnsubscribe(&opObserver);
        inst.operationNotify(inst, {}, ItemOp::TYPE::READ);
    }

    SECTION("Instance event subscribe/unsubscribe") {
        inst.eventSubscribe(&eventObserver);
        inst.eventSubscribe(&eventObserver);
        inst.eventSubscribe(NULL);

        REQUIRE(eventObserver.instEventCount == 0);
        inst.eventNotify(inst, 0);
        REQUIRE(eventObserver.instEventCount == 1);
        inst.eventNotify(inst, 0);
        REQUIRE(eventObserver.instEventCount == 2);

        inst.eventUnsubscribe(&eventObserver);
        inst.eventNotify(inst, 0);
        REQUIRE(eventObserver.instEventCount == 2);
    }

    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    SECTION("Instance block operation subscribe/unsubscribe") {
        inst.blockOpSubscribe(&blockOpObserver);
        inst.blockOpSubscribe(&blockOpObserver);
        inst.blockOpSubscribe(NULL);

        REQUIRE(blockOpObserver.resourceBlockWriteCount == 0);
        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_WRITE, {}, 0, false);
        REQUIRE(blockOpObserver.resourceBlockWriteCount == 1);
        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_WRITE, {}, 0, false);
        REQUIRE(blockOpObserver.resourceBlockWriteCount == 2);

        inst.blockOpUnsubscribe(&blockOpObserver);
        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_WRITE, {}, 0, false);
        REQUIRE(blockOpObserver.resourceBlockWriteCount == 2);
    }
    #endif
}

TEST_CASE("InstSubject: operations and events notifying", "[operationNotify][blockOperationNotify][eventNotify]") {
    Instance inst;
    InstOpObserverTest opObserver;
    InstEventObserverTest eventObserver;
    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    InstBlockOpObserverTest blockOpObserver;
    #endif

    SECTION("Instance operation notify") {
        inst.opSubscribe(&opObserver);
        inst.operationNotify(inst, {}, ItemOp::TYPE::READ);
        
        REQUIRE(opObserver.resourceWriteCount == 0);
        inst.operationNotify(inst, {}, ItemOp::TYPE::WRITE);
        REQUIRE(opObserver.resourceWriteCount == 1);
        inst.operationNotify(inst, {}, ItemOp::TYPE::WRITE);
        REQUIRE(opObserver.resourceWriteCount == 2);

        REQUIRE(opObserver.resourceExecuteCount == 0);
        inst.operationNotify(inst, {}, ItemOp::TYPE::EXECUTE);
        REQUIRE(opObserver.resourceExecuteCount == 1);
        inst.operationNotify(inst, {}, ItemOp::TYPE::EXECUTE);
        REQUIRE(opObserver.resourceExecuteCount == 2);

        REQUIRE(opObserver.resourcesReplacedCount == 0);
        inst.operationNotify(inst, {}, ItemOp::TYPE::WRITE);
        REQUIRE(opObserver.resourcesReplacedCount == 0);
        inst.operationNotify(inst, {}, ItemOp::TYPE::WRITE);
        REQUIRE(opObserver.resourcesReplacedCount == 0);

        inst.operationNotify(inst, {}, ItemOp::TYPE::NONE);
        REQUIRE(opObserver.resourceWriteCount == 4);
        REQUIRE(opObserver.resourceExecuteCount == 2);
        REQUIRE(opObserver.resourcesReplacedCount == 0);

        InstOpObserverTest opObservers[100];
        for (int i = 0; i < 100; i++) {
            inst.opSubscribe(&opObservers[i]);
        }

        inst.operationNotify(inst, {}, ItemOp::TYPE::READ);
        inst.operationNotify(inst, {}, ItemOp::TYPE::WRITE);
        inst.operationNotify(inst, {}, ItemOp::TYPE::WRITE);
        inst.operationNotify(inst, {}, ItemOp::TYPE::EXECUTE);
        for (int i = 0; i < 100; i++) {
            REQUIRE(opObservers[i].resourceWriteCount == 2);
            REQUIRE(opObservers[i].resourceExecuteCount == 1);
            REQUIRE(opObservers[i].resourcesReplacedCount == 0);
        }
        REQUIRE(opObserver.resourceWriteCount == 6);
        REQUIRE(opObserver.resourceExecuteCount == 3);
        REQUIRE(opObserver.resourcesReplacedCount == 0);
    }

    SECTION("Instance event notify") {
        inst.eventSubscribe(&eventObserver);

        REQUIRE(eventObserver.instEventCount == 0);
        inst.eventNotify(inst, 0);
        REQUIRE(eventObserver.instEventCount == 1);
        inst.eventNotify(inst, 0);
        REQUIRE(eventObserver.instEventCount == 2);

        InstEventObserverTest eventObservers[100];
        for (int i = 0; i < 100; i++) {
            inst.eventSubscribe(&eventObservers[i]);
        }

        inst.eventNotify(inst, 0);
        inst.eventNotify(inst, 0);
        inst.eventNotify(inst, 0);
        for (int i = 0; i < 100; i++) {
            REQUIRE(eventObservers[i].instEventCount == 3);
        }
        REQUIRE(eventObserver.instEventCount == 5);
    }

    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    SECTION("Instance block operation notify") {
        inst.blockOpSubscribe(&blockOpObserver);

        REQUIRE(blockOpObserver.resourceBlockWriteCount == 0);
        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_WRITE, {}, 0, false);
        REQUIRE(blockOpObserver.resourceBlockWriteCount == 1);
        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_WRITE, {}, 0, false);
        REQUIRE(blockOpObserver.resourceBlockWriteCount == 2);

        REQUIRE(blockOpObserver.resourceBlockExecuteCount == 0);
        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_EXECUTE, {}, 0, false);
        REQUIRE(blockOpObserver.resourceBlockExecuteCount == 1);
        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_EXECUTE, {}, 0, false);
        REQUIRE(blockOpObserver.resourceBlockExecuteCount == 2);

        inst.blockOperationNotify(inst, {}, ItemOp::NONE, {}, 0, false);
        REQUIRE(blockOpObserver.resourceBlockWriteCount == 2);
        REQUIRE(blockOpObserver.resourceBlockExecuteCount == 2);

        InstBlockOpObserverTest blockOpObservers[100];
        for (int i = 0; i < 100; i++) {
            inst.blockOpSubscribe(&blockOpObservers[i]);
        }

        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_EXECUTE, {}, 0, false);
        inst.blockOperationNotify(inst, {}, ItemOp::BLOCK_WRITE, {}, 0, false);
        for (int i = 0; i < 100; i++) {
            REQUIRE(blockOpObservers[i].resourceBlockWriteCount == 1);
            REQUIRE(blockOpObservers[i].resourceBlockExecuteCount == 1);
        }
        REQUIRE(blockOpObserver.resourceBlockWriteCount == 3);
        REQUIRE(blockOpObserver.resourceBlockExecuteCount == 3);
    }
    #endif
}