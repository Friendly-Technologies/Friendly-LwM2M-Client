#include "catch_amalgamated.hpp"
#include "ObjSubject.h"

namespace wpp
{
    class Object : public ObjSubject
    {
    public:
        Object() {}
        virtual ~Object() {}

        void operationNotify(Object &obj, ID_T instanceId, ItemOp::TYPE type) { ObjSubject::operationNotify(obj, instanceId, type); }
    };

    class ObjOpObserverTest : public ObjOpObserver
    {
    public:
        int instanceCreatedCount = 0;
        int instanceDeletingCount = 0;

        void instanceCreated(Object &object, ID_T instanceId) override
        {
            ObjOpObserver::instanceCreated(object, instanceId);
            instanceCreatedCount++;
        }

        void instanceDeleting(Object &object, ID_T instanceId) override
        {
            ObjOpObserver::instanceDeleting(object, instanceId);
            instanceDeletingCount++;
        }
    };
}

using namespace wpp;

TEST_CASE("ObjSubject: subscribe/unsubscribe", "[opSubscribe][opUnsubscribe][actSubscribe][actUnsubscribe]")
{
    Object obj;
    ObjOpObserverTest opObserver;

    SECTION("Operation subscribe/unsubscribe")
    {
        obj.opSubscribe(&opObserver);
        obj.opSubscribe(&opObserver);
        obj.opSubscribe(NULL);

        REQUIRE(opObserver.instanceCreatedCount == 0);
        obj.operationNotify(obj, 0, ItemOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 1);
        obj.operationNotify(obj, 0, ItemOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 2);

        obj.opUnsubscribe(&opObserver);
        obj.operationNotify(obj, 0, ItemOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 2);
    }

    SECTION("Action subscribe/unsubscribe")
    {
        obj.opSubscribe(&opObserver);
        obj.opSubscribe(&opObserver);
        obj.opSubscribe(NULL);
        obj.opUnsubscribe(&opObserver);
    }
}

TEST_CASE("ObjSubject: operation notify and request for action", "[operationNotify][observerDoAction]")
{
    Object obj;
    ObjOpObserverTest opObserver;

    SECTION("Operation notify")
    {
        obj.opSubscribe(&opObserver);

        REQUIRE(opObserver.instanceCreatedCount == 0);
        obj.operationNotify(obj, 0, ItemOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 1);

        REQUIRE(opObserver.instanceDeletingCount == 0);
        obj.operationNotify(obj, 0, ItemOp::DELETE);
        REQUIRE(opObserver.instanceDeletingCount == 1);

        obj.operationNotify(obj, 0, ItemOp::CREATE);
        REQUIRE(opObserver.instanceCreatedCount == 2);
        obj.operationNotify(obj, 0, ItemOp::DELETE);
        REQUIRE(opObserver.instanceCreatedCount == 2);

        ObjOpObserverTest opObservers[100];
        for (int i = 0; i < 100; i++)
        {
            obj.opSubscribe(&opObservers[i]);
        }

        obj.operationNotify(obj, 0, ItemOp::CREATE);
        obj.operationNotify(obj, 0, ItemOp::CREATE);
        obj.operationNotify(obj, 0, ItemOp::DELETE);
        for (int i = 0; i < 100; i++)
        {
            REQUIRE(opObservers[i].instanceCreatedCount == 2);
            REQUIRE(opObservers[i].instanceDeletingCount == 1);
        }
        REQUIRE(opObserver.instanceCreatedCount == 4);
        REQUIRE(opObserver.instanceCreatedCount == 4);
    }
}