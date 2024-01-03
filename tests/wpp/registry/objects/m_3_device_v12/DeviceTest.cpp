#include "catch_amalgamated.hpp"
#include "./../../../../../wpp/registry/objects/m_3_device_v12/Device.h"

using namespace wpp;

TEST_CASE("deviceMock", "[deviceMock]")
{
    SECTION("deviceMock")
    {
        class DeviceMock : public Device
        {
        public:
            DeviceMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Device(context, id) {}

            void setDefaultState()
            {
                Device::setDefaultState();
                REQUIRE(Device::resource(ERROR_CODE_11)->set((INT_T)NO_ERROR));
                REQUIRE_FALSE(Device::resource(ERROR_CODE_11)->set((INT_T(10))));
                REQUIRE_FALSE(Device::resource(ERROR_CODE_11)->set((INT_T(-1))));

                REQUIRE_FALSE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T("")));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_TCP)));
            }

            void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Device::serverOperationNotifier(type, resId); }
            void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Device::userOperationNotifier(type, resId); }
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        // Create an instance of SecurityMock
        DeviceMock deviceMock(mockContext, mockId);

        deviceMock.setDefaultState();

        deviceMock.serverOperationNotifier(ResOp::TYPE::READ, {0, 0});
        deviceMock.userOperationNotifier(ResOp::TYPE::WRITE, {10, 10});
    }
}