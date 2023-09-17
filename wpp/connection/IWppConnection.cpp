/*
 * IWppConnection.cpp
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#include "IWppConnection.h"
//TODO: #include "liblwm2m.h"
#include "dep.h"
#include "WppLogs.h"

#define TAG	"IWppConnection"

namespace wpp {

IWppConnection::IWppConnection() {}

IWppConnection::~IWppConnection() {
	clearPacketQueue();
}

/* ------------- Connection abilities ------------- */

bool IWppConnection::addPacketToQueue(Packet packet) {
	Packet tmpPkt = packet;

	WPP_LOGD_ARG(TAG, "Retrieved new packet from server: session->%d, length->%d", packet.session, packet.length);
	tmpPkt.buffer = new uint8_t[packet.length];
	memcpy(tmpPkt.buffer, packet.buffer, packet.length);

	if (!packets.push(&tmpPkt)) {
		WPP_LOGE_ARG(TAG, "Error ocured during add packet to queue: queue size -> %d, max queue size -> %d", getPacketQueueSize(), WPP_CONN_I_PACKETS_QUEUE_SIZE);
		delete [] tmpPkt.buffer;
		return false;
	}

	return true;
}

uint8_t IWppConnection::getPacketQueueSize() {
	return packets.size();
}

void IWppConnection::clearPacketQueue() {
	WPP_LOGD_ARG(TAG, "Clearing queue: queue size -> %d", getPacketQueueSize());
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			delete [] pkt->buffer;
		}
		packets.pop();
	}
}

bool IWppConnection::setDataBlockSize(uint16_t size) {
	WPP_LOGD_ARG(TAG, "New data block size -> %d", size);
	return true;// TODO: lwm2m_set_coap_block_size(size);
}

uint16_t IWppConnection::getDataBlockSize()  {
	return 0; // TODO: lwm2m_get_coap_block_size();
}

void IWppConnection::handlePacketsInQueue(lwm2m_context_t *context) {
	WPP_LOGD_ARG(TAG, "Handling packets in queue: packets count -> %d", getPacketQueueSize());
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			//lwm2m_handle_packet(context, pkt->buffer, pkt->length, pkt->session);
			delete [] pkt->buffer;
		}
		packets.pop();
	}
	WPP_LOGD(TAG, "Done");
}

extern "C" {
 	/*---------- Connection bindings ----------*/
    void * lwm2m_connect_server(uint16_t secObjInstID, void * userData) {
		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGD_ARG(TAG, "Connecting to server: security obj ID -> %d", secObjInstID);
		wpp::Security *security = client->registry().security().getInstance(secObjInstID);
		if (!security) {
			WPP_LOGE_ARG(TAG, "Security obj with ID -> %d not found", secObjInstID);
			return NULL;
		}

		wpp::IWppConnection::SESSION_T session = client->connection().connect(*security);
		WPP_LOGI_ARG(TAG, "Connected to server: security obj ID-> %d, session -> %d", secObjInstID, session);
		return session;
	}

	void lwm2m_close_connection(void * sessionH, void * userData) {
		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGI_ARG(TAG, "Close connection: session ID -> %d", sessionH);
		return client->connection().disconnect(sessionH);
	}

	uint8_t lwm2m_buffer_send(void * sessionH, uint8_t * buffer, size_t length, void * userData) {
		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGD_ARG(TAG, "Sending buffer to server: session -> %d, size -> %d", sessionH, length);
		return client->connection().sendPacket({sessionH, length, buffer})? COAP_NO_ERROR : COAP_500_INTERNAL_SERVER_ERROR;
	}

	bool lwm2m_session_is_equal(void * session1, void * session2, void * userData) {
		wpp::WppClient *client = (wpp::WppClient *)userData;
		return client->connection().sessionCmp(session1, session2);
	}
}

} /* namespace wpp */
