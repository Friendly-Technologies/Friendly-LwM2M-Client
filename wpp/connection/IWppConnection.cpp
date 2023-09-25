/*
 * IWppConnection.cpp
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#include "IWppConnection.h"

#include "WppClient.h"
#include "WppRegistry.h"
#include "WppLogs.h"

namespace wpp {

IWppConnection::IWppConnection() {}

IWppConnection::~IWppConnection() {
	clearPacketQueue();
}

/* ------------- Connection abilities ------------- */

bool IWppConnection::addPacketToQueue(const Packet &packet) {
	Packet tmpPkt = packet;

	WPP_LOGD_ARG(TAG_WPP_CONN, "Retrieved new packet from server: session->0x%x, length->%d", packet.session, packet.length);
	tmpPkt.buffer = new uint8_t[packet.length];
	memcpy(tmpPkt.buffer, packet.buffer, packet.length);

	if (!packets.push(&tmpPkt)) {
		WPP_LOGE_ARG(TAG_WPP_CONN, "Error ocured during add packet to queue: queue size -> %d, max queue size -> %d", getPacketQueueSize(), WPP_CONN_I_PACKETS_QUEUE_SIZE);
		delete [] tmpPkt.buffer;
		return false;
	}

	return true;
}

uint8_t IWppConnection::getPacketQueueSize() {
	return packets.size();
}

void IWppConnection::clearPacketQueue() {
	WPP_LOGD_ARG(TAG_WPP_CONN, "Clearing queue: queue size -> %d", getPacketQueueSize());
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			delete [] pkt->buffer;
		}
		packets.pop();
	}
}

bool IWppConnection::setDataBlockSize(uint16_t size) {
	WPP_LOGD_ARG(TAG_WPP_CONN, "New data block size -> %d", size);
	return lwm2m_set_coap_block_size(size);
}

uint16_t IWppConnection::getDataBlockSize()  {
	return lwm2m_get_coap_block_size();
}

void IWppConnection::handlePacketsInQueue(lwm2m_context_t *context) {
	WPP_LOGD_ARG(TAG_WPP_CONN, "Handling packets in queue: packets count -> %d", getPacketQueueSize());
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			lwm2m_handle_packet(context, pkt->buffer, pkt->length, pkt->session);
			delete [] pkt->buffer;
		}
		packets.pop();
	}
	WPP_LOGD(TAG_WPP_CONN, "Done");
}

extern "C" {
 	/*---------- Connection bindings ----------*/
    void * lwm2m_connect_server(uint16_t secObjInstID, void * userData) {
		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGD_ARG(TAG_WPP_CONN, "Connecting to server: security obj ID -> %d", secObjInstID);
		wpp::Security *security = client->registry().security().instance(secObjInstID);
		if (!security) {
			WPP_LOGE_ARG(TAG_WPP_CONN, "Security obj with ID -> %d not found", secObjInstID);
			return NULL;
		}

		wpp::IWppConnection::SESSION_T session = client->connection().connect(*security);
		WPP_LOGI_ARG(TAG_WPP_CONN, "Connected to server: security obj ID-> %d, session -> 0x%x", secObjInstID, session);
		return session;
	}

	void lwm2m_close_connection(void * sessionH, void * userData) {
		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGI_ARG(TAG_WPP_CONN, "Close connection: session ID -> 0x%x", sessionH);
		return client->connection().disconnect(sessionH);
	}

	uint8_t lwm2m_buffer_send(void * sessionH, uint8_t * buffer, size_t length, void * userData) {
		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGD_ARG(TAG_WPP_CONN, "Sending buffer to server: session -> 0x%x, size -> %d", sessionH, length);
		return client->connection().sendPacket({sessionH, length, buffer})? COAP_NO_ERROR : COAP_500_INTERNAL_SERVER_ERROR;
	}

	bool lwm2m_session_is_equal(void * session1, void * session2, void * userData) {
		wpp::WppClient *client = (wpp::WppClient *)userData;
		return client->connection().sessionCmp(session1, session2);
	}
}

} /* namespace wpp */
