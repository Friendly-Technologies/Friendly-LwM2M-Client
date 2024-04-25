// #include "catch_amalgamated.hpp"
// #include "m_1_lwm2m_server/Lwm2mServer.h"
// #include <thread>

// namespace wpp {
//     class WppClient {
//     public:
//         WppClient() {}
//         ~WppClient() {}
//     };
// }

// using namespace wpp;

// TEST_CASE("objectLwm2mServer", "[objectLwm2mServer]")
// {
//     SECTION("objectLwm2mServer")
//     {
//         class Lwm2mServerMock : public Lwm2mServer
//         {
//         public:
//             Lwm2mServerMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mServer(context, id) {}
//             void setDefaultState()
//             {
//                 // Lwm2mServer::setDefaultState(); TODO

//                 // return SINGLE_INSTANCE_ID < value && value < ID_T_MAX_VAL
//                 REQUIRE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(SINGLE_INSTANCE_ID + 1)));
//                 REQUIRE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(ID_T_MAX_VAL - 1)));
//                 REQUIRE_FALSE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(SINGLE_INSTANCE_ID)));
//                 REQUIRE_FALSE(Lwm2mServer::resource(SHORT_SERVER_ID_0)->set(INT_T(ID_T_MAX_VAL)));

//                 // return wppBindingValidate(value)
//                 REQUIRE_FALSE(Lwm2mServer::resource(BINDING_7)->set(STRING_T("")));
//                 REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_TCP)));
//                 REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_UDP)));
//                 REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_SMS)));
//                 REQUIRE(Lwm2mServer::resource(BINDING_7)->set(STRING_T(WPP_BINDING_NON_IP)));
//             }

//             // void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { Lwm2mServer::serverOperationNotifier(securityInst, type, resId); }
//             void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { Lwm2mServer::userOperationNotifier(type, resId); }
//         };

//         lwm2m_context_t mockContext;
//         OBJ_LINK_T mockId = {0, 1};
//         // Create an instance of ServerMock
//         Lwm2mServerMock serverMock(mockContext, mockId);

//         serverMock.setDefaultState();

//         // serverMock.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
//         serverMock.userOperationNotifier(ItemOp::TYPE::WRITE, {10, 10});
//         EXECUTE_T exe;
//         // serverMock.get(8, exe); TODO
//         REQUIRE(exe(serverMock, 8, OPAQUE_T()));
//         #if RES_1_9
//         WppClient client;
//         REQUIRE(WppTaskQueue::getTaskCnt() == 0);
//         // serverMock.get(9, exe); TODO
//         REQUIRE(exe(serverMock, 9, OPAQUE_T()));
//         REQUIRE(WppTaskQueue::getTaskCnt() == 1);
//         REQUIRE(exe(serverMock, 9, OPAQUE_T()));
//         REQUIRE(WppTaskQueue::getTaskCnt() == 1);
//         std::this_thread::sleep_for(std::chrono::seconds(WPP_TASK_DEF_DELAY_S+1));
//         WppTaskQueue::handleEachTask(client);
//         REQUIRE(WppTaskQueue::getTaskCnt() == 0);
//         #endif
//     }
// }
