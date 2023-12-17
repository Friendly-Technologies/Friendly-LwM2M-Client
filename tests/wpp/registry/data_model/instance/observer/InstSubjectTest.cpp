#include "catch_amalgamated.hpp"
#include "InstSubject.h"

namespace wpp {

class Instance {
public:
    Instance() {}
    ~Instance() {}
};

class InstOpObserverTest : public InstOpObserver {
public:
    int resourceReadCount = 0;
    int resourceWriteCount = 0;
    int resourceExecuteCount = 0;
    int resourcesReplacedCount = 0;

    void resourceRead(Instance &inst, const ResLink &resource) override {
        InstOpObserver::resourceRead(inst, resource);
    }
    void resourceWrite(Instance &inst, const ResLink &resource) override {
        InstOpObserver::resourceWrite(inst, resource);
    }
    void resourceExecute(Instance &inst, const ResLink &resource) override {
        InstOpObserver::resourceExecute(inst, resource);
    }
    void resourcesReplaced(Instance &inst) override {
        InstOpObserver::resourcesReplaced(inst);
    }
};

class InstEventObserverTest : public InstEventObserver {
public:
    int instEventCount = 0;

    void instEvent(Instance &inst, EVENT_ID_T eventId) override {
        InstEventObserver::instEvent(inst, eventId);
    }
};

class InstBlockOpObserverTest : public InstBlockOpObserver {
public:
    int resourceBlockWriteCount = 0;
    int resourceBlockExecuteCount = 0;

    void resourceBlockWrite(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) override {
        InstBlockOpObserver::resourceBlockWrite(inst, resource, buff, blockNum, isLastBlock);
    }
    void resourceBlockExecute(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) override {
        InstBlockOpObserver::resourceBlockExecute(inst, resource, buff, blockNum, isLastBlock);
    }
};

}

TEST_CASE("") {}