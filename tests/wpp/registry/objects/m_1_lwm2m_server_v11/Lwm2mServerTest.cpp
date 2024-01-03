#include "catch_amalgamated.hpp"
#include "./../../../../../wpp/registry/objects/m_1_lwm2m_server_v11/Lwm2mServer.h"

using namespace wpp;

TEST_CASE("serverMock", "[serverMock]")
{
    SECTION("serverMock")
    {
        class Lwm2mServerMock : public Lwm2mServer
        {
        public:
            Lwm2mServerMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mServer(context, id) {}
            void setDefaultState()
            {
                Lwm2mServer::setDefaultState();
                REQUIRE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(15)));
                REQUIRE_FALSE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(0)));
                REQUIRE_FALSE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(65535)));

                REQUIRE_FALSE(Lwm2mServer::resource(BINDING_7)->set(STRING_T("")));
                REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_TCP)));
            }

            void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Lwm2mServer::serverOperationNotifier(type, resId); }
            void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Lwm2mServer::userOperationNotifier(type, resId); }
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        // Create an instance of SecurityMock
        Lwm2mServerMock serverMock(mockContext, mockId);

        serverMock.setDefaultState();

        serverMock.serverOperationNotifier(ResOp::TYPE::READ, {0, 0});
        serverMock.userOperationNotifier(ResOp::TYPE::WRITE, {10, 10});
    }
}
