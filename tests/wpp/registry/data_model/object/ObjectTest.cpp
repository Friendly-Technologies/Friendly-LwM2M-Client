#include "catch_amalgamated.hpp"
#include "Object.h"
// #include "ObjectImpl.h"

using namespace wpp;

class ObjectMock : public Object
{
public:
    ObjectMock(lwm2m_context_t &context) : Object(context) {}

    ObjectMock(lwm2m_context_t &context, const ObjectInfo &info) : Object(context, info) {}
    ~ObjectMock(){};

    Instance *createInstance(ID_T instanceID) override { return 0; }
    Instance *instance(ID_T instanceID = 0) { return Object::instance(instanceID); }
    lwm2m_context_t &getContext() { return _context; }
    ID_T getFirstAvailableInstanceID() { return Object::getFirstAvailableInstanceID(); }

    uint8_t serverRead_clb(lwm2m_context_t *contextP, lwm2m_server_t *server, ID_T instanceId, int *numDataP, lwm2m_data_t **dataArrayP, lwm2m_object_t *objectP) { return Object::serverRead_clb(contextP, server, instanceId, numDataP, dataArrayP, objectP); }
    uint8_t serverWrite_clb(lwm2m_context_t *contextP, lwm2m_server_t *server, ID_T instanceId, int numData, lwm2m_data_t *dataArray, lwm2m_object_t *objectP, lwm2m_write_type_t writeType) { return Object::serverWrite_clb(contextP, server, instanceId, numData, dataArray, objectP, writeType); }
    uint8_t serverExecute_clb(lwm2m_context_t *contextP, lwm2m_server_t *server, ID_T instanceId, ID_T resId, uint8_t *buffer, int length, lwm2m_object_t *objectP) { return Object::serverExecute_clb(contextP, server, instanceId, resId, buffer, length, objectP); }
    uint8_t serverDiscover_clb(lwm2m_context_t *contextP, lwm2m_server_t *server, ID_T instanceId, int *numDataP, lwm2m_data_t **dataArrayP, lwm2m_object_t *objectP) { return Object::serverDiscover_clb(contextP, server, instanceId, numDataP, dataArrayP, objectP); }
    uint8_t serverCreate_clb(lwm2m_context_t *contextP, lwm2m_server_t *server, ID_T instanceId, int numData, lwm2m_data_t *dataArray, lwm2m_object_t *objectP) { return Object::serverCreate_clb(contextP, server, instanceId, numData, dataArray, objectP); }
    uint8_t serverDelete_clb(lwm2m_context_t *contextP, lwm2m_server_t *server, ID_T instanceId, lwm2m_object_t *objectP) { return Object::serverDelete_clb(contextP, server, instanceId, objectP); }

    WppRegistry &getRegistry() { return Object::getRegistry(); }
};

ObjectInfo mockInfo = {
    "Test Object",
    OBJ_ID::MAX_ID,
    "urn:oma:lwm2m:oma:123:1",
    Version{1, 0},
    Version{1, 0},
    IS_SINGLE::MULTIPLE,
    IS_MANDATORY::OPTIONAL,
    ItemOp(ItemOp::CREATE | ItemOp::DELETE),
};

class InstanceMock : public Instance
{
public:
    InstanceMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Instance(context, id) {}

    void setDefaultState() {}
    void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) override {}
    void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) override {}
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

lwm2m_context_t mockContextObj = {};
static lwm2m_server_t mockServer = {};
ObjectMock obj(mockContextObj, mockInfo);
ObjectMock obj2(mockContextObj);

TEST_CASE("Object: constructing/destucting and getting infor", "[Object][Object][getObjectID][getLwm2mObject][getObjectInfo]")
{

    SECTION("Constructor")
    {
        REQUIRE(obj.getObjectID() == OBJ_ID::MAX_ID);
        REQUIRE(obj2.getObjectID() == 0);
        REQUIRE(std::strcmp(obj.getObjectInfo().name, "Test Object") == 0);
        REQUIRE(obj.getObjectInfo().objID == OBJ_ID::MAX_ID);
        REQUIRE(std::strcmp(obj.getObjectInfo().urn, "urn:oma:lwm2m:oma:123:1") == 0);
        REQUIRE(obj.getObjectInfo().lwm2mVersion.minor == 0);
        REQUIRE(obj.getObjectInfo().lwm2mVersion.major == 1);
        REQUIRE(obj.getObjectInfo().objVersion.minor == 0);
        REQUIRE(obj.getObjectInfo().objVersion.major == 1);
        REQUIRE(obj.getObjectInfo().isSingle == IS_SINGLE::MULTIPLE);
        REQUIRE(obj.getObjectInfo().isMandatory == IS_MANDATORY::OPTIONAL);
        REQUIRE(obj.getObjectInfo().operations.getFlags() == ItemOp(ItemOp::CREATE | ItemOp::DELETE).getFlags());
        REQUIRE(obj.getObjectInfo().operations.getFlags() != ItemOp(ItemOp::READ |
                                                                    ItemOp::WRITE |
                                                                    ItemOp::DISCOVER |
                                                                    ItemOp::EXECUTE |
                                                                    ItemOp::DELETE)
                                                                 .getFlags());

        REQUIRE(obj.getFirstAvailableInstanceID() == 0);
        lwm2m_object_t lwm2mObj = obj.getLwm2mObject();
        REQUIRE(lwm2mObj.objID == (ID_T)OBJ_ID::MAX_ID);
        REQUIRE(lwm2mObj.instanceList == NULL);
        REQUIRE(lwm2mObj.versionMajor == 1);
        REQUIRE(lwm2mObj.versionMinor == 0);
        REQUIRE(lwm2mObj.userData == &obj);
        REQUIRE(lwm2mObj.readFunc == NULL);
        REQUIRE(lwm2mObj.discoverFunc == NULL);
        REQUIRE(lwm2mObj.writeFunc == NULL);
        REQUIRE(lwm2mObj.executeFunc == NULL);
        REQUIRE(lwm2mObj.createFunc != NULL);
        REQUIRE(lwm2mObj.deleteFunc != NULL);

        ObjectInfo constrainedObjInfo = mockInfo;
        constrainedObjInfo.operations = ItemOp(ItemOp::NONE);
        ObjectMock constrainedObj(mockContextObj, constrainedObjInfo);
        lwm2m_object_t constrainedLwm2mObj = constrainedObj.getLwm2mObject();
        REQUIRE(constrainedLwm2mObj.userData == &constrainedObj);
        REQUIRE(constrainedLwm2mObj.readFunc == NULL);
        REQUIRE(constrainedLwm2mObj.discoverFunc == NULL);
        REQUIRE(constrainedLwm2mObj.writeFunc == NULL);
        REQUIRE(constrainedLwm2mObj.executeFunc == NULL);
        REQUIRE(constrainedLwm2mObj.createFunc == NULL);
        REQUIRE(constrainedLwm2mObj.deleteFunc == NULL);
    }

    SECTION("Object: Destructor")
    {
        REQUIRE(obj.instanceCnt() == 0);
    }
}

TEST_CASE("Object: instance clb/remove", "[clb][remove][instanceCnt][isExist][restore]")
{
    ObjOpObserverTest actObserver;
    lwm2m_object_t lwm2mObj = obj.getLwm2mObject();

    obj.opSubscribe(&actObserver);
    obj.getClient();
    obj.instances();
    obj.instance(1);

    REQUIRE(obj.getFirstAvailableInstanceID() == 0);
    REQUIRE(obj.serverRead_clb(&mockContextObj, &mockServer, 0, 0, 0, &lwm2mObj) == COAP_404_NOT_FOUND);
    REQUIRE(obj.serverWrite_clb(&mockContextObj, &mockServer, 0, 0, 0, &lwm2mObj, LWM2M_WRITE_PARTIAL_UPDATE) == COAP_404_NOT_FOUND);
    REQUIRE(obj.serverExecute_clb(&mockContextObj, &mockServer, 0, 0, 0, 0, &lwm2mObj) == COAP_404_NOT_FOUND);
    REQUIRE(obj.serverDiscover_clb(&mockContextObj, &mockServer, 0, 0, 0, &lwm2mObj) == COAP_404_NOT_FOUND);
    REQUIRE(obj.serverCreate_clb(&mockContextObj, &mockServer, 0, 0, 0, &lwm2mObj) == COAP_500_INTERNAL_SERVER_ERROR);
    REQUIRE(obj.serverDelete_clb(&mockContextObj, &mockServer, 0, &lwm2mObj) == COAP_404_NOT_FOUND);

    obj.createInstance(2);

    // obj.getRegistry();

    SECTION("createInstance")
    {
        REQUIRE(obj.instanceCnt() == 0);
        REQUIRE_FALSE(obj.isExist(0));
        REQUIRE(obj.instanceCnt() == 0);

        ObjectInfo constrainedObjInfo = mockInfo;
        constrainedObjInfo.isSingle = IS_SINGLE::SINGLE;
        ObjectMock constrainedObj(mockContextObj, constrainedObjInfo);
    }

    SECTION("remove")
    {
        REQUIRE_FALSE(obj.remove(5));
        REQUIRE(obj.instanceCnt() == 0);
        REQUIRE_FALSE(obj.remove(ID_T_MAX_VAL));
        REQUIRE_FALSE(obj.isExist(0));
    }
}