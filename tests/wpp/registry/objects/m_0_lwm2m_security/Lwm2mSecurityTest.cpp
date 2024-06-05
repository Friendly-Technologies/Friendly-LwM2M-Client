#include "catch_amalgamated.hpp"
#include "./../../../../../wpp/registry/objects/m_0_lwm2m_security/Lwm2mSecurity.h"
#include "WppConnection.h"
#include "WppTaskQueue.h"
#include "WppClient.h"
using namespace wpp;

static WppTaskQueue::task_t createDummyTask()
{
    return [](WppClient &, void *)
    { return true; };
}

TEST_CASE("objectLwm2mSecurity", "[objectLwm2mSecurity]")
{
    SECTION("objectLwm2mSecurity")
    {
        class Lwm2mSecurityMock : public Lwm2mSecurity
        {
        public:
            Lwm2mSecurityMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mSecurity(context, id) {}
            void setDefaultState()
            {
                REQUIRE(Lwm2mSecurity::resource(LWM2M_SERVER_URI_0)->set(STRING_T("")));
                REQUIRE_FALSE(Lwm2mSecurity::resource(LWM2M_SERVER_URI_0)->set(STRING_T("want-more-and-more-more-more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312")));
                REQUIRE(Lwm2mSecurity::resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_PRE_SHARED_KEY)));
                REQUIRE(Lwm2mSecurity::resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_NONE)));
                REQUIRE_FALSE(Lwm2mSecurity::resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_PRE_SHARED_KEY - 1)));
                REQUIRE_FALSE(Lwm2mSecurity::resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_NONE + 1)));
#if RES_0_10
                REQUIRE(Lwm2mSecurity::resource(SHORT_SERVER_ID_10)->set(INT_T(1)));
                REQUIRE_FALSE(Lwm2mSecurity::resource(SHORT_SERVER_ID_10)->set(INT_T(SINGLE_INSTANCE_ID)));
                REQUIRE_FALSE(Lwm2mSecurity::resource(SHORT_SERVER_ID_10)->set(INT_T(ID_T_MAX_VAL)));
#endif
            }
            void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { Lwm2mSecurity::serverOperationNotifier(securityInst, type, resId); }
            void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { Lwm2mSecurity::userOperationNotifier(type, resId); }
        };

        WppClient::ClientInfo clientInfo;
        clientInfo.endpointName = "exampleEndpoint";
        clientInfo.msisdn = "1234567890";
        clientInfo.altPath = "";

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        // Create an instance of SecurityMock
        Lwm2mSecurityMock securityMock(mockContext, mockId);

        securityMock.setDefaultState();

        WppClient *defclient = WppClient::takeOwnership();
        defclient->giveOwnership();

        securityMock.object(*WppClient::takeOwnership());
        defclient->giveOwnership();

        REQUIRE(securityMock.instance(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(securityMock.instance(*WppClient::takeOwnership(), ID_T_MAX_VAL) == NULL);
        defclient->giveOwnership();

        REQUIRE(securityMock.createInst(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();
        REQUIRE(securityMock.createInst(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(securityMock.instance(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();

        REQUIRE(securityMock.removeInst(*WppClient::takeOwnership(), 1));
        defclient->giveOwnership();

        securityMock.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
        securityMock.userOperationNotifier(ItemOp::TYPE::WRITE, {10, 10});
    }
}
