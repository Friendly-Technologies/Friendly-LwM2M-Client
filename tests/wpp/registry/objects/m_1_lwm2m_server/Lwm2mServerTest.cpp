#include "catch_amalgamated.hpp"
#include "m_1_lwm2m_server/Lwm2mServer.h"
#include <thread>
#include "WppClient.h"

using namespace wpp;
EXECUTE_T exe;

TEST_CASE("objectLwm2mServer", "[objectLwm2mServer]")
{
    class Lwm2mServerMock : public Lwm2mServer
    {
    public:
        Lwm2mServerMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mServer(context, id) {}
        void setDefaultState()
        {
            REQUIRE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(SINGLE_INSTANCE_ID + 1)));
            REQUIRE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(ID_T_MAX_VAL - 1)));
            REQUIRE_FALSE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(SINGLE_INSTANCE_ID)));
            REQUIRE_FALSE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(ID_T_MAX_VAL)));
            REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T("")));
            REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_TCP)));
            REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_UDP)));
            REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_SMS)));
            REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_NON_IP)));
        }

        void getDISABLE_4()
        {
            exe = Lwm2mServer::resource(DISABLE_4)->get<EXECUTE_T>();
        }

        void getBOOTSTRAP_REQUEST_TRIGGER_9()
        {
            exe = Lwm2mServer::resource(BOOTSTRAP_REQUEST_TRIGGER_9)->get<EXECUTE_T>();
        }

        void getREGISTRATION_UPDATE_TRIGGER_8()
        {
            exe = Lwm2mServer::resource(REGISTRATION_UPDATE_TRIGGER_8)->get<EXECUTE_T>();
        }

        void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { Lwm2mServer::serverOperationNotifier(securityInst, type, resId); }
        void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { Lwm2mServer::userOperationNotifier(type, resId); }
    };

    lwm2m_context_t mockContext;
    OBJ_LINK_T mockId = {1, 1};
    // Create an instance of ServerMock
    Lwm2mServerMock serverMock(mockContext, mockId);

    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";

    WppClient *client;

    SECTION("objectLwm2mServer")
    {

        REQUIRE(serverMock.isExist(4));
        serverMock.setDefaultState();

        serverMock.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
        serverMock.serverOperationNotifier(0, ItemOp::TYPE::READ, {1, 1});
        serverMock.serverOperationNotifier(0, ItemOp::TYPE::WRITE, {1, 1});
        serverMock.serverOperationNotifier(0, ItemOp::TYPE::NONE, {1, 1});
        serverMock.serverOperationNotifier(0, ItemOp::TYPE::EXECUTE, {1, 1});
        serverMock.serverOperationNotifier(0, ItemOp::TYPE::DISCOVER, {1, 1});
        serverMock.serverOperationNotifier(0, ItemOp::TYPE::CREATE, {1, 1});
        serverMock.serverOperationNotifier(0, ItemOp::TYPE::DELETE, {1, 1});

        serverMock.userOperationNotifier(ItemOp::TYPE::WRITE, {1, 1});
        
#ifdef LWM2M_CLIENT_MODE
        mockContext.state = STATE_READY;
        serverMock.userOperationNotifier(ItemOp::TYPE::WRITE, {1, 1});
#endif

        serverMock.userOperationNotifier(ItemOp::TYPE::WRITE, {23, 23});

        serverMock.getDISABLE_4();
        REQUIRE(exe(serverMock, Lwm2mServerMock::DISABLE_4, OPAQUE_T()));
        REQUIRE(exe(serverMock, Lwm2mServerMock::DISABLE_4, OPAQUE_T())); // for else - if exist

        serverMock.getREGISTRATION_UPDATE_TRIGGER_8();
        REQUIRE(exe(serverMock, Lwm2mServerMock::REGISTRATION_UPDATE_TRIGGER_8, OPAQUE_T()));

#if RES_1_9
        REQUIRE(WppTaskQueue::getTaskCnt() == 0);
        serverMock.getBOOTSTRAP_REQUEST_TRIGGER_9();
        REQUIRE(exe(serverMock, Lwm2mServerMock::BOOTSTRAP_REQUEST_TRIGGER_9, OPAQUE_T()));
        REQUIRE(exe(serverMock, Lwm2mServerMock::BOOTSTRAP_REQUEST_TRIGGER_9, OPAQUE_T())); // for else - if exist
        REQUIRE(WppTaskQueue::getTaskCnt() == 1);
        std::this_thread::sleep_for(std::chrono::seconds(WPP_TASK_DEF_DELAY_S + 1));
        WppTaskQueue::handleEachTask(*client);
        REQUIRE(WppTaskQueue::getTaskCnt() == 0);
#endif
    }

    SECTION("instansesServer")
    {
        WppClient *defclient = WppClient::takeOwnership();
        defclient->giveOwnership();

        serverMock.object(*WppClient::takeOwnership());
        defclient->giveOwnership();

        REQUIRE(serverMock.instance(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(serverMock.instance(*WppClient::takeOwnership(), ID_T_MAX_VAL) == NULL);
        defclient->giveOwnership();

        REQUIRE(serverMock.createInst(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();
        REQUIRE(serverMock.createInst(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(serverMock.instance(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();

        REQUIRE(serverMock.removeInst(*WppClient::takeOwnership(), 1));
        defclient->giveOwnership();

        WppTaskQueue::requestToRemoveEachTask();
        WppTaskQueue::handleEachTask(*client);
        REQUIRE(WppTaskQueue::getTaskCnt() == 0);
    }
}
