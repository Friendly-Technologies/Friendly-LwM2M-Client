#include "catch_amalgamated.hpp"
#include "o_4_connectivity_monitoring/ConnectivityMonitoring.h"
#include "WppClient.h"

#define NTWRK_BRR_MIN 0
#define NTWRK_BRR_MAX 50
#define AVLB_NTWRK_BRR_MIN 0
#define AVLB_NTWRK_BRR_MAX 50
#define LINK_QUALITY_IEEE_802_15_4_MIN 0
#define LINK_QUALITY_IEEE_802_15_4_MAX 255
#define LINK_QUALITY_GSM_MIN 0
#define LINK_QUALITY_GSM_MAX 7
#define CELL_ID_GSM_MIN 0
#define CELL_ID_GSM_MAX 65535
#define CELL_ID_WCDMA_MIN 0
#define CELL_ID_WCDMA_MAX 268435455

using namespace wpp;

TEST_CASE("objectConnectivityMonitoring", "[objectConnectivityMonitoring]")
{
    SECTION("objectConnectivityMonitoring")
    {
        class ConnectivityMonitoringMock : public ConnectivityMonitoring
        {
        public:
            ConnectivityMonitoringMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : ConnectivityMonitoring(context, id) {}

            void setDefaultState()
            {
                // ConnectivityMonitoring::setDefaultState(); // TODO

                // return true as for as resource not used in the another cases
                REQUIRE(ConnectivityMonitoring::checkLinkQuality(0));
                REQUIRE(ConnectivityMonitoring::checkCellId(0));

                // return GSM <= value && value < NTWRK_BRR_MAX
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(GSM)));
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(NTWRK_BRR_MAX - 1)));
                ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(GSM - 1)); // return is undefined, only for good coverage
                REQUIRE_FALSE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(NTWRK_BRR_MAX)));
                REQUIRE_FALSE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(NTWRK_BRR_MAX + 5)));

                // return AVLB_NTWRK_BRR_MIN <= value && value <= AVLB_NTWRK_BRR_MAX
                REQUIRE(ConnectivityMonitoring::resource(AVAILABLE_NETWORK_BEARER_1)->set(INT_T(AVLB_NTWRK_BRR_MIN)));
                REQUIRE(ConnectivityMonitoring::resource(AVAILABLE_NETWORK_BEARER_1)->set(INT_T(AVLB_NTWRK_BRR_MAX)));
                ConnectivityMonitoring::resource(AVAILABLE_NETWORK_BEARER_1)->set(INT_T(AVLB_NTWRK_BRR_MIN - 1)); // return is undefined, only for good coverage
                REQUIRE_FALSE(ConnectivityMonitoring::resource(AVAILABLE_NETWORK_BEARER_1)->set(INT_T(AVLB_NTWRK_BRR_MAX + 1)));
                REQUIRE_FALSE(ConnectivityMonitoring::resource(AVAILABLE_NETWORK_BEARER_1)->set(INT_T(AVLB_NTWRK_BRR_MAX + 5)));

                //
                // bool ConnectivityMonitoring::checkLinkQuality(uint8_t linkQuality)
                // GSM
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(GSM)));
                // return LINK_QUALITY_GSM_MIN <= linkQuality && linkQuality <= LINK_QUALITY_GSM_MAX
                REQUIRE(ConnectivityMonitoring::checkLinkQuality(LINK_QUALITY_GSM_MIN));
                REQUIRE(ConnectivityMonitoring::checkLinkQuality(LINK_QUALITY_GSM_MAX));
                ConnectivityMonitoring::checkLinkQuality(LINK_QUALITY_GSM_MIN - 1); // return is undefined, only for good coverage
                REQUIRE_FALSE(ConnectivityMonitoring::checkLinkQuality(LINK_QUALITY_GSM_MAX + 1));
                // IEEE_802_15_4
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(IEEE_802_15_4)));
                // return LINK_QUALITY_IEEE_802_15_4_MIN <= linkQuality && linkQuality <= LINK_QUALITY_IEEE_802_15_4_MAX
                REQUIRE(ConnectivityMonitoring::checkLinkQuality(LINK_QUALITY_IEEE_802_15_4_MIN));
                REQUIRE(ConnectivityMonitoring::checkLinkQuality(LINK_QUALITY_IEEE_802_15_4_MAX));
                ConnectivityMonitoring::checkLinkQuality(LINK_QUALITY_IEEE_802_15_4_MIN - 1); // return is undefined, only for good coverage
                // LTE_TDD
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(LTE_TDD)));
                REQUIRE(ConnectivityMonitoring::checkLinkQuality(0));
                // LTE_FDD
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(LTE_FDD)));
                REQUIRE(ConnectivityMonitoring::checkLinkQuality(0));
                // NB_IOT
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(NB_IOT)));
                REQUIRE(ConnectivityMonitoring::checkLinkQuality(0));
                //

                //
                // bool ConnectivityMonitoring::checkCellId(uint32_t cellId)
                // GSM
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(GSM)));
                // return CELL_ID_GSM_MIN <= cellId && cellId <= CELL_ID_GSM_MAX
                REQUIRE(ConnectivityMonitoring::checkCellId(CELL_ID_GSM_MIN));
                REQUIRE(ConnectivityMonitoring::checkCellId(CELL_ID_GSM_MAX));
                ConnectivityMonitoring::checkCellId(CELL_ID_GSM_MIN - 1); // return is undefined, only for good coverage
                REQUIRE_FALSE(ConnectivityMonitoring::checkCellId(CELL_ID_GSM_MAX + 1));
                // WCDMA
                REQUIRE(ConnectivityMonitoring::resource(NETWORK_BEARER_0)->set(INT_T(WCDMA)));
                // return CELL_ID_WCDMA_MIN <= cellId && cellId <= CELL_ID_WCDMA_MAX
                REQUIRE(ConnectivityMonitoring::checkCellId(CELL_ID_WCDMA_MIN));
                REQUIRE(ConnectivityMonitoring::checkCellId(CELL_ID_WCDMA_MAX));
                ConnectivityMonitoring::checkCellId(CELL_ID_WCDMA_MIN - 1); // return is undefined, only for good coverage
                REQUIRE_FALSE(ConnectivityMonitoring::checkCellId(CELL_ID_WCDMA_MAX + 1));
                //
            }

            void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { ConnectivityMonitoring::serverOperationNotifier(securityInst, type, resId); }
            void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { ConnectivityMonitoring::userOperationNotifier(type, resId); }
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        ConnectivityMonitoringMock connectivitymonitoringMock(mockContext, mockId);

        mockContext.state = STATE_INITIAL;

        connectivitymonitoringMock.setDefaultState();
        connectivitymonitoringMock.serverOperationNotifier(&connectivitymonitoringMock, ItemOp::TYPE::READ, {0, 0}); // TODO Instance *securityInst
        connectivitymonitoringMock.userOperationNotifier(ItemOp::TYPE::WRITE, {10, 10});
    }

    SECTION("instansesConnectivityMonitoring")
    {

        class ConnectivityMonitoringMock : public ConnectivityMonitoring
        {
        public:
            ConnectivityMonitoringMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : ConnectivityMonitoring(context, id) {}
        };

        WppClient::ClientInfo clientInfo;
        clientInfo.endpointName = "exampleEndpoint";
        clientInfo.msisdn = "1234567890";
        clientInfo.altPath = "";

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 0};
        ConnectivityMonitoringMock connm(mockContext, mockId);
        WppClient *defclient = WppClient::takeOwnership();
        defclient->giveOwnership();

        connm.object(*WppClient::takeOwnership());
        defclient->giveOwnership();

        REQUIRE(connm.instance(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(connm.instance(*WppClient::takeOwnership(), ID_T_MAX_VAL) == NULL);
        defclient->giveOwnership();

        REQUIRE(connm.createInst(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();
        REQUIRE(connm.createInst(*WppClient::takeOwnership(), 1) == NULL);
        defclient->giveOwnership();

        REQUIRE(connm.instance(*WppClient::takeOwnership(), 1) != NULL);
        defclient->giveOwnership();

        REQUIRE(connm.removeInst(*WppClient::takeOwnership(), 1));
        defclient->giveOwnership();
    }
}