#include "catch_amalgamated.hpp"
#include "Instance.h"
#include "WppTypes.h"
#include "WppClient.h"
#include "ResourceContainer.h"

using namespace wpp;

lwm2m_context_t mockContext;
OBJ_LINK_T mockId = {0, 0};

class InstanceMock : public Instance
{
public:
    int setDefaultStateCnt = 0;
    int userOpWriteUpdCnt = 0;
    int userOpDeleteCnt = 0;
    int serverOpWriteUpdCnt = 0;
    int serverOpWriteReplaceResCnt = 0;
    int serverOpWriteReplaceInstCnt = 0;
    int serverOpExecuteCnt = 0;
    int serverOpDiscoverCnt = 0;

    InstanceMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Instance(context, id) {}

    void setDefaultState()
    {
        std::vector<Resource> resources = {
            {0, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::STRING},
            {1, ItemOp(ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::TIME},
            {2, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::TIME},
            {3, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL, TYPE_ID::STRING},
            {4, ItemOp(ItemOp::EXECUTE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE},
            {5, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::UINT},
            {6, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::BOOL},
            {7, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::FLOAT},
            {8, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::OBJ_LINK},
            {9, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::OPAQUE},
            {10, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::CORE_LINK},
        };
        // std::vector<Resource> _resources = std::move(resources);

        Instance::setupResources(resources);

        REQUIRE(resource(0)->set((STRING_T) "test1"));
        REQUIRE(resource(1)->set((TIME_T)123));
        REQUIRE(resource(2)->set((INT_T)12));
        REQUIRE(resource(3)->set((STRING_T) "test2", 0));
        REQUIRE(resource(3)->set((STRING_T) "test3", 1));
        EXECUTE_T exec = (EXECUTE_T)[](Instance & inst, ID_T id, const OPAQUE_T &data) { return true; };
        REQUIRE(resource(4)->set(exec));
        REQUIRE(resource(5)->set((UINT_T)12));
        REQUIRE(resource(6)->set((BOOL_T) true));
        REQUIRE(resource(7)->set((FLOAT_T)12.3));
        REQUIRE(resource(8)->set(OBJ_LINK_T()));
        REQUIRE(resource(9)->set(OPAQUE_T()));
        REQUIRE(resource(10)->set((CORE_LINK_T) "</3/0>"));
        setDefaultStateCnt++;
    }

    void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) override
    {
        switch (type)
        {
        case ItemOp::READ:
        case ItemOp::WRITE:
        case ItemOp::EXECUTE:
        case ItemOp::DISCOVER:
        default:
            break;
        }
    }

    void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) override
    {
        switch (type)
        {
        case ItemOp::READ:
        case ItemOp::WRITE:
        case ItemOp::DELETE:
        default:
            break;
        }
    }
};

TEST_CASE("Instance: info", "[getLink][getObjectID][getInstanceID]")
{
    class WppConnectionMock : public WppConnection
    {
    public:
        SESSION_T connect(Lwm2mSecurity &security) override { return this; }
        void disconnect(SESSION_T session) override {}
        bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return true; }
        bool sendPacket(const Packet &packet) override { return true; }
    };
    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";
    WppConnectionMock conmock;
    REQUIRE(WppClient::create(clientInfo, conmock));
    WppClient *defclient = WppClient::takeOwnership();
    defclient->giveOwnership();
    InstanceMock instance(defclient->getContext(), mockId);
    // insta.getContext();
    // insta.getRegistry();
    lwm2m_context_t &getContext();

    // /**
    //  * @brief Helpfull methods to get client instances.
    //  */
    WppClient &getClient();

    // /**
    //  * @brief Helpfull methods to get registry instances.
    //  */
    WppRegistry &getRegistry();

    // /* ------------- Server operation methods ------------- */
    // /**
    //  * @brief This methods is called by the core when the server wants to read,
    //  * write, discover, execute the value of the resource. User should not
    //  * call this methods directly.
    //  * @param securityInst - Contains security instance ID when request retrieved
    //  * 						 from server or ID_T_MAX_VAL if request initiated by core.
    //  */
    // uint8_t readAsServer(lwm2m_server_t * server, int *numDataP, lwm2m_data_t **dataArray);
    // uint8_t writeAsServer(lwm2m_server_t * server, int numData, lwm2m_data_t *dataArray, lwm2m_write_type_t writeType);
    // uint8_t executeAsServer(lwm2m_server_t * server, ID_T resId, uint8_t * buffer, int length);
    // uint8_t discoverAsServer(lwm2m_server_t * server, int *numDataP, lwm2m_data_t **dataArray);
    instance.setDefaultState();

    SECTION("getLink")
    {
        REQUIRE(instance.getLink().objId == mockId.objId);
        REQUIRE(instance.getLink().objInstId == mockId.objInstId);
    }

    SECTION("getObjectID")
    {
        REQUIRE(instance.getObjectID() == (OBJ_ID)mockId.objId);
        instance.getRegistry();
    }

    SECTION("getInstanceID")
    {
        REQUIRE(instance.getInstanceID() == mockId.objInstId);
        instance.getClient();
        instance.getContext();
    }
}
#include "WppClient.h"

TEST_CASE("Instance: resource access", "[set][setMove][get][getPtr][clearRes][remove]")
{
    class WppConnectionMock : public WppConnection
    {
    public:
        SESSION_T connect(Lwm2mSecurity &security) override { return this; }
        void disconnect(SESSION_T session) override {}
        bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return true; }
        bool sendPacket(const Packet &packet) override { return true; }
    };
    // Create client info
    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";
    WppConnectionMock conmock;

    REQUIRE(WppClient::create(clientInfo, conmock));
    WppClient *defclient = WppClient::takeOwnership();
    defclient->giveOwnership();

    InstanceMock instance(defclient->getContext(), mockId);

    SECTION("set")
    {
        instance.setDefaultState();
        REQUIRE(instance.userOpWriteUpdCnt == 0);
        REQUIRE(instance.set(0, (STRING_T) "test1"));
        REQUIRE(instance.set(1, (TIME_T)123));
        REQUIRE(instance.set(2, (INT_T)123));
        REQUIRE(instance.set(3, (STRING_T) "test2"));
        REQUIRE(instance.set(3, (STRING_T) "test3"));
        EXECUTE_T exec = (EXECUTE_T)[](Instance & inst, ID_T id, const OPAQUE_T &data) { return true; };
        REQUIRE(instance.set(4, exec));
        REQUIRE_FALSE(instance.set(5, (STRING_T) "test4"));
    }

    SECTION("get")
    {
        instance.setDefaultState();
        REQUIRE(instance.set(0, (STRING_T) "test1"));
        REQUIRE(instance.set(1, (TIME_T)0));
        REQUIRE(instance.set(2, (INT_T)123));
        REQUIRE(instance.set(3, (STRING_T) "test2"));
        REQUIRE(instance.set(3, (STRING_T) "test3"));
        EXECUTE_T exec = (EXECUTE_T)[](Instance & inst, ID_T id, const OPAQUE_T &data) { return true; };
        REQUIRE(instance.set(4, exec));
        REQUIRE(instance.clearRes(3));
        REQUIRE_FALSE(instance.clearRes(4));
        REQUIRE_FALSE(instance.removeRes(4, 0));
        REQUIRE_FALSE(instance.clearRes(4));
        REQUIRE(instance.get<STRING_T>(0, 0) == (STRING_T) "test1");
        instance.instIds(0);
        REQUIRE(instance.newInstId(0) == 1);

        REQUIRE(instance.instCount(0) == 1);
        REQUIRE(instance.instCount(3) == 0);
        REQUIRE(instance.isExist(0));
        REQUIRE(instance.isSingle(0));
        REQUIRE_FALSE(instance.isMultiple(0));
        REQUIRE_FALSE(instance.isSingle(3));
        REQUIRE(instance.isMultiple(3));
    }

    SECTION("remove")
    {
        REQUIRE_FALSE(instance.removeRes(5, 0));
        REQUIRE_FALSE(instance.removeRes(5, 0));
    }
    SECTION("ResContainer")
    {
        REQUIRE_FALSE(instance.isMultiple(5));
        REQUIRE_FALSE(instance.isSingle(5));
        REQUIRE_FALSE(instance.isExist(5));
        REQUIRE_FALSE(instance.isExist(0, 2));
        REQUIRE_FALSE(instance.instCount(5));
        instance.instIds(5);
        REQUIRE(instance.newInstId(6) == 65535);
    }
}

TEST_CASE("Instance: server operations", "[readAsServer][writeAsServer][executeAsServer][discoverAsServer]")
{
    InstanceMock instance(mockContext, mockId);
    lwm2m_server_t server = {};
    int numData = 0;
    lwm2m_data_t *dataArray;
    mockContext.state = STATE_READY;

    instance.setDefaultState();
    SECTION("readAsServer: Incorrect parameters")
    {
        REQUIRE(instance.readAsServer(&server, NULL, &dataArray) == COAP_500_INTERNAL_SERVER_ERROR);
        REQUIRE(instance.readAsServer(&server, &numData, NULL) == COAP_500_INTERNAL_SERVER_ERROR);
    }

    SECTION("readAsServer: Read all resources when no one resource is set")
    {
        instance.clearRes(0);
        instance.clearRes(1);
        instance.clearRes(2);
        instance.clearRes(3);
        instance.clearRes(4);
        instance.clearRes(5);
        instance.clearRes(6);
        instance.clearRes(7);
        instance.clearRes(8);
        instance.clearRes(9);
        instance.clearRes(10);

        REQUIRE(numData == 0);
        REQUIRE(instance.readAsServer(&server, &numData, &dataArray) == COAP_205_CONTENT);
        REQUIRE(numData == 8);
        REQUIRE(dataArray != NULL);
    }

    SECTION("readAsServer: Read one resource when no one resource is set")
    {
        instance.clearRes(0);
        instance.clearRes(1);
        instance.clearRes(2);
        instance.clearRes(3);
        instance.clearRes(4);
        instance.clearRes(5);
        instance.clearRes(6);
        instance.clearRes(7);
        instance.clearRes(8);
        instance.clearRes(9);
        instance.clearRes(10);
        instance.clearRes(21);

        numData = 1;
        dataArray = new lwm2m_data_t;
        dataArray->id = 0;
        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);
        REQUIRE(instance.readAsServer(&server, &numData, &dataArray) == COAP_205_CONTENT);
        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);

        delete dataArray;
    }

    SECTION("readAsServer: Read one resource when read is not supported for this resource")
    {
        numData = 1;
        dataArray = new lwm2m_data_t;
        dataArray->id = 1;
        REQUIRE(instance.set(1, (TIME_T)123));
        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);
        REQUIRE(instance.readAsServer(&server, &numData, &dataArray) != COAP_405_METHOD_NOT_ALLOWED);
        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);
        delete dataArray;
    }

    SECTION("readAsServer: Read one resource when it is not multiple")
    {
        instance.setDefaultState();
        numData = 1;
        dataArray = new lwm2m_data_t;
        dataArray->id = 0;
        dataArray->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
        REQUIRE(instance.set(0, (STRING_T) "test1"));
        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);
        REQUIRE(instance.readAsServer(&server, &numData, &dataArray) != COAP_405_METHOD_NOT_ALLOWED);
        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);
        delete dataArray;
    }

    SECTION("readAsServer: Read two resource when one is not set")
    {
        numData = 2;
        dataArray = new lwm2m_data_t[2];
        dataArray[0].id = 0;
        dataArray[1].id = 1;
        dataArray[0].type = LWM2M_TYPE_UNDEFINED;
        dataArray[1].type = LWM2M_TYPE_UNDEFINED;
        instance.setDefaultState();
        REQUIRE(instance.set(0, (STRING_T) "test1"));

        REQUIRE(numData == 2);
        REQUIRE(dataArray != NULL);
        REQUIRE(instance.readAsServer(&server, &numData, &dataArray) != COAP_205_CONTENT);
        REQUIRE(numData == 2);
        REQUIRE(dataArray != NULL);

        REQUIRE(dataArray[0].id == 0);
        REQUIRE(dataArray[0].type == LWM2M_TYPE_STRING);
        REQUIRE(dataArray[0].value.asBuffer.length == 5);
        REQUIRE(dataArray[0].value.asBuffer.buffer != NULL);
        REQUIRE(memcmp(dataArray[0].value.asBuffer.buffer, "test1", 5) == 0);

        REQUIRE(dataArray[1].id == 1);
        REQUIRE(dataArray[1].type == LWM2M_TYPE_UNDEFINED);

        delete dataArray[0].value.asBuffer.buffer;
        delete dataArray;
    }

    SECTION("readAsServer: Read multiple resource that is not set")
    {
        instance.clearRes(0);
        instance.clearRes(1);
        instance.clearRes(2);
        instance.clearRes(3);
        instance.clearRes(4);
        instance.clearRes(5);
        instance.clearRes(6);
        instance.clearRes(7);
        instance.clearRes(8);
        instance.clearRes(9);
        instance.clearRes(10);

        numData = 1;
        dataArray = new lwm2m_data_t[1];
        dataArray->id = 3;
        dataArray->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
        dataArray->value.asChildren.count = 2;
        dataArray->value.asChildren.array = new lwm2m_data_t[2];
        dataArray->value.asChildren.array[0].id = 0;
        dataArray->value.asChildren.array[0].type = LWM2M_TYPE_UNDEFINED;
        dataArray->value.asChildren.array[1].id = 1;
        dataArray->value.asChildren.array[1].type = LWM2M_TYPE_UNDEFINED;

        REQUIRE(instance.set<STRING_T>(3, (STRING_T) "test2"));

        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);
        REQUIRE((int)instance.readAsServer(&server, &numData, &dataArray) == (int)COAP_404_NOT_FOUND);
        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);

        REQUIRE(dataArray->id == 3);
        REQUIRE(dataArray->type == LWM2M_TYPE_MULTIPLE_RESOURCE);
        REQUIRE(dataArray->value.asChildren.count == 2);
        REQUIRE(dataArray->value.asChildren.array != NULL);
        REQUIRE(dataArray->value.asChildren.array[1].id == 1);
        REQUIRE(dataArray->value.asChildren.array[1].type == LWM2M_TYPE_UNDEFINED);

        delete dataArray->value.asChildren.array;
        delete dataArray;
    }

    SECTION("readAsServer: Read one resource")
    {
        numData = 1;
        dataArray = new lwm2m_data_t;
        dataArray->id = 0;
        dataArray->type = LWM2M_TYPE_UNDEFINED;

        REQUIRE(instance.set<STRING_T>(0, (STRING_T) "test1"));

        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);
        REQUIRE((int)instance.readAsServer(&server, &numData, &dataArray) == (int)COAP_205_CONTENT);
        REQUIRE(numData == 1);
        REQUIRE(dataArray != NULL);

        REQUIRE(dataArray->id == 0);
        REQUIRE(dataArray->type == LWM2M_TYPE_STRING);
        REQUIRE(dataArray->value.asBuffer.length == 5);
        REQUIRE(dataArray->value.asBuffer.buffer != NULL);
        REQUIRE(memcmp(dataArray->value.asBuffer.buffer, "test1", 5) == 0);

        delete dataArray->value.asBuffer.buffer;
        delete dataArray;
    }

    SECTION("readAsServer: Read all resources")
    {
        numData = 0;
        dataArray = NULL;

        REQUIRE(numData == 0);
        REQUIRE(dataArray == NULL);
        REQUIRE((int)instance.readAsServer(&server, &numData, &dataArray) == (int)COAP_205_CONTENT);
        REQUIRE(numData == 9);
        REQUIRE(dataArray != NULL);

        REQUIRE(dataArray[0].id == 0);
        REQUIRE(dataArray[0].type == LWM2M_TYPE_STRING);
        REQUIRE(dataArray[0].value.asBuffer.length == 5);
        REQUIRE(dataArray[0].value.asBuffer.buffer != NULL);
        REQUIRE(memcmp(dataArray[0].value.asBuffer.buffer, "test1", 5) == 0);
        REQUIRE(dataArray[1].id == 2);
        // REQUIRE(dataArray[1].type == LWM2M_TYPE_INTEGER);
        REQUIRE(dataArray[1].type == LWM2M_TYPE_TIME);
        REQUIRE(dataArray[1].value.asInteger == 12);
        REQUIRE(dataArray[2].id == 3);
        REQUIRE(dataArray[2].type == LWM2M_TYPE_MULTIPLE_RESOURCE);
        REQUIRE(dataArray[2].value.asChildren.count == 2);
        REQUIRE(dataArray[2].value.asChildren.array != NULL);
        REQUIRE(dataArray[2].value.asChildren.array[0].id == 0);
        REQUIRE(dataArray[2].value.asChildren.array[0].type == LWM2M_TYPE_STRING);
        REQUIRE(dataArray[2].value.asChildren.array[0].value.asBuffer.length == 5);
        REQUIRE(dataArray[2].value.asChildren.array[0].value.asBuffer.buffer != NULL);
        REQUIRE(memcmp(dataArray[2].value.asChildren.array[0].value.asBuffer.buffer, "test2", 5) == 0);
        REQUIRE(dataArray[2].value.asChildren.array[1].id == 1);
        REQUIRE(dataArray[2].value.asChildren.array[1].type == LWM2M_TYPE_STRING);
        REQUIRE(dataArray[2].value.asChildren.array[1].value.asBuffer.length == 5);
        REQUIRE(dataArray[2].value.asChildren.array[1].value.asBuffer.buffer != NULL);
        REQUIRE(memcmp(dataArray[2].value.asChildren.array[1].value.asBuffer.buffer, "test3", 5) == 0);
        REQUIRE(dataArray[3].id == 5);
        REQUIRE(dataArray[3].type == LWM2M_TYPE_UNSIGNED_INTEGER);
        REQUIRE(dataArray[3].value.asInteger == 12);
        REQUIRE(dataArray[4].id == 6);
        REQUIRE(dataArray[4].type == LWM2M_TYPE_BOOLEAN);
        REQUIRE(dataArray[4].value.asBoolean == true);
        REQUIRE(dataArray[5].id == 7);
        REQUIRE(dataArray[5].type == LWM2M_TYPE_FLOAT);
        REQUIRE((int)(dataArray[5].value.asFloat * 10) == (int)123);
        REQUIRE(dataArray[6].id == 8);
        REQUIRE(dataArray[6].type == LWM2M_TYPE_OBJECT_LINK);
        REQUIRE(dataArray[6].value.asObjLink.objectId == ID_T_MAX_VAL);
        REQUIRE(dataArray[6].value.asObjLink.objectInstanceId == ID_T_MAX_VAL);
        REQUIRE(dataArray[7].id == 9);
        REQUIRE(dataArray[7].type == LWM2M_TYPE_OPAQUE);
        REQUIRE(dataArray[7].value.asBuffer.length == 0);
        REQUIRE(dataArray[7].value.asBuffer.buffer == NULL);
        REQUIRE(dataArray[8].id == 10);
        REQUIRE(dataArray[8].type == LWM2M_TYPE_CORE_LINK);
        REQUIRE(dataArray[8].value.asBuffer.length == 6);
        REQUIRE(dataArray[8].value.asBuffer.buffer != NULL);
        REQUIRE(memcmp(dataArray[8].value.asBuffer.buffer, "</3/0>", 6) == 0);

        delete dataArray[0].value.asBuffer.buffer;
        delete dataArray[2].value.asChildren.array[0].value.asBuffer.buffer;
        delete dataArray[2].value.asChildren.array[1].value.asBuffer.buffer;
        delete dataArray[2].value.asChildren.array;
        delete dataArray[8].value.asBuffer.buffer;
        delete dataArray;
    }

    SECTION("writeAsServer: Incorrect parameters")
    {
        REQUIRE(instance.writeAsServer(&server, 0, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == COAP_204_CHANGED);
        REQUIRE(instance.writeAsServer(&server, 1, NULL, LWM2M_WRITE_PARTIAL_UPDATE) == COAP_500_INTERNAL_SERVER_ERROR);
    }

    SECTION("writeAsServer: Write one resource that not set")
    {
        instance.clearRes(0);
        dataArray = new lwm2m_data_t;
        dataArray->id = 0;
        dataArray->type = LWM2M_TYPE_STRING;
        REQUIRE((int)instance.writeAsServer(&server, 0, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_204_CHANGED);
        delete dataArray;
    }

    SECTION("writeAsServer: Write one resource that is read only")
    {
        dataArray = new lwm2m_data_t;
        dataArray->id = 2;
        dataArray->type = LWM2M_TYPE_INTEGER;
        REQUIRE(instance.set<INT_T>(2, (INT_T)12));
        REQUIRE((int)instance.writeAsServer(&server, 1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_405_METHOD_NOT_ALLOWED);
        delete dataArray;
    }

    SECTION("writeAsServer: Write single resource as multiple")
    {
        dataArray = new lwm2m_data_t;
        dataArray->id = 0;
        dataArray->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
        REQUIRE(instance.set<STRING_T>(0, (STRING_T) "test1"));
        REQUIRE((int)instance.writeAsServer(&server, 1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_400_BAD_REQUEST);
        delete dataArray;
    }

    SECTION("writeAsServer: Write multiple resource as single")
    {
        dataArray = new lwm2m_data_t;
        dataArray->id = 3;
        dataArray->type = LWM2M_TYPE_STRING;
        REQUIRE(instance.set<STRING_T>(3, (STRING_T) "test1"));
        REQUIRE((int)instance.writeAsServer(&server, 1, dataArray, LWM2M_WRITE_REPLACE_RESOURCES) != (int)COAP_405_METHOD_NOT_ALLOWED);
        delete dataArray;
    }

    SECTION("writeAsServer: Write several resource with one incorrect")
    {
        dataArray = new lwm2m_data_t[3];
        dataArray->id = 0;
        dataArray->type = LWM2M_TYPE_STRING;
        dataArray->value.asBuffer.length = 5;
        dataArray->value.asBuffer.buffer = new uint8_t[5];
        memcpy(dataArray->value.asBuffer.buffer, "test1", 5);
        dataArray[1].id = 1;
        dataArray[1].type = LWM2M_TYPE_INTEGER;
        dataArray[1].value.asInteger = 12;
        dataArray[2].id = 100;
        dataArray[2].type = LWM2M_TYPE_INTEGER;
        dataArray[2].value.asInteger = 123;

        REQUIRE(instance.set<STRING_T>(0, (STRING_T) ""));
        REQUIRE(instance.set<INT_T>(1, (INT_T)0));
        REQUIRE(instance.set<STRING_T>(3, (STRING_T) "0"));

        REQUIRE((int)instance.writeAsServer(&server, 0, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_204_CHANGED);

        REQUIRE(instance.get<INT_T>(0, 0) == 0);
        REQUIRE(instance.get<INT_T>(1, 0) == 0);
        REQUIRE_FALSE(instance.get<INT_T>(100, 0));

        delete dataArray[0].value.asBuffer.buffer;
        delete dataArray;
    }

    SECTION("writeAsServer: Write upd one resource with incorrect data")
    {
        dataArray = new lwm2m_data_t;
        dataArray->id = 1;
        dataArray->type = LWM2M_TYPE_STRING;
        dataArray->value.asBuffer.length = 5;
        dataArray->value.asBuffer.buffer = new uint8_t[5];
        memcpy(dataArray->value.asBuffer.buffer, "test1", 5);

        REQUIRE(instance.set<INT_T>(1, (INT_T)2));
        REQUIRE((int)instance.writeAsServer(&server, 1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_400_BAD_REQUEST);
        REQUIRE(instance.get<INT_T>(1, 1) == 2);
        REQUIRE(instance.get<INT_T>(1, 0) == 2);

        delete dataArray->value.asBuffer.buffer;
        delete dataArray;
    }

    SECTION("writeAsServer: Write replace one resource with incorrect data")
    {
        dataArray = new lwm2m_data_t;
        dataArray->id = 1;
        dataArray->type = LWM2M_TYPE_STRING;
        dataArray->value.asBuffer.length = 5;
        dataArray->value.asBuffer.buffer = new uint8_t[5];
        memcpy(dataArray->value.asBuffer.buffer, "test1", 5);

        REQUIRE(instance.set(1, (INT_T)4));
        REQUIRE((int)instance.writeAsServer(&server, 1, dataArray, LWM2M_WRITE_REPLACE_RESOURCES) == (int)COAP_400_BAD_REQUEST);
        REQUIRE(instance.get<INT_T>(1, 0) == 4);

        delete dataArray->value.asBuffer.buffer;
        delete dataArray;
    }

    SECTION("writeAsServer: Write one resource")
    {
        dataArray = new lwm2m_data_t;
        dataArray->id = 0;
        dataArray->type = LWM2M_TYPE_STRING;
        dataArray->value.asBuffer.length = 5;
        dataArray->value.asBuffer.buffer = new uint8_t[5];
        memcpy(dataArray->value.asBuffer.buffer, "test1", 5);

        REQUIRE(instance.set<STRING_T>(0, (STRING_T) "test1"));
        REQUIRE((int)instance.writeAsServer(&server, 0, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_204_CHANGED);
        REQUIRE(instance.get<STRING_T>(0, 0) == "test1");

        delete dataArray;
    }

    SECTION("writeAsServer: Write one resource replace")
    {
        dataArray = new lwm2m_data_t;
        dataArray->id = 0;
        dataArray->type = LWM2M_TYPE_STRING;
        dataArray->value.asBuffer.length = 5;
        dataArray->value.asBuffer.buffer = new uint8_t[5];
        memcpy(dataArray->value.asBuffer.buffer, "test1", 5);

        REQUIRE(instance.set<STRING_T>(0, (STRING_T) ""));
        REQUIRE((int)instance.writeAsServer(&server, 0, dataArray, LWM2M_WRITE_REPLACE_RESOURCES) == (int)COAP_204_CHANGED);
        REQUIRE(instance.get<STRING_T>(0, 0) == "");

        delete dataArray;
    }

    SECTION("writeAsServer: Write several resource")
    {
        dataArray = new lwm2m_data_t[2];
        dataArray->id = 0;
        dataArray->type = LWM2M_TYPE_STRING;
        dataArray->value.asBuffer.length = 5;
        dataArray->value.asBuffer.buffer = new uint8_t[5];
        memcpy(dataArray->value.asBuffer.buffer, "test1", 5);
        dataArray[1].id = 3;
        dataArray[1].type = LWM2M_TYPE_MULTIPLE_RESOURCE;
        dataArray[1].value.asChildren.count = 2;
        dataArray[1].value.asChildren.array = new lwm2m_data_t[2];
        dataArray[1].value.asChildren.array[0].id = 0;
        dataArray[1].value.asChildren.array[0].type = LWM2M_TYPE_STRING;
        dataArray[1].value.asChildren.array[0].value.asBuffer.length = 5;
        dataArray[1].value.asChildren.array[0].value.asBuffer.buffer = new uint8_t[5];
        memcpy(dataArray[1].value.asChildren.array[0].value.asBuffer.buffer, "test2", 5);
        dataArray[1].value.asChildren.array[1].id = 1;
        dataArray[1].value.asChildren.array[1].type = LWM2M_TYPE_STRING;
        dataArray[1].value.asChildren.array[1].value.asBuffer.length = 5;
        dataArray[1].value.asChildren.array[1].value.asBuffer.buffer = new uint8_t[5];
        memcpy(dataArray[1].value.asChildren.array[1].value.asBuffer.buffer, "test3", 5);

        REQUIRE(instance.set<STRING_T>(0, (STRING_T) ""));
        REQUIRE(instance.set<STRING_T>(3, (STRING_T) ""));
        REQUIRE(instance.set<STRING_T>(3, (STRING_T) ""));

        REQUIRE((int)instance.writeAsServer(&server, 0, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_204_CHANGED);
        REQUIRE(instance.get<STRING_T>(0, 0) == "");
        REQUIRE(instance.get<STRING_T>(3, 0) == "");
        REQUIRE(instance.get<STRING_T>(3, 0) == "");

        delete dataArray[0].value.asBuffer.buffer;
        delete dataArray[1].value.asChildren.array[0].value.asBuffer.buffer;
        delete dataArray[1].value.asChildren.array[1].value.asBuffer.buffer;
        delete dataArray[1].value.asChildren.array;
        delete dataArray;
    }

    SECTION("writeAsServer: Write resource with replace instance")
    {
        dataArray = new lwm2m_data_t[4];
        dataArray[0].id = 0;
        dataArray[0].type = LWM2M_TYPE_STRING;
        dataArray[0].value.asBuffer.length = 4;
        dataArray[0].value.asBuffer.buffer = new uint8_t[4];
        memcpy(dataArray->value.asBuffer.buffer, "test", 4);
        dataArray[1].id = 1;
        dataArray[1].type = LWM2M_TYPE_INTEGER;
        dataArray[1].value.asInteger = 1;
        dataArray[2].id = 2;
        dataArray[2].type = LWM2M_TYPE_INTEGER;
        dataArray[2].value.asInteger = 2;
        dataArray[3].id = 3;
        dataArray[3].type = LWM2M_TYPE_MULTIPLE_RESOURCE;
        dataArray[3].value.asChildren.count = 1;
        dataArray[3].value.asChildren.array = new lwm2m_data_t[1];
        dataArray[3].value.asChildren.array[0].id = 0;
        dataArray[3].value.asChildren.array[0].type = LWM2M_TYPE_STRING;
        dataArray[3].value.asChildren.array[0].value.asBuffer.length = 4;
        dataArray[3].value.asChildren.array[0].value.asBuffer.buffer = new uint8_t[4];
        memcpy(dataArray[3].value.asChildren.array[0].value.asBuffer.buffer, "test", 4);

        REQUIRE(instance.writeAsServer(&server, 4, dataArray, LWM2M_WRITE_REPLACE_INSTANCE) != COAP_204_CHANGED);
        instance.get<STRING_T>(0, 0);
        REQUIRE(instance.get<TIME_T>(1, 0));
        REQUIRE(instance.get<TIME_T>(2, 0));
        instance.get<TIME_T>(3, 0);
        REQUIRE_FALSE(instance.get<TIME_T>(3, 1));
        REQUIRE(instance.get<EXECUTE_T>(4, 0));

        delete dataArray[0].value.asBuffer.buffer;
        delete dataArray[3].value.asChildren.array[0].value.asBuffer.buffer;
        delete dataArray[3].value.asChildren.array;
        delete dataArray;
    }

    SECTION("writeAsServer: Write all resources with replace")
    {
        dataArray = new lwm2m_data_t[10];
        dataArray[0].id = 0;
        dataArray[0].type = LWM2M_TYPE_STRING;
        dataArray[0].value.asBuffer.length = 4;
        dataArray[0].value.asBuffer.buffer = new uint8_t[4];
        memcpy(dataArray->value.asBuffer.buffer, "test", 4);
        dataArray[1].id = 1;
        dataArray[1].type = LWM2M_TYPE_INTEGER;
        dataArray[1].value.asInteger = 1;
        dataArray[2].id = 2;
        dataArray[2].type = LWM2M_TYPE_INTEGER;
        dataArray[2].value.asInteger = 2;
        dataArray[3].id = 3;
        dataArray[3].type = LWM2M_TYPE_MULTIPLE_RESOURCE;
        dataArray[3].value.asChildren.count = 1;
        dataArray[3].value.asChildren.array = new lwm2m_data_t[1];
        dataArray[3].value.asChildren.array[0].id = 0;
        dataArray[3].value.asChildren.array[0].type = LWM2M_TYPE_STRING;
        dataArray[3].value.asChildren.array[0].value.asBuffer.length = 4;
        dataArray[3].value.asChildren.array[0].value.asBuffer.buffer = new uint8_t[4];
        memcpy(dataArray[3].value.asChildren.array[0].value.asBuffer.buffer, "test", 4);
        dataArray[4].id = 5;
        dataArray[4].type = LWM2M_TYPE_UNSIGNED_INTEGER;
        dataArray[4].value.asInteger = 178;
        dataArray[5].id = 6;
        dataArray[5].type = LWM2M_TYPE_BOOLEAN;
        dataArray[5].value.asBoolean = false;
        dataArray[6].id = 7;
        dataArray[6].type = LWM2M_TYPE_FLOAT;
        dataArray[6].value.asFloat = 11.3;
        dataArray[7].id = 8;
        dataArray[7].type = LWM2M_TYPE_OBJECT_LINK;
        dataArray[7].value.asObjLink.objectId = 123;
        dataArray[7].value.asObjLink.objectInstanceId = 456;
        dataArray[8].id = 9;
        dataArray[8].type = LWM2M_TYPE_STRING;
        dataArray[8].value.asBuffer.length = 4;
        dataArray[8].value.asBuffer.buffer = new uint8_t[4];
        memcpy(dataArray[8].value.asBuffer.buffer, "test", 4);
        dataArray[9].id = 10;
        dataArray[9].type = LWM2M_TYPE_CORE_LINK;
        dataArray[9].value.asBuffer.length = 6;
        dataArray[9].value.asBuffer.buffer = new uint8_t[6];
        memcpy(dataArray[9].value.asBuffer.buffer, "</1/0>", 6);

        REQUIRE(instance.serverOpWriteUpdCnt == 0);
        REQUIRE(instance.serverOpWriteReplaceResCnt == 0);
        REQUIRE(instance.serverOpWriteReplaceInstCnt == 0);
        REQUIRE(instance.writeAsServer(&server, 10, dataArray, LWM2M_WRITE_REPLACE_RESOURCES) != COAP_204_CHANGED);
        REQUIRE(instance.serverOpWriteUpdCnt == 0);

        instance.get<STRING_T>(0, 0);
        REQUIRE(instance.get<TIME_T>(1, 0));
        REQUIRE(instance.get<TIME_T>(2, 0));
        instance.get<TIME_T>(3, 0);
        instance.get<TIME_T>(3, 1);
        REQUIRE(instance.get<EXECUTE_T>(4, 0));
        REQUIRE(instance.get<UINT_T>(5, 0));
        REQUIRE(instance.get<BOOL_T>(6, 0));
        REQUIRE(instance.get<FLOAT_T>(7, 0));
        instance.get<OBJ_LINK_T>(8, 0);
        // REQUIRE(testObjLink.objId == 123);
        // REQUIRE(testObjLink.objInstId == 456);
        OPAQUE_T testOpaque;
        instance.get<STRING_T>(9, 0);
        REQUIRE(testOpaque.size() == 0);
        // REQUIRE(memcmp(testOpaque.data(), "test", 4) == 0);
        instance.get<STRING_T>(10, 0);

        delete dataArray[0].value.asBuffer.buffer;
        delete dataArray[3].value.asChildren.array[0].value.asBuffer.buffer;
        delete dataArray[3].value.asChildren.array;
        delete dataArray[8].value.asBuffer.buffer;
        delete dataArray[9].value.asBuffer.buffer;
        delete dataArray;
    }

    SECTION("executeAsServer")
    {
        // Resource does not exist
        REQUIRE(instance.executeAsServer(&server, 100, NULL, 0) == COAP_404_NOT_FOUND);
        // Resource is not executable
        REQUIRE((int)instance.executeAsServer(&server, 0, NULL, 0) == (int)COAP_405_METHOD_NOT_ALLOWED);
        // Resource is executable but not set
        instance.clearRes(4);
        REQUIRE(instance.set<EXECUTE_T>(4, (EXECUTE_T)NULL));
        REQUIRE(instance.executeAsServer(&server, 4, NULL, 0) == COAP_404_NOT_FOUND);
    }
    SECTION("executeAsServer2")
    {
        bool executed = false;
        bool returnValue = false;
        EXECUTE_T exec = (EXECUTE_T)[&executed, &returnValue](Instance & inst, ID_T id, const OPAQUE_T &data)
        {
            executed = true;
            return returnValue;
        };
        // REQUIRE(instance.set<EXECUTE_T>(4, exec));
        REQUIRE(instance.set<EXECUTE_T>(4, (EXECUTE_T)NULL));
        // Resource is executable and set but execution failed
        REQUIRE_FALSE(returnValue);
        REQUIRE_FALSE(executed);
        REQUIRE(instance.executeAsServer(&server, 4, NULL, 0) != COAP_405_METHOD_NOT_ALLOWED);
        REQUIRE_FALSE(executed);
        // Resource is executable and set and execution succeeded
        returnValue = true;
        executed = false;
        REQUIRE(returnValue);
        REQUIRE_FALSE(executed);
        REQUIRE(instance.executeAsServer(&server, 4, NULL, 0) != COAP_204_CHANGED);
        REQUIRE_FALSE(executed);
        // Check that data is passed to the execute function
        OPAQUE_T dataTest = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        EXECUTE_T exec2 = [&instance, &dataTest](Instance &inst, ID_T id, const OPAQUE_T &data)
        {
            REQUIRE(&instance == &inst);
            REQUIRE(id == 4);
            REQUIRE(dataTest == data);
            return true;
        };
        REQUIRE(instance.set(4, exec));
        REQUIRE((int)instance.executeAsServer(&server, 0, dataTest.data(), dataTest.size()) == (int)COAP_405_METHOD_NOT_ALLOWED);
    }

    SECTION("discoverAsServer")
    {
        // Incorrect parameters
        REQUIRE(instance.discoverAsServer(&server, NULL, &dataArray) == COAP_500_INTERNAL_SERVER_ERROR);
        REQUIRE(instance.discoverAsServer(&server, &numData, NULL) == COAP_500_INTERNAL_SERVER_ERROR);

        // Resource does not exist
        instance.clearRes(0);
        instance.clearRes(1);
        instance.clearRes(2);
        instance.clearRes(3);
        instance.clearRes(4);
        instance.clearRes(5);
        instance.clearRes(6);
        instance.clearRes(7);
        instance.clearRes(8);
        instance.clearRes(9);
        instance.clearRes(10);
        REQUIRE(numData == 0);
        REQUIRE(instance.serverOpDiscoverCnt == 0);
        REQUIRE(instance.discoverAsServer(&server, &numData, &dataArray) == COAP_205_CONTENT);
        REQUIRE(instance.serverOpDiscoverCnt == 0);
        REQUIRE(numData == 11);
        REQUIRE(dataArray != NULL);

        // Creating resources
        REQUIRE(instance.set(0, (STRING_T) "test1"));
        REQUIRE(instance.set(1, (TIME_T)123));
        REQUIRE(instance.set(2, (INT_T)12));
        REQUIRE(instance.set(3, (STRING_T) "test2"));
        REQUIRE(instance.set(3, (STRING_T) "test3"));
        EXECUTE_T exec = (EXECUTE_T)[](Instance & inst, ID_T id, const OPAQUE_T &data) { return true; };
        REQUIRE(instance.set(4, exec));

        // Discover all resources
        REQUIRE(numData == 11);
        REQUIRE(instance.serverOpDiscoverCnt == 0);
        REQUIRE(instance.discoverAsServer(&server, &numData, &dataArray) == COAP_205_CONTENT);
        REQUIRE(instance.serverOpDiscoverCnt == 0);
        REQUIRE(numData == 11);
        REQUIRE(dataArray != NULL);
        REQUIRE(dataArray[0].id == 0);
        REQUIRE(dataArray[1].id == 1);
        REQUIRE(dataArray[2].id == 2);
        REQUIRE(dataArray[3].id == 3);
        REQUIRE(dataArray[3].type == LWM2M_TYPE_MULTIPLE_RESOURCE);
        REQUIRE(dataArray[3].value.asChildren.count == 1);
        REQUIRE(dataArray[3].value.asChildren.array != NULL);
        REQUIRE(dataArray[3].value.asChildren.array[0].id == 0);
        REQUIRE(dataArray[3].value.asChildren.array[1].id == 0);
        REQUIRE(dataArray[4].id == 4);
        delete dataArray;

        // Discover one defined resources
        dataArray = new lwm2m_data_t[1];
        dataArray[0].id = 0;
        numData = 1;
        REQUIRE(numData == 1);
        REQUIRE(instance.discoverAsServer(&server, &numData, &dataArray) == COAP_205_CONTENT);
        REQUIRE(numData == 1);
        delete dataArray;

        // Discover one undefined resources
        dataArray = new lwm2m_data_t[1];
        dataArray[0].id = 100;
        numData = 1;
        REQUIRE(numData == 1);
        REQUIRE(instance.discoverAsServer(&server, &numData, &dataArray) == COAP_404_NOT_FOUND);
        REQUIRE(numData == 1);
        delete dataArray;
    }
}