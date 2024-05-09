#include "catch_amalgamated.hpp"
#include "m_3_device/Device.h"
#include <thread>
#include "WppClient.h"

using namespace wpp;
EXECUTE_T exeDev;

TEST_CASE("objectDevice", "[objectDevice]")
{
    class DeviceMock : public Device
    {
    public:
        DeviceMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Device(context, id) {}

        void setDefaultState()
        {
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

        void getMANUFACTURER_0() { exeDev = Device::resource(MANUFACTURER_0)->get<EXECUTE_T>(); }
        void getMODEL_NUMBER_1() { exeDev = Device::resource(MODEL_NUMBER_1)->get<EXECUTE_T>(); }
        void getSERIAL_NUMBER_2() { exeDev = Device::resource(SERIAL_NUMBER_2)->get<EXECUTE_T>(); }
        void getFIRMWARE_VERSION_3() { exeDev = Device::resource(FIRMWARE_VERSION_3)->get<EXECUTE_T>(); }
        void getREBOOT_4() { exeDev = Device::resource(REBOOT_4)->get<EXECUTE_T>(); }
        void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { Device::serverOperationNotifier(securityInst, type, resId); }
        void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { Device::userOperationNotifier(type, resId); }
    };

    lwm2m_context_t mockContext;
    OBJ_LINK_T mockId = {0, 0};

    // Create an instance of DeviceMock
    DeviceMock deviceMock(mockContext, mockId);

    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";

    SECTION("instansesDevice")
    {
        WppClient *defclient = WppClient::takeOwnership();
        defclient->giveOwnership();

        deviceMock.object(*WppClient::takeOwnership());
        defclient->giveOwnership();

        REQUIRE(deviceMock.instance(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(deviceMock.instance(*WppClient::takeOwnership(), ID_T_MAX_VAL) == NULL);
        defclient->giveOwnership();

        REQUIRE(deviceMock.createInst(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();
        REQUIRE(deviceMock.createInst(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(deviceMock.instance(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();

        REQUIRE(deviceMock.removeInst(*WppClient::takeOwnership(), 1));
        defclient->giveOwnership();
    }

    SECTION("objectDevice")
    {
        WppClient *client;

        deviceMock.getMANUFACTURER_0();
        deviceMock.getMODEL_NUMBER_1();
        deviceMock.getSERIAL_NUMBER_2();
        deviceMock.getFIRMWARE_VERSION_3();
        deviceMock.getREBOOT_4();
        REQUIRE(exeDev(deviceMock, DeviceMock::MANUFACTURER_0, OPAQUE_T()));

        REQUIRE(WppTaskQueue::getTaskCnt() == 3);
        std::this_thread::sleep_for(std::chrono::seconds(2));

        deviceMock.setDefaultState();
        REQUIRE(WppTaskQueue::getTaskCnt() == 3);

        deviceMock.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
        deviceMock.userOperationNotifier(ItemOp::TYPE::WRITE, {10, 10});

        WppTaskQueue::handleEachTask(*client);
        REQUIRE(WppTaskQueue::getTaskCnt() == 1);

        WppTaskQueue::requestToRemoveEachTask();
        WppTaskQueue::handleEachTask(*client);
        REQUIRE(WppTaskQueue::getTaskCnt() == 0);
    }
}