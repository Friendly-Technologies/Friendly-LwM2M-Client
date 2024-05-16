#include "catch_amalgamated.hpp"
#include "./../../../../../wpp/registry/objects/o_2_lwm2m_access_control/Lwm2mAccessControl.h"
#include "WppClient.h"
#include "WppRegistry.h"
#include "Object.h"

#define AC_OBJ_ID_MIN 1
#define AC_OBJ_ID_MAX (ID_T_MAX_VAL - 1)
#define AC_OBJ_INST_ID_MIN 0
#define AC_OBJ_INST_ID_MAX ID_T_MAX_VAL
#define AC_OWNER_MIN 0
#define AC_OWNER_MAX ID_T_MAX_VAL

using namespace wpp;

TEST_CASE("objectLwm2mAccessControl", "[objectLwm2mAccessControl]")
{
    // Connection mock class for virtual connection | for return TRUE
    class WppConnectionMock : public WppConnection
    {
    public:
        SESSION_T connect(Lwm2mSecurity &security) override { return NULL; }
        void disconnect(SESSION_T session) override {}
        bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return true; }
        bool sendPacket(const Packet &packet) override { return true; }
    };

    // Create a packet for testing
    WppConnection::Packet testPacket;
    testPacket.session = nullptr;
    testPacket.length = 10;
    uint8_t test_buffer[10] = {
        0};
    testPacket.buffer = test_buffer;

    // Create client info
    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";

    WppConnectionMock conmock;

    SECTION("objectLwm2mAccessControl")
    {
        class Lwm2mAccessControlMock : public Lwm2mAccessControl
        {
        public:
            Lwm2mAccessControlMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mAccessControl(context, id) {}

            void setDefaultState()
            {
                // return AC_OBJ_ID_MIN <= value && value <= AC_OBJ_ID_MAX
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MIN)));
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MAX)));
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MIN - 1))); // !!
                REQUIRE_FALSE(Lwm2mAccessControl::resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MAX + 1)));

                // return AC_OBJ_INST_ID_MIN <= value && value <= AC_OBJ_INST_ID_MAX
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MIN)));
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MAX)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MIN - 1)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MAX + 1)));

#if RES_2_2
                REQUIRE(resource(ACL_2)->set(INT_T(0)));
                REQUIRE(resource(ACL_2)->set(INT_T(3)));
                REQUIRE_FALSE(resource(ACL_2)->set(INT_T(200)));
#endif

                // return AC_OWNER_MIN <= value && value <= AC_OWNER_MAX
                REQUIRE(Lwm2mAccessControl::resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MIN)));
                REQUIRE(Lwm2mAccessControl::resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MAX)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MIN - 1)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MAX + 1)));
            }

            Object &object(WppClient &ctx) { return Lwm2mAccessControl::object(ctx); }

            void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { Lwm2mAccessControl::serverOperationNotifier(securityInst, type, resId); }
            void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { Lwm2mAccessControl::userOperationNotifier(type, resId); }

            Lwm2mAccessControl *createLoc(Instance &targetInst, ID_T owner, uint8_t defaultAcl) { return Lwm2mAccessControl::create(targetInst, owner, defaultAcl); }
            void removeLoc(Instance &targetInst) { return Lwm2mAccessControl::remove(targetInst); }
            void removeAcLoc(Instance &targetInst, ID_T serverShortId) { return Lwm2mAccessControl::removeAcl(targetInst, serverShortId); }
        };

        WppClient *defclient = WppClient::takeOwnership();
        lwm2m_context_t mockContext = defclient->getContext();
        OBJ_LINK_T mockId = {0, 1};
        Lwm2mAccessControlMock accesscontrolMock(mockContext, mockId);

        accesscontrolMock.setDefaultState();
        accesscontrolMock.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
        accesscontrolMock.userOperationNotifier(ItemOp::TYPE::WRITE, {10, 10});
        accesscontrolMock.userOperationNotifier(ItemOp::TYPE::DELETE, {10, 10});

        Lwm2mAccessControl::remove(defclient->registry().device());
        Lwm2mAccessControl::removeAcl(defclient->registry().device(), 1);
        REQUIRE(Lwm2mAccessControl::create(defclient->registry().device(), 0) != 0);
        REQUIRE(Lwm2mAccessControl::addAcl(defclient->registry().device(), 1, 11) == true);
        Lwm2mAccessControl::removeAcl(defclient->registry().device(), 1);
        REQUIRE(Lwm2mAccessControl::addAcl(defclient->registry().device(), 2, 15) == true);
        REQUIRE(Lwm2mAccessControl::create(defclient->registry().device(), 0) == NULL);
        Lwm2mAccessControl::remove(defclient->registry().device());
        REQUIRE(Lwm2mAccessControl::addAcl(defclient->registry().device(), 1, 11) == false);
        Lwm2mAccessControl::remove(defclient->registry().device());

        REQUIRE(Lwm2mAccessControl::create(defclient->registry().device(), 0) != 0);
        defclient->registry().device().createInstance(1);

        Lwm2mAccessControl::remove(defclient->registry().device());
        REQUIRE(Lwm2mAccessControl::create(*defclient->registry().device().instance(1), 1, 1) != NULL);
        REQUIRE(Lwm2mAccessControl::create(*defclient->registry().device().instance(1), 1, 1) == NULL);

        Lwm2mAccessControl::removeAcl(*defclient->registry().device().instance(1), 1);
        Lwm2mAccessControl::addAcl(*defclient->registry().device().instance(1), 1, Lwm2mAccessControl::NO_ACCESS);
        Lwm2mAccessControl::removeAcl(*defclient->registry().device().instance(1), 1);
        Lwm2mAccessControl::remove(*defclient->registry().device().instance(1));
        Lwm2mAccessControl::addAcl(*defclient->registry().device().instance(1), 1, Lwm2mAccessControl::ALL_OBJ_RIGHTS);

        Lwm2mAccessControl::remove(defclient->registry().device());
    }

    SECTION("instansesAccessControl")
    {

        class Lwm2mAccessControlMock : public Lwm2mAccessControl
        {
        public:
            Lwm2mAccessControlMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mAccessControl(context, id) {}
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 0};
        Lwm2mAccessControlMock acont(mockContext, mockId);
        WppClient *defclient = WppClient::takeOwnership();
        defclient->giveOwnership();

        acont.object(*WppClient::takeOwnership());
        defclient->giveOwnership();

        REQUIRE(acont.instance(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(acont.instance(*WppClient::takeOwnership(), ID_T_MAX_VAL) == NULL);
        defclient->giveOwnership();

        REQUIRE(acont.createInst(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();
        REQUIRE(acont.createInst(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(acont.instance(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();

        REQUIRE(acont.removeInst(*WppClient::takeOwnership(), 1));
        defclient->giveOwnership();
    }
}