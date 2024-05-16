#include "catch_amalgamated.hpp"
#include "WppConnection.h"
#include "WppTaskQueue.h"
#include "WppClient.h"

using namespace wpp;

static WppTaskQueue::task_t createDummyTask()
{
    return [](WppClient &client, void *ctx) -> bool
    {
        return true;
    };
}

TEST_CASE("WppClient", "[wppclient]")
{
    // Connection mock class for virtual connection | for return TRUE
    class WppConnectionMock : public WppConnection
    {
    public:
        SESSION_T connect(Lwm2mSecurity &security) override { return NULL; }
        void disconnect(SESSION_T session) override {}
        bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return true; }
        bool sendPacket(const Packet &packet) override { return true; }
    };

    // Create a packet for testing
    WppConnection::Packet testPacket;
    testPacket.session = nullptr;
    testPacket.length = 10;
    uint8_t test_buffer[10] = {
        0};
    testPacket.buffer = test_buffer;

    // Create client info
    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";

    WppConnectionMock conmock;

    SECTION("client-takeOwnership")
    {
        REQUIRE(conmock.addPacketToQueue(testPacket));
        REQUIRE(WppClient::create(clientInfo, conmock));
        REQUIRE(WppClient::create(clientInfo, conmock));
        WppClient *defclient = WppClient::takeOwnership();

        REQUIRE(defclient->getState() == 0);

        REQUIRE(WppTaskQueue::getTaskCnt() == 0);
        REQUIRE(conmock.getPacketQueueSize() == 1);
        REQUIRE(defclient->loop() == 0);

        REQUIRE(defclient->takeOwnership() == NULL);
        defclient->giveOwnership();
        REQUIRE(defclient->takeOwnership() != NULL);

        conmock.handlePacketsInQueue(*defclient); // COAP_NO_ERROR

        WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, createDummyTask());
        REQUIRE(defclient->loop() == 0);
        WppTaskQueue::hardReset();

        WppTaskQueue::addTask(29, createDummyTask());
        REQUIRE(defclient->loop() == 0);
        WppTaskQueue::hardReset();

        WppTaskQueue::addTask(31, createDummyTask());
        REQUIRE(defclient->loop() == 0);
        WppTaskQueue::hardReset();

        defclient->send({}); // DataLink &link
        defclient->deregister();
        WppClient::remove();

        clientInfo.altPath = "0";
        REQUIRE(conmock.addPacketToQueue(testPacket));

        REQUIRE(conmock.getPacketQueueSize() == 1);
        conmock.clearPacketQueue();
        REQUIRE(conmock.getPacketQueueSize() == 0);

        REQUIRE_FALSE(WppClient::create(clientInfo, conmock));

        WppClient::remove();
        defclient->remove();

        clientInfo.altPath = "/1/0/1";
        REQUIRE(conmock.addPacketToQueue(testPacket));
        REQUIRE_FALSE(WppClient::create(clientInfo, conmock));
    }

    SECTION("takeOwnershipBlocking")
    {
        REQUIRE(conmock.addPacketToQueue(testPacket));
        REQUIRE(WppClient::create(clientInfo, conmock));

        WppClient *defclient = WppClient::takeOwnershipBlocking();
        defclient->giveOwnership();
        REQUIRE(defclient->takeOwnershipBlocking() != NULL);
        REQUIRE(defclient->takeOwnership() == NULL);

        conmock.clearPacketQueue();
        defclient->remove();

        REQUIRE(defclient->takeOwnershipBlocking() == NULL);
    }
}