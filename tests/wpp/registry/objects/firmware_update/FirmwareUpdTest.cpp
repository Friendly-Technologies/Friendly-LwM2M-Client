#include "catch_amalgamated.hpp"
#include "o_5_firmware_update/FirmwareUpdate.h"

using namespace wpp;

class FirmwareUpdateMock : public FirmwareUpdate {
public:
    FirmwareUpdateMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : FirmwareUpdate(context, id) {}

    void setDefaultState() {
        // FirmwareUpdate::setDefaultState(); TODO
    }
    
	void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) {
        FirmwareUpdate::serverOperationNotifier(securityInst, type, resId);
    }

	void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) {
        FirmwareUpdate::userOperationNotifier(type, resId);
    }
};

TEST_CASE("FirmwareUpdate: resource initialization", "[resourcesCreate][resourcesInit][setDefaultState]") {
    OPAQUE_T opaque;
    STRING_T str;
    // INT_T integer;
    EXECUTE_T execute;
    
    lwm2m_context_t context;
    const OBJ_LINK_T id {5, 0};
    FirmwareUpdateMock fwu(context, id);

    SECTION("resourcesCreate&resourcesInit") {
        // REQUIRE(fwu.get(0, opaque));
        REQUIRE(opaque.size() == 0);
        // REQUIRE(fwu.get(1, str));
        REQUIRE(str.size() == 0);
        // REQUIRE(fwu.get(2, execute));
        execute(fwu, 2, opaque);
        REQUIRE(execute != nullptr);
        // REQUIRE(fwu.get(3, integer));
        // REQUIRE(integer == FirmwareUpdate::S_IDLE);
        // REQUIRE(fwu.get(5, integer));
        // REQUIRE(integer == FirmwareUpdate::R_INITIAL);
        // REQUIRE(fwu.get(6, str));
        REQUIRE(str.size() == 0);
        // REQUIRE(fwu.get(7, str));
        REQUIRE(str.size() == 0);
        // REQUIRE(fwu.get(8, integer));
        // REQUIRE(integer == FirmwareUpdate::COAP);
        // REQUIRE(fwu.get(9, integer));
        // REQUIRE(integer == FirmwareUpdate::PUSH);
    }

    SECTION("setDefaultState") {
        opaque = {0x01, 0x02, 0x03};
        str = "test";
        // integer = 0x1234;
        execute = (EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; };
        // REQUIRE(fwu.set(0, opaque));
        // REQUIRE(fwu.set(1, STRING_T("")));
        // REQUIRE(fwu.set(2, execute));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADING));
        // REQUIRE(fwu.set(5, (INT_T)FirmwareUpdate::R_INTEGRITY_CHECK_FAIL));
        // REQUIRE(fwu.set(6, str));
        // REQUIRE(fwu.set(7, str));
        // REQUIRE(fwu.set(8, (INT_T)FirmwareUpdate::HTTP));
        // REQUIRE(fwu.set(9, (INT_T)FirmwareUpdate::PUSH));

        fwu.setDefaultState();
        // REQUIRE(fwu.get(0, opaque));
        REQUIRE(opaque.size() == 0);
        // REQUIRE(fwu.get(1, str));
        REQUIRE(str.size() == 0);
        // REQUIRE(fwu.get(2, execute));
        REQUIRE(execute != nullptr);
        // REQUIRE(fwu.get(3, integer));
        // REQUIRE(integer == FirmwareUpdate::S_IDLE);
        // REQUIRE(fwu.get(5, integer));
        // REQUIRE(integer == FirmwareUpdate::R_INITIAL);
        // REQUIRE(fwu.get(6, str));
        REQUIRE(str.size() == 0);
        // REQUIRE(fwu.get(7, str));
        REQUIRE(str.size() == 0);
        // REQUIRE(fwu.get(8, integer));
        // REQUIRE(integer == FirmwareUpdate::COAP);
        // REQUIRE(fwu.get(9, integer));
        // REQUIRE(integer == FirmwareUpdate::PUSH);
    }
}

TEST_CASE("FirmwareUpdate", "[serverOperationNotifier][userOperationNotifier]") {
    lwm2m_context_t context;
    const OBJ_LINK_T id {5, 0};
    FirmwareUpdateMock fwu(context, id);

    SECTION("serverOperationNotifier") {
        // INT_T state;

        // fwu.serverOperationNotifier(ItemOp::WRITE, {FirmwareUpdate::PACKAGE_0, 0});
        // REQUIRE(fwu.get(3, state));
        // REQUIRE(state == FirmwareUpdate::S_IDLE);
        // REQUIRE(fwu.get(5, state));
        // REQUIRE(state == FirmwareUpdate::R_INITIAL);

        // fwu.serverOperationNotifier(ItemOp::WRITE, {FirmwareUpdate::PACKAGE_URI_1, 0});
        // REQUIRE(fwu.get(3, state));
        // REQUIRE(state == FirmwareUpdate::S_IDLE);
        // REQUIRE(fwu.get(5, state));
        // REQUIRE(state == FirmwareUpdate::R_INITIAL);
        
        // fwu.serverOperationNotifier(ItemOp::EXECUTE, {FirmwareUpdate::UPDATE_2, 0});

        // REQUIRE(fwu.set(9, (INT_T)FirmwareUpdate::PULL));
        // REQUIRE(fwu.set(1, STRING_T("coap://localhost:5683/")));
        // fwu.serverOperationNotifier(ItemOp::WRITE, {FirmwareUpdate::PACKAGE_URI_1, 0});
        // REQUIRE(fwu.get(3, state));
        // REQUIRE(state == FirmwareUpdate::S_DOWNLOADING);
        // REQUIRE(fwu.get(5, state));
        // REQUIRE(state == FirmwareUpdate::R_INITIAL);

        // REQUIRE(fwu.set(9, (INT_T)FirmwareUpdate::PUSH));
        // REQUIRE(fwu.set(0, OPAQUE_T{0x01, 0x02, 0x03}));
        // fwu.serverOperationNotifier(ItemOp::WRITE, {FirmwareUpdate::PACKAGE_0, 0});
        // REQUIRE(fwu.get(3, state));
        // REQUIRE(state == FirmwareUpdate::S_DOWNLOADED);
        // REQUIRE(fwu.get(5, state));
        // REQUIRE(state == FirmwareUpdate::R_INITIAL);

        // fwu.serverOperationNotifier(ItemOp::EXECUTE, {FirmwareUpdate::UPDATE_2, 0});
    }

    SECTION("userOperationNotifier") {
        // INT_T state;

        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADING));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADED));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_UPDATING));
        // REQUIRE(fwu.set(5, (INT_T)FirmwareUpdate::R_FW_UPD_SUCCESS));
        // REQUIRE(fwu.get(3, state));
        // REQUIRE(state == FirmwareUpdate::S_IDLE);

        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADING));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADED));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_UPDATING));
        // REQUIRE(fwu.set(5, (INT_T)FirmwareUpdate::R_NOT_ENOUGH_FLASH));
        // REQUIRE(fwu.get(3, state));
        // REQUIRE(state == FirmwareUpdate::S_IDLE);

        // REQUIRE_FALSE(fwu.set(5, (INT_T)FirmwareUpdate::UPD_RES_MAX));
    }
}

TEST_CASE("FirmwareUpdate", "[isUriValid][schemeToProtId][isDeliveryTypeSupported][isNewStateValid]") {
    lwm2m_context_t context;
    const OBJ_LINK_T id {5, 0};
    FirmwareUpdate fwu(context, id);
    
    SECTION("isUriValid, isDeliveryTypeSupported, schemeToProtId") {
        // REQUIRE(fwu.set(9, (INT_T)FirmwareUpdate::BOTH));
        // REQUIRE(fwu.set({8, 1}, INT_T(FirmwareUpdate::COAPS)));
        // REQUIRE(fwu.set({8, 2}, INT_T(FirmwareUpdate::HTTP)));
        // REQUIRE(fwu.set({8, 3}, INT_T(FirmwareUpdate::HTTPS)));
        // REQUIRE(fwu.set({8, 4}, INT_T(FirmwareUpdate::COAP_TCP)));
        // REQUIRE(fwu.set({8, 5}, INT_T(FirmwareUpdate::COAP_TLS)));
        // REQUIRE_FALSE(fwu.set({8, 6}, INT_T(FirmwareUpdate::FW_UPD_PROTOCOL_MAX)));

        // REQUIRE(fwu.set(1, STRING_T("coap://localhost:5683/")));
        // REQUIRE(fwu.set(1, STRING_T("coaps://localhost:5683/")));
        // REQUIRE(fwu.set(1, STRING_T("http://localhost:5683/")));
        // REQUIRE(fwu.set(1, STRING_T("https://localhost:5683/")));
        // REQUIRE(fwu.set(1, STRING_T("coap+tcp://localhost:5683/")));
        // REQUIRE(fwu.set(1, STRING_T("coaps+tcp://localhost:5683/")));

        // REQUIRE_FALSE(fwu.set(1, STRING_T("coapss://localhost:5683/")));
        // fwu.remove({8, 1});
        // REQUIRE_FALSE(fwu.set(1, STRING_T("coaps://localhost:5683/")));
    }

    SECTION("isNewStateValid") {
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADING));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADED));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_UPDATING));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_DOWNLOADED));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_UPDATING));
        // REQUIRE(fwu.set(3, (INT_T)FirmwareUpdate::S_IDLE));
        // REQUIRE_FALSE(fwu.set(3, (INT_T)FirmwareUpdate::STATE_MAX));
        // REQUIRE_FALSE(fwu.set(3, (INT_T)FirmwareUpdate::S_UPDATING));
    }
}