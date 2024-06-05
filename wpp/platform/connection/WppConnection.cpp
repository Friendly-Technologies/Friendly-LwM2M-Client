/*
 * WppConnection.cpp
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#include "WppConnection.h"

#include "WppClient.h"
#include "WppRegistry.h"
#include "WppLogs.h"

namespace wpp {

WppConnection::WppConnection() {}

WppConnection::~WppConnection() {
	clearPacketQueue();
}

/* ------------- Connection abilities ------------- */

bool WppConnection::addPacketToQueue(const Packet &packet) {
	Packet tmpPkt = packet;

	WPP_LOGD(TAG_WPP_CONN, "Retrieved new packet from server: session-> %p, length->%d", packet.session, packet.length);
	tmpPkt.buffer = new uint8_t[packet.length];
	memcpy(tmpPkt.buffer, packet.buffer, packet.length);

	if (!packets.push(&tmpPkt)) {
		WPP_LOGE(TAG_WPP_CONN, "Error ocured during add packet to queue: queue size -> %d, max queue size -> %d", getPacketQueueSize(), WPP_CONN_I_PACKETS_QUEUE_SIZE);
		delete [] tmpPkt.buffer;
		return false;
	}

	return true;
}

uint8_t WppConnection::getPacketQueueSize() {
	return packets.size();
}

void WppConnection::clearPacketQueue() {
	WPP_LOGD(TAG_WPP_CONN, "Clearing queue: queue size -> %d", getPacketQueueSize());
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			delete [] pkt->buffer;
		}
		packets.pop();
	}
}

bool WppConnection::setDataBlockSize(uint16_t size) {
	WPP_LOGD(TAG_WPP_CONN, "New data block size -> %d", size);
	return lwm2m_set_coap_block_size(size);
}

uint16_t WppConnection::getDataBlockSize()  {
	return lwm2m_get_coap_block_size();
}

void WppConnection::handlePacketsInQueue(WppClient &client) {
	WPP_LOGD(TAG_WPP_CONN, "Handling packets in queue: packets count -> %d", getPacketQueueSize());
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			lwm2m_handle_packet(&client.getContext(), pkt->buffer, pkt->length, pkt->session);
			delete [] pkt->buffer;
		}
		packets.pop();
	}
	WPP_LOGD(TAG_WPP_CONN, "Done");
}

extern "C" {
 	/*---------- Connection bindings ----------*/
    void * lwm2m_connect_server(uint16_t secObjInstID, void * userData) {
		if (!userData) {
			WPP_LOGE(TAG_WPP_CONN, "User data is NULL");
			return NULL;
		}

		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGD(TAG_WPP_CONN, "Connecting to server: security obj ID -> %d", secObjInstID);
		wpp::Lwm2mSecurity *security = Lwm2mSecurity::instance(*client, secObjInstID);
		if (!security) {
			WPP_LOGE(TAG_WPP_CONN, "Lwm2mSecurity obj with ID -> %d not found", secObjInstID);
			return NULL;
		}

		wpp::WppConnection::SESSION_T session = client->connection().connect(*security);
		if (!session) {
			WPP_LOGE(TAG_WPP_CONN, "Not posible connect to server: security obj ID-> %d, session -> %p", secObjInstID, session);
		} else {
			WPP_LOGI(TAG_WPP_CONN, "Connected to server: security obj ID-> %d, session -> %p", secObjInstID, session);
		}

		return session;
	}

	void lwm2m_close_connection(void * sessionH, void * userData) {
		if (!userData) {
			WPP_LOGE(TAG_WPP_CONN, "User data is NULL");
			return;
		}

		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGI(TAG_WPP_CONN, "Close connection: session -> %p", sessionH);
		client->connection().disconnect(sessionH);
	}

	uint8_t lwm2m_buffer_send(void * sessionH, uint8_t * buffer, size_t length, void * userData) {
		if (!userData) {
			WPP_LOGE(TAG_WPP_CONN, "User data is NULL");
			return COAP_500_INTERNAL_SERVER_ERROR;
		}

		wpp::WppClient *client = (wpp::WppClient *)userData;
		WPP_LOGD(TAG_WPP_CONN, "Sending buffer to server: session -> %p, size -> %d", sessionH, length);
		bool result = client->connection().sendPacket({sessionH, length, buffer});
		WPP_LOGD(TAG_WPP_CONN, "Sending buffer to server: session -> %p, result -> %d", sessionH, result);
		return result? COAP_NO_ERROR : COAP_500_INTERNAL_SERVER_ERROR;
	}

	bool lwm2m_session_is_equal(void * session1, void * session2, void * userData) {
		if (!userData) {
			WPP_LOGE(TAG_WPP_CONN, "User data is NULL");
			return false;
		}

		wpp::WppClient *client = (wpp::WppClient *)userData;
		return client->connection().sessionCmp(session1, session2);
	}
}

} /* namespace wpp */
