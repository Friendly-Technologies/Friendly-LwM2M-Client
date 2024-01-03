#include "catch_amalgamated.hpp"
#include "./../../../../../wpp/registry/objects/m_3_device_v12/Device.h"

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
                Device::setDefaultState();

                // return NO_ERROR <= value && value < ERR_CODE_MAX
                REQUIRE(Device::resource(ERROR_CODE_11)->set(INT_T(NO_ERROR)));
                REQUIRE(Device::resource(ERROR_CODE_11)->set(INT_T(ERR_CODE_MAX - 1)));
                REQUIRE_FALSE(Device::resource(ERROR_CODE_11)->set((INT_T(NO_ERROR - 1))));
                REQUIRE_FALSE(Device::resource(ERROR_CODE_11)->set((INT_T(ERR_CODE_MAX))));

                // return wppBindingValidate(value)
                REQUIRE_FALSE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T("")));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_TCP)));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_UDP)));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_SMS)));
                REQUIRE(Device::resource(SUPPORTED_BINDING_AND_MODES_16)->set(STRING_T(WPP_BINDING_NON_IP)));
            }

            void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Device::serverOperationNotifier(type, resId); }
            void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Device::userOperationNotifier(type, resId); }
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        // Create an instance of DeviceMock
        DeviceMock deviceMock(mockContext, mockId);

        deviceMock.setDefaultState();

        deviceMock.serverOperationNotifier(ResOp::TYPE::READ, {0, 0});
        deviceMock.userOperationNotifier(ResOp::TYPE::WRITE, {10, 10});
    }
}