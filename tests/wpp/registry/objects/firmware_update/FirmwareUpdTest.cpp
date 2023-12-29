#include "catch_amalgamated.hpp"
#include "o_5_firmware_update_v11/FirmwareUpdate.h"

using namespace wpp;

class FirmwareUpdateMock : public FirmwareUpdate {
public:
    FirmwareUpdateMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : FirmwareUpdate(context, id) {}

    void setDefaultState() {
        FirmwareUpdate::setDefaultState();
    }
    
	void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
        FirmwareUpdate::serverOperationNotifier(type, resId);
    }

	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
        FirmwareUpdate::userOperationNotifier(type, resId);
    }
};

TEST_CASE("FirmwareUpdate: resource initialization", "[resourcesCreate][resourcesInit][setDefaultState]") {
    OPAQUE_T opaque;
    STRING_T str;
    INT_T integer;
    EXECUTE_T execute;
    
    lwm2m_context_t context;
    const OBJ_LINK_T id {5, 0};
    FirmwareUpdateMock fwu(context, id);

    SECTION("resourcesCreate&resourcesInit") {
        REQUIRE(fwu.get(0, opaque));
        REQUIRE(opaque.size() == 0);
        REQUIRE(fwu.get(1, str));
        REQUIRE(str.size() == 0);
        REQUIRE(fwu.get(2, execute));
        REQUIRE(execute != nullptr);
        REQUIRE(fwu.get(3, integer));
        REQUIRE(integer == FirmwareUpdate::S_IDLE);
        REQUIRE(fwu.get(5, integer));
        REQUIRE(integer == FirmwareUpdate::R_INITIAL);
        REQUIRE(fwu.get(6, str));
        REQUIRE(str.size() == 0);
        REQUIRE(fwu.get(7, str));
        REQUIRE(str.size() == 0);
        REQUIRE(fwu.get(8, integer));
        REQUIRE(integer == FirmwareUpdate::COAP);
        REQUIRE(fwu.get(9, integer));
        REQUIRE(integer == FirmwareUpdate::PUSH);
    }

    SECTION("setDefaultState") {
        opaque = {0x01, 0x02, 0x03};
        str = "test";
        integer = 0x1234;
        execute = (EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; };
        REQUIRE(fwu.set(0, opaque));
        REQUIRE(fwu.set(1, STRING_T("")));
        REQUIRE(fwu.set(2, execute));
        REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADING));
        REQUIRE(fwu.set(5, integer));
        REQUIRE(fwu.set(6, str));
        REQUIRE(fwu.set(7, str));
        REQUIRE(fwu.set(8, (INT_T)FirmwareUpdate::HTTP));
        REQUIRE(fwu.set(9, (INT_T)FirmwareUpdate::PUSH));

        fwu.setDefaultState();
        REQUIRE(fwu.get(0, opaque));
        REQUIRE(opaque.size() == 0);
        REQUIRE(fwu.get(1, str));
        REQUIRE(str.size() == 0);
        REQUIRE(fwu.get(2, execute));
        REQUIRE(execute != nullptr);
        REQUIRE(fwu.get(3, integer));
        REQUIRE(integer == FirmwareUpdate::S_IDLE);
        REQUIRE(fwu.get(5, integer));
        REQUIRE(integer == FirmwareUpdate::R_INITIAL);
        REQUIRE(fwu.get(6, str));
        REQUIRE(str.size() == 0);
        REQUIRE(fwu.get(7, str));
        REQUIRE(str.size() == 0);
        REQUIRE(fwu.get(8, integer));
        REQUIRE(integer == FirmwareUpdate::COAP);
        REQUIRE(fwu.get(9, integer));
        REQUIRE(integer == FirmwareUpdate::PUSH);
    }
}

TEST_CASE("FirmwareUpdate", "[serverOperationNotifier][userOperationNotifier]") {
    lwm2m_context_t context;
    const OBJ_LINK_T id {5, 0};
    FirmwareUpdateMock fwu(context, id);

    SECTION("serverOperationNotifier") {
        INT_T state;

        fwu.serverOperationNotifier(ResOp::WRITE_UPD, {FirmwareUpdate::PACKAGE_0, 0});
        REQUIRE(fwu.get(3, state));
        REQUIRE(state == FirmwareUpdate::S_IDLE);
        REQUIRE(fwu.get(5, state));
        REQUIRE(state == FirmwareUpdate::R_INITIAL);

        fwu.serverOperationNotifier(ResOp::WRITE_UPD, {FirmwareUpdate::PACKAGE_URI_1, 0});
        REQUIRE(fwu.get(3, state));
        REQUIRE(state == FirmwareUpdate::S_IDLE);
        REQUIRE(fwu.get(5, state));
        REQUIRE(state == FirmwareUpdate::R_INITIAL);
        
        fwu.serverOperationNotifier(ResOp::EXECUTE, {FirmwareUpdate::UPDATE_2, 0});

        REQUIRE(fwu.set(9, (INT_T)FirmwareUpdate::PULL));
        REQUIRE(fwu.set(1, STRING_T("coap://localhost:5683/")));
        fwu.serverOperationNotifier(ResOp::WRITE_UPD, {FirmwareUpdate::PACKAGE_URI_1, 0});
        REQUIRE(fwu.get(3, state));
        REQUIRE(state == FirmwareUpdate::S_DOWNLOADING);
        REQUIRE(fwu.get(5, state));
        REQUIRE(state == FirmwareUpdate::R_INITIAL);

        REQUIRE(fwu.set(9, (INT_T)FirmwareUpdate::PUSH));
        REQUIRE(fwu.set(0, OPAQUE_T{0x01, 0x02, 0x03}));
        fwu.serverOperationNotifier(ResOp::WRITE_UPD, {FirmwareUpdate::PACKAGE_0, 0});
        REQUIRE(fwu.get(3, state));
        REQUIRE(state == FirmwareUpdate::S_DOWNLOADED);
        REQUIRE(fwu.get(5, state));
        REQUIRE(state == FirmwareUpdate::R_INITIAL);

        fwu.serverOperationNotifier(ResOp::EXECUTE, {FirmwareUpdate::UPDATE_2, 0});
    }

    SECTION("userOperationNotifier") {
        
    }
}

TEST_CASE("FirmwareUpdate", "[changeUpdRes][changeState][resetStateMachine][isUriValid][extractSchemeFromUri][isSchemeValid][isSchemeSupported][schemeToProtId][isNewStateValid][isDeliveryTypeSupported][startDeferUpdateGuard][stopDeferUpdateGuard]") {
    lwm2m_context_t context;
    const OBJ_LINK_T id {5, 0};
    FirmwareUpdate fwu(context, id);

}