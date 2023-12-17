#include "catch_amalgamated.hpp"
#include "ObjSubject.h"

namespace wpp {
class Object : public ObjSubject {
public:
    Object() {}
    virtual ~Object() {}

    void operationNotify(Object &obj, ID_T instanceId, InstOp::TYPE type) {
        ObjSubject::operationNotify(obj, instanceId, type);
    }

    void observerDoAction(Object &obj, Action action) {
        ObjSubject::observerDoAction(obj, action);
    }
};

class ObjOpObserverTest : public ObjOpObserver {
public:
    int instanceCreatedCount = 0;
    int instanceDeletingCount = 0;

    void instanceCreated(Object &object, ID_T instanceId) override {
        ObjOpObserver::instanceCreated(object, instanceId);
        instanceCreatedCount++;
    }

    void instanceDeleting(Object &object, ID_T instanceId) override {
        ObjOpObserver::instanceDeleting(object, instanceId);
        instanceDeletingCount++;
    }
};

class ObjActObserverTest : public ObjActObserver {
public:
    int objectRestoreCount = 0;

    void objectRestore(Object &object) override {
        objectRestoreCount++;
    }
};
}

using namespace wpp;

TEST_CASE("ObjSubject subscribe/unsubscribe", "[opSubscribe][opUnsubscribe][actSubscribe][actUnsubscribe]") {
    Object obj;
    ObjOpObserverTest opObserver;
    ObjActObserverTest actObserver;

    SECTION("Operation subscribe/unsubscribe") {
        obj.opSubscribe(&opObserver);
        obj.opSubscribe(&opObserver);
        obj.opSubscribe(NULL);

        REQUIRE(opObserver.instanceCreatedCount == 0);
        obj.operationNotify(obj, 0, InstOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 1);
         obj.operationNotify(obj, 0, InstOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 2);

        obj.opUnsubscribe(&opObserver);
        obj.operationNotify(obj, 0, InstOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 2);
    }

    SECTION("Action subscribe/unsubscribe") {
        obj.actSubscribe(&actObserver);
        obj.actSubscribe(&actObserver);
        obj.actSubscribe(NULL);

        REQUIRE(actObserver.objectRestoreCount == 0);
        obj.observerDoAction(obj, ObjSubject::RESTORE);
        REQUIRE(actObserver.objectRestoreCount == 1);
         obj.observerDoAction(obj, ObjSubject::RESTORE);
        REQUIRE(actObserver.objectRestoreCount == 2);

        obj.actUnsubscribe(&actObserver);
        obj.observerDoAction(obj, ObjSubject::RESTORE);
        REQUIRE(actObserver.objectRestoreCount == 2);
    }

    SECTION("Operation notify") {
        obj.opSubscribe(&opObserver);

        REQUIRE(opObserver.instanceCreatedCount == 0);
        obj.operationNotify(obj, 0, InstOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 1);

        REQUIRE(opObserver.instanceDeletingCount == 0);
        obj.operationNotify(obj, 0, InstOp::DELETE);
        REQUIRE(opObserver.instanceDeletingCount == 1);

        obj.operationNotify(obj, 0, InstOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 2);
        obj.operationNotify(obj, 0, InstOp::DELETE);
        REQUIRE(opObserver.instanceCreatedCount == 2);

        ObjOpObserverTest opObservers[100];
        for (int i = 0; i < 100; i++) {
            obj.opSubscribe(&opObservers[i]);
        }

        obj.operationNotify(obj, 0, InstOp::CREATE);
        obj.operationNotify(obj, 0, InstOp::CREATE);
        obj.operationNotify(obj, 0, InstOp::DELETE);
        for (int i = 0; i < 100; i++) {
            REQUIRE(opObservers[i].instanceCreatedCount == 2);
            REQUIRE(opObservers[i].instanceDeletingCount == 1);
        }
        REQUIRE(opObserver.instanceCreatedCount == 4);
        REQUIRE(opObserver.instanceCreatedCount == 4);
    }

    SECTION("Do action notify") {
        obj.actSubscribe(&actObserver);

        REQUIRE(actObserver.objectRestoreCount == 0);
        obj.observerDoAction(obj, ObjSubject::RESTORE);
        REQUIRE(actObserver.objectRestoreCount == 1);
        obj.observerDoAction(obj, ObjSubject::RESTORE);
        REQUIRE(actObserver.objectRestoreCount == 2);

        ObjActObserverTest actObservers[100];
        for (int i = 0; i < 100; i++) {
            obj.actSubscribe(&actObservers[i]);
        }

        obj.observerDoAction(obj, ObjSubject::RESTORE);
        obj.observerDoAction(obj, ObjSubject::RESTORE);
        obj.observerDoAction(obj, ObjSubject::RESTORE);
        for (int i = 0; i < 100; i++) {
            REQUIRE(actObservers[i].objectRestoreCount == 3);
        }
        REQUIRE(actObserver.objectRestoreCount == 5);
    }
}   