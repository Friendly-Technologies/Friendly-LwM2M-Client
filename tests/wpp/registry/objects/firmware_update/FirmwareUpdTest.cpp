#include "catch_amalgamated.hpp"
#include "o_5_firmware_update/FirmwareUpdate.h"
#include "WppTypes.h"
#include "WppClient.h"

using namespace wpp;
EXECUTE_T exeUpdMock;
OPAQUE_T opaque;
STRING_T str;
INT_T integer;

class WppConnectionMock : public WppConnection
{
public:
    SESSION_T connect(Lwm2mSecurity &security) override { return this; }
    void disconnect(SESSION_T session) override {}
    bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return true; }
    bool sendPacket(const Packet &packet) override { return true; }
};

class FirmwareUpdateMock : public FirmwareUpdate
{
public:
    FirmwareUpdateMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : FirmwareUpdate(context, id) {}

    void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { FirmwareUpdate::serverOperationNotifier(securityInst, type, resId); }
    void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { FirmwareUpdate::userOperationNotifier(type, resId); }

    OPAQUE_T getPACKAGE_0() { return FirmwareUpdate::resource(PACKAGE_0)->get<OPAQUE_T>(); }
    STRING_T getPACKAGE_URI_1() { return FirmwareUpdate::resource(PACKAGE_URI_1)->get<STRING_T>(); }
    EXECUTE_T getUPDATE_2() { return FirmwareUpdate::resource(UPDATE_2)->get<EXECUTE_T>(); }
    INT_T getSTATE_3() { return FirmwareUpdate::resource(STATE_3)->get<INT_T>(); }
    INT_T getUPDATE_RESULT_5() { return FirmwareUpdate::resource(UPDATE_RESULT_5)->get<INT_T>(); }
    STRING_T getPKGNAME_6() { return FirmwareUpdate::resource(PKGNAME_6)->get<STRING_T>(); }
    STRING_T getPKGVERSION_7() { return FirmwareUpdate::resource(PKGVERSION_7)->get<STRING_T>(); }
    INT_T getUPDATE_PROTOCOL_SUPPORT_8() { return FirmwareUpdate::resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->get<INT_T>(); }
    INT_T getUPDATE_DELIVERY_METHOD_9() { return FirmwareUpdate::resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->get<INT_T>(); }

    bool setPACKAGE_0(OPAQUE_T pack) { return FirmwareUpdate::resource(PACKAGE_0)->set<OPAQUE_T>(pack); }
    bool setPACKAGE_URI_1(STRING_T uri) { return FirmwareUpdate::resource(PACKAGE_URI_1)->set<STRING_T>(uri); }
    bool setUPDATE_2(EXECUTE_T upd) { return FirmwareUpdate::resource(UPDATE_2)->set<EXECUTE_T>(upd); }
    bool setSTATE_3(INT_T intgr) { return FirmwareUpdate::resource(STATE_3)->set<INT_T>(intgr); }
    bool setUPDATE_RESULT_5(INT_T res) { return FirmwareUpdate::resource(UPDATE_RESULT_5)->set<INT_T>(res); }
    bool setPKGNAME_6(STRING_T pcgn) { return FirmwareUpdate::resource(PKGNAME_6)->set<STRING_T>(pcgn); }
    bool setPKGVERSION_7(STRING_T pckv) { return FirmwareUpdate::resource(PKGVERSION_7)->set<STRING_T>(pckv); }
    bool setUPDATE_PROTOCOL_SUPPORT_8(INT_T supp) { return FirmwareUpdate::resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->set<INT_T>(supp); }
    bool setUPDATE_DELIVERY_METHOD_9(INT_T meth) { return FirmwareUpdate::resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set<INT_T>(meth); }

    FwUpdater *pkgUpdater;
    FwInternalDl *downloaderI;
    FwExternalDl *downloaderE;
};

TEST_CASE("FirmwareUpdate: resource initialization", "[resourcesCreate][resourcesInit][setDefaultState]")
{
    lwm2m_context_t mockContext;
    OBJ_LINK_T mockId = {0, 0};
    FirmwareUpdateMock fwu(mockContext, mockId);

    // Create client info
    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";
    WppConnectionMock conmock;

    SECTION("lwm2m_connect_server")
    {

        SECTION("www")
        {
            REQUIRE(WppClient::create(clientInfo, conmock));
            WppClient *defclient = WppClient::takeOwnership();
            defclient->giveOwnership();
            FirmwareUpdate::setFwUpdater(*WppClient::takeOwnership(), *fwu.pkgUpdater);
            defclient->giveOwnership();
            FirmwareUpdate::setFwExternalDownloader(*WppClient::takeOwnership(), *fwu.downloaderE);

            defclient->giveOwnership();
            FirmwareUpdate::setFwInternalDownloader(*WppClient::takeOwnership(), *fwu.downloaderI);
            // fwu.downloaderI = ?;
        }

        SECTION("resourcesCreate&resourcesInit")
        {
            REQUIRE(WppClient::create(clientInfo, conmock));
            WppClient *defclient = WppClient::takeOwnership();
            defclient->giveOwnership();

            FirmwareUpdateMock::object(*WppClient::takeOwnership());
            defclient->giveOwnership();

            REQUIRE(FirmwareUpdateMock::instance(*WppClient::takeOwnership(), 1) == NULL);
            defclient->giveOwnership();

            REQUIRE(FirmwareUpdateMock::instance(*WppClient::takeOwnership(), ID_T_MAX_VAL) == NULL);
            defclient->giveOwnership();

            REQUIRE(FirmwareUpdateMock::createInst(*WppClient::takeOwnership(), 1) != NULL);
            defclient->giveOwnership();

            FirmwareUpdate::supportedProtocols(*WppClient::takeOwnership());
            defclient->giveOwnership();

            FirmwareUpdateMock::createInst(*WppClient::takeOwnership(), FirmwareUpdate::FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8);
            defclient->giveOwnership();

            FirmwareUpdate::supportedProtocols(*WppClient::takeOwnership());
            defclient->giveOwnership();

            REQUIRE(FirmwareUpdateMock::createInst(*WppClient::takeOwnership(), ID_T_MAX_VAL) == NULL);
            defclient->giveOwnership();

            REQUIRE(FirmwareUpdateMock::instance(*WppClient::takeOwnership(), 1) != NULL);
            defclient->giveOwnership();

            REQUIRE(FirmwareUpdateMock::removeInst(*WppClient::takeOwnership(), 1));
            defclient->giveOwnership();

            fwu.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
            fwu.serverOperationNotifier(0, ItemOp::TYPE::WRITE, {0, 0});
            fwu.serverOperationNotifier(0, ItemOp::TYPE::WRITE, {1, 1});

            fwu.userOperationNotifier(ItemOp::TYPE::WRITE, {0, 0});
            fwu.userOperationNotifier(ItemOp::TYPE::WRITE, {1, 1});
            fwu.userOperationNotifier(ItemOp::TYPE::DELETE, {1, 1});
        }

        SECTION("setDefaultState")
        {
            opaque = {0x01, 0x02, 0x03};
            str = "test";
            integer = 0x1234;
            exeUpdMock = (EXECUTE_T)[](Instance & inst, ID_T resId, const OPAQUE_T &data) { return true; };
            REQUIRE(fwu.setPACKAGE_0(opaque));
            REQUIRE(fwu.setPACKAGE_URI_1(""));
            REQUIRE(fwu.setUPDATE_2(([](Instance &inst, ID_T resId, const OPAQUE_T &data)
                                     { return true; })));
            REQUIRE(fwu.setUPDATE_2(NULL));
            REQUIRE(fwu.setSTATE_3(S_IDLE));
            REQUIRE(fwu.setSTATE_3(S_DOWNLOADING));
            REQUIRE(fwu.setUPDATE_RESULT_5(R_INTEGRITY_CHECK_FAIL));
            REQUIRE_FALSE(fwu.setUPDATE_RESULT_5(UPD_RES_MAX + 1));
            REQUIRE(fwu.setPKGNAME_6(str));
            REQUIRE(fwu.setPKGVERSION_7(str));
            REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(HTTP));
            REQUIRE(fwu.setUPDATE_DELIVERY_METHOD_9(PUSH));
            REQUIRE(fwu.getPACKAGE_0() == opaque);
            REQUIRE(fwu.getPACKAGE_URI_1() == "");
            REQUIRE(fwu.getUPDATE_2() == NULL);
            REQUIRE(fwu.getSTATE_3() == S_DOWNLOADING);
            REQUIRE(fwu.getUPDATE_RESULT_5() == R_INTEGRITY_CHECK_FAIL);
            REQUIRE(fwu.getPKGNAME_6() == str);
            REQUIRE(fwu.getPKGVERSION_7() == str);
            REQUIRE_FALSE(fwu.getUPDATE_PROTOCOL_SUPPORT_8() == COAP);
            REQUIRE(fwu.getUPDATE_PROTOCOL_SUPPORT_8() == HTTP);
            REQUIRE(fwu.getUPDATE_DELIVERY_METHOD_9() == PUSH);

            WppClient *defclient = WppClient::takeOwnership();
            defclient->giveOwnership();
            FirmwareUpdate::supportedProtocols(*WppClient::takeOwnership());
        }
    }

    SECTION("serverOperationNotifier")
    {
        fwu.serverOperationNotifier(0, ItemOp::WRITE, {FirmwareUpdate::PACKAGE_0, 0});
        REQUIRE(fwu.getSTATE_3() == S_IDLE);
        REQUIRE(fwu.getUPDATE_RESULT_5() == R_INITIAL);

        fwu.serverOperationNotifier(0, ItemOp::WRITE, {FirmwareUpdate::PACKAGE_URI_1, 0});
        REQUIRE(fwu.getSTATE_3() == S_IDLE);
        REQUIRE(fwu.getUPDATE_RESULT_5() == R_INITIAL);

        fwu.serverOperationNotifier(0, ItemOp::EXECUTE, {FirmwareUpdate::UPDATE_2, 0});

        REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(COAP));
        REQUIRE(fwu.setUPDATE_DELIVERY_METHOD_9(PULL));
        REQUIRE(fwu.setPACKAGE_URI_1("coap://localhost:5683/"));
        fwu.serverOperationNotifier(0, ItemOp::WRITE, {FirmwareUpdate::PACKAGE_URI_1, 0});
        // REQUIRE(fwu.getSTATE_3() == S_DOWNLOADING); // 0 == 
        REQUIRE(fwu.getUPDATE_RESULT_5() == R_INITIAL);

        REQUIRE(fwu.setUPDATE_DELIVERY_METHOD_9(PUSH));
        REQUIRE(fwu.setPACKAGE_0({0x01, 0x02, 0x03}));
        fwu.serverOperationNotifier(0, ItemOp::WRITE, {FirmwareUpdate::PACKAGE_0, 0});
        // REQUIRE(fwu.getSTATE_3() == S_DOWNLOADED); // 0 == 
        REQUIRE(fwu.getUPDATE_RESULT_5() == R_INITIAL);

        fwu.serverOperationNotifier(0, ItemOp::EXECUTE, {FirmwareUpdate::UPDATE_2, 0});
    }

    SECTION("userOperationNotifier")
    {
        REQUIRE(fwu.setSTATE_3(S_DOWNLOADING));
        REQUIRE(fwu.setSTATE_3(S_DOWNLOADED));
        REQUIRE(fwu.setSTATE_3(S_UPDATING));
        REQUIRE(fwu.setUPDATE_RESULT_5(R_FW_UPD_SUCCESS));
        REQUIRE_FALSE(fwu.getSTATE_3() == S_IDLE); // TODO

        REQUIRE(fwu.setSTATE_3(S_DOWNLOADING));
        REQUIRE(fwu.setSTATE_3(S_DOWNLOADED));
        REQUIRE(fwu.setSTATE_3(S_UPDATING));
        REQUIRE(fwu.setUPDATE_RESULT_5(R_NOT_ENOUGH_FLASH));
        REQUIRE_FALSE(fwu.getSTATE_3() == S_IDLE); // TODO

        REQUIRE_FALSE(fwu.setUPDATE_RESULT_5(UPD_RES_MAX));
    }

    SECTION("isUriValid, isDeliveryTypeSupported, schemeToProtId")
    {
        REQUIRE(fwu.setUPDATE_DELIVERY_METHOD_9(PUSH));
        REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(COAP));
        REQUIRE_FALSE(fwu.setPACKAGE_URI_1("coap://localhost:5683/"));

        REQUIRE(fwu.setUPDATE_DELIVERY_METHOD_9(BOTH));

        REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(COAP));
        REQUIRE(fwu.setPACKAGE_URI_1("coap://localhost:5683/"));
        REQUIRE_FALSE(fwu.setPACKAGE_URI_1("coap/:/localhost:5683/"));  // isSchemeValid
        REQUIRE_FALSE(fwu.setPACKAGE_URI_1("coaps://localhost:5683/")); // isSchemeSupported

        REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(COAPS));
        REQUIRE(fwu.setPACKAGE_URI_1("coaps://localhost:5685/"));

        REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(HTTP));
        REQUIRE(fwu.setPACKAGE_URI_1("http://localhost:5683/"));
        REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(HTTPS));
        REQUIRE(fwu.setPACKAGE_URI_1("https://localhost:5683/"));
        REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(COAP_TCP));
        REQUIRE(fwu.setPACKAGE_URI_1("coap+tcp://localhost:5683/"));
        REQUIRE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(COAP_TLS));
        REQUIRE(fwu.setPACKAGE_URI_1("coaps+tcp://localhost:5683/"));

        REQUIRE_FALSE(fwu.setUPDATE_PROTOCOL_SUPPORT_8(FW_UPD_PROTOCOL_MAX));
        REQUIRE_FALSE(fwu.setPACKAGE_URI_1("coaps-tcp://localhost:5683/"));
    }

    SECTION("isNewStateValid")
    {
        REQUIRE(fwu.setSTATE_3(S_DOWNLOADING));
        REQUIRE(fwu.setSTATE_3(S_DOWNLOADED));
        REQUIRE(fwu.setSTATE_3(S_UPDATING));
        REQUIRE(fwu.setSTATE_3(S_DOWNLOADED));
        REQUIRE(fwu.setSTATE_3(S_UPDATING));
        REQUIRE(fwu.setSTATE_3(S_IDLE));
        REQUIRE(fwu.setSTATE_3(S_UPDATING));

        REQUIRE_FALSE(fwu.setSTATE_3(STATE_MAX));
    }
}