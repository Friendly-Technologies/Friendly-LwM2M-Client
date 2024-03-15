#include "catch_amalgamated.hpp"
#include "WppConnection.h"
#include "WppClient.h"

using namespace wpp;

// for return TRUE
class WppConnectionMock : public WppConnection
{
public:
    SESSION_T connect(Lwm2mSecurity &security) override { return this; }
    void disconnect(SESSION_T session) override {}
    bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return true; }
    bool sendPacket(const Packet &packet) override { return true; }
};

class WppConnectionNullMock : public WppConnection
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
    testPacket.length = 10;
    uint8_t test_buffer[10] = {
        0,
    };
    testPacket.buffer = test_buffer;

    // Create client info
    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";

    SECTION("DataBlockSize")
    {
        uint16_t valid_block_sizes[] = {16, 32, 64, 128, 256, 512, 1024};
        size_t sizeOfArray = sizeof(valid_block_sizes) / sizeof(valid_block_sizes[0]);

        WppConnectionMock connection;

        REQUIRE(connection.getDataBlockSize() == LWM2M_COAP_DEFAULT_BLOCK_SIZE);

        for (uint8_t i = 0; i < sizeOfArray; i++)
        {
            REQUIRE(connection.setDataBlockSize(valid_block_sizes[i]));
            REQUIRE(connection.getDataBlockSize() == valid_block_sizes[i]);
        }

        REQUIRE_FALSE(connection.setDataBlockSize(500));
        REQUIRE(connection.getDataBlockSize() == 1024);
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
            dummyPacket.buffer = test_buffer;
            REQUIRE(conmock.addPacketToQueue(dummyPacket));
            REQUIRE(conmock.getPacketQueueSize() == i + 1);
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

        REQUIRE(conmock.addPacketToQueue(testPacket));
        REQUIRE(conmock.getPacketQueueSize() == 1);

        conmock.clearPacketQueue();
        REQUIRE(conmock.getPacketQueueSize() == 0);

        defclient->remove();
    }
}

TEST_CASE("WppPlatform", "[lwm2m_connect_server][lwm2m_close_connection][lwm2m_buffer_send][lwm2m_session_is_equal]") {
    WppClient::ClientInfo clientInfo;
    clientInfo.endpointName = "exampleEndpoint";
    clientInfo.msisdn = "1234567890";
    clientInfo.altPath = "";
    WppConnectionMock conmock;
    WppConnectionNullMock connullmock;

    SECTION("lwm2m_connect_server") {
        REQUIRE(WppClient::create(clientInfo, conmock));
        WppClient *defclient = WppClient::takeOwnership();
        REQUIRE(defclient != NULL);
        REQUIRE(lwm2m_connect_server(0, NULL) == NULL);
        REQUIRE(lwm2m_connect_server(0, defclient) == NULL);
        defclient->registry().lwm2mSecurity().createInstance(0);
        REQUIRE(lwm2m_connect_server(0, defclient) != NULL);
        defclient->remove();

        REQUIRE(WppClient::create(clientInfo, connullmock));
        defclient = WppClient::takeOwnership();
        REQUIRE(defclient != NULL);
        defclient->registry().lwm2mSecurity().createInstance(0);
        REQUIRE(lwm2m_connect_server(0, defclient) == NULL);
        defclient->remove();
    }

    SECTION("lwm2m_close_connection") {
        REQUIRE(WppClient::create(clientInfo, conmock));
        WppClient *defclient = WppClient::takeOwnership();
        REQUIRE(defclient != NULL);
        lwm2m_close_connection(NULL, NULL);
        lwm2m_close_connection(NULL, defclient);
        defclient->remove();
    }

    SECTION("lwm2m_buffer_send") {
        REQUIRE(WppClient::create(clientInfo, conmock));
        WppClient *defclient = WppClient::takeOwnership();
        REQUIRE(defclient != NULL);
        REQUIRE(lwm2m_buffer_send(NULL, NULL, 0, NULL) == COAP_500_INTERNAL_SERVER_ERROR);
        REQUIRE(lwm2m_buffer_send(NULL, NULL, 0, defclient) == COAP_NO_ERROR);
        defclient->remove();

        REQUIRE(WppClient::create(clientInfo, connullmock));
        defclient = WppClient::takeOwnership();
        REQUIRE(defclient != NULL);
        REQUIRE(lwm2m_buffer_send(NULL, NULL, 0, defclient) == COAP_500_INTERNAL_SERVER_ERROR);
        defclient->remove();
    }

    SECTION("lwm2m_session_is_equal") {
        REQUIRE(WppClient::create(clientInfo, conmock));
        WppClient *defclient = WppClient::takeOwnership();
        REQUIRE(defclient != NULL);
        REQUIRE(lwm2m_session_is_equal(NULL, NULL, NULL) == false);
        REQUIRE(lwm2m_session_is_equal(NULL, NULL, defclient) == true);
        defclient->remove();

        REQUIRE(WppClient::create(clientInfo, connullmock));
        defclient = WppClient::takeOwnership();
        REQUIRE(defclient != NULL);
        REQUIRE(lwm2m_session_is_equal(NULL, NULL, defclient) == false);
        defclient->remove();
    }
}
