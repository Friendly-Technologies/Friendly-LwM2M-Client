#include "catch_amalgamated.hpp"
#include "WppConnection.h"
#include "WppClient.h"

using namespace wpp;

// for return TRUE
class WppConnectionMock : public WppConnection
{
public:
    SESSION_T connect(Lwm2mSecurity &security) override { return NULL; }
    void disconnect(SESSION_T session) override {}
    bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return true; }
    bool sendPacket(const Packet &packet) override { return true; }
};

// for return FALSE
class WppConnectionMockErr : public WppConnection
{
public:
    SESSION_T connect(Lwm2mSecurity &security) override { return NULL; }
    void disconnect(SESSION_T session) override {}
    bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return false; }
    bool sendPacket(const Packet &packet) override { return false; }
};

TEST_CASE("WppConnection", "[wppconnection]")
{
    // Create a packet for testing
    WppConnection::Packet testPacket;
    testPacket.session = nullptr;
    testPacket.length = 9;
    testPacket.buffer = new uint8_t[testPacket.length];

    // Create client info
    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";

    SECTION("connection_without_object")
    {
        WppConnection *connection;

        REQUIRE(connection->addPacketToQueue(testPacket) == false);
        REQUIRE(connection->getDataBlockSize() == 1024);
        REQUIRE(connection->setDataBlockSize(512));
        REQUIRE(connection->getDataBlockSize() == 512);
    }

    SECTION("connection_with_object")
    {
        WppConnectionMock conmock;

        REQUIRE(conmock.addPacketToQueue(testPacket));
        REQUIRE(conmock.getPacketQueueSize() == 1);

        conmock.clearPacketQueue();

        // Force the queue to be full (assuming packets has a max size)
        for (size_t i = 0; i < WPP_CONN_I_PACKETS_QUEUE_SIZE; ++i)
        {
            WppConnection::Packet dummyPacket;
            dummyPacket.session = nullptr;
            dummyPacket.length = 10;
            dummyPacket.buffer = new uint8_t[dummyPacket.length];
            conmock.addPacketToQueue(dummyPacket);
        }

        // Add the test packet to the full queue, it should fail
        bool result = conmock.addPacketToQueue(testPacket);

        // Add your assertions here to check if the packet is not added due to failure
        REQUIRE(result == false);
    }

    SECTION("handlepackets")
    {
        WppConnectionMock conmock;
        WppClient::remove();

        REQUIRE(conmock.addPacketToQueue(testPacket));
        REQUIRE(WppClient::create(clientInfo, conmock));
        WppClient *defclient = WppClient::takeOwnership();
        conmock.handlePacketsInQueue(*defclient); // COAP_NO_ERROR

        REQUIRE(defclient->getState() == 0);
        REQUIRE(conmock.addPacketToQueue(testPacket));

        REQUIRE(defclient->takeOwnership() == NULL);
        defclient->giveOwnership();
        REQUIRE(defclient->takeOwnership() != NULL);

        conmock.handlePacketsInQueue(*defclient); // COAP_NO_ERROR

        REQUIRE(WppClient::isCreated());
        REQUIRE(WppClient::create(clientInfo, conmock));

        WppClient::remove();
        REQUIRE_FALSE(WppClient::isCreated());

        clientInfo.altPath = "0";
        REQUIRE_FALSE(WppClient::create(clientInfo, conmock));

        REQUIRE(conmock.addPacketToQueue(testPacket));
        REQUIRE(conmock.getPacketQueueSize() == 1);
        conmock.clearPacketQueue();
        REQUIRE(conmock.getPacketQueueSize() == 0);
        defclient->remove();
    }

    SECTION("takeOwnershipBlocking")
    {
        WppConnectionMockErr conmock;

        REQUIRE(conmock.addPacketToQueue(testPacket));
        REQUIRE(WppClient::create(clientInfo, conmock));

        WppClient *defclient = WppClient::takeOwnershipBlocking();
        defclient->giveOwnership();
        REQUIRE(defclient->takeOwnershipBlocking() != NULL);
        
        conmock.handlePacketsInQueue(*defclient); // COAP_500_INTERNAL_SERVER_ERROR

        conmock.clearPacketQueue();
        defclient->remove();

        REQUIRE(defclient->takeOwnershipBlocking() == NULL);
    }
}
