#include "catch_amalgamated.hpp"
#include "m_3_device/Device.h"
#include <thread>

namespace wpp {
    class WppClient {
    public:
        WppClient() {}
        ~WppClient() {}
    };
}

using namespace wpp;

TEST_CASE("objectDevice", "[objectDevice]")
{
    SECTION("objectDevice")
    {
        class DeviceMock : public Device
        {
        public:
            DeviceMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Device(context, id) {}

            void setDefaultState()
            {
                // Device::setDefaultState();

                // return NO_ERROR <= value && value < ERR_CODE_MAX
                REQUIRE(Device::resource(ERROR_CODE_11)->set(INT_T(NO_ERROR)));
                REQUIRE(Device::resource(ERROR_CODE_11)->set(INT_T(ERR_CODE_MAX - 1)));
                Device::resource(ERROR_CODE_11)->set((INT_T(NO_ERROR - 1))); // return is undefined, only for good coverage
                REQUIRE_FALSE(Device::resource(ERROR_CODE_11)->set((INT_T(ERR_CODE_MAX))));

                // return wppBindingValidate(value)
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T("")));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_TCP)));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_UDP)));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_SMS)));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_NON_IP)));
            }

            void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { Device::serverOperationNotifier(securityInst, type, resId); }
            void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { Device::userOperationNotifier(type, resId); }
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        REQUIRE(WppTaskQueue::getTaskCnt() == 0);
        // Create an instance of DeviceMock
        DeviceMock deviceMock(mockContext, mockId);
        REQUIRE(WppTaskQueue::getTaskCnt() == 1);
        std::this_thread::sleep_for(std::chrono::seconds(2));

        deviceMock.setDefaultState();
        REQUIRE(WppTaskQueue::getTaskCnt() == 1);

        deviceMock.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
        deviceMock.userOperationNotifier(ItemOp::TYPE::WRITE, {10, 10});

        EXECUTE_T exe;
        exe = deviceMock.resource(4)->get<EXECUTE_T>();
        REQUIRE(exe(deviceMock, 4, OPAQUE_T()));

        WppClient client;
        WppTaskQueue::handleEachTask(client);
        REQUIRE(WppTaskQueue::getTaskCnt() == 1);
        WppTaskQueue::requestToRemoveEachTask();
        WppTaskQueue::handleEachTask(client);
        REQUIRE(WppTaskQueue::getTaskCnt() == 0);
    }
}