#include "catch_amalgamated.hpp"
#include "./../../../../../wpp/registry/objects/m_1_lwm2m_server_v11/Lwm2mServer.h"

using namespace wpp;

TEST_CASE("objectLwm2mServer", "[objectLwm2mServer]")
{
    SECTION("objectLwm2mServer")
    {
        class Lwm2mServerMock : public Lwm2mServer
        {
        public:
            Lwm2mServerMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mServer(context, id) {}
            void setDefaultState()
            {
                Lwm2mServer::setDefaultState();

                // return SINGLE_INSTANCE_ID < value && value < ID_T_MAX_VAL
                REQUIRE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(SINGLE_INSTANCE_ID + 1)));
                REQUIRE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(ID_T_MAX_VAL - 1)));
                REQUIRE_FALSE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(SINGLE_INSTANCE_ID)));
                REQUIRE_FALSE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(ID_T_MAX_VAL)));

                // return wppBindingValidate(value)
                REQUIRE_FALSE(Lwm2mServer::resource(BINDING_7)->set(STRING_T("")));
                REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_TCP)));
                REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_UDP)));
                REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_SMS)));
                REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_NON_IP)));
            }

            void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Lwm2mServer::serverOperationNotifier(type, resId); }
            void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Lwm2mServer::userOperationNotifier(type, resId); }
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        // Create an instance of ServerMock
        Lwm2mServerMock serverMock(mockContext, mockId);

        serverMock.setDefaultState();

        serverMock.serverOperationNotifier(ResOp::TYPE::READ, {0, 0});
        serverMock.userOperationNotifier(ResOp::TYPE::WRITE, {10, 10});
    }
}
