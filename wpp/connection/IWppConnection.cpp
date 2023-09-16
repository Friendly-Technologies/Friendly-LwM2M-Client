/*
 * IWppConnection.cpp
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#include "IWppConnection.h"
#include "WppClient.h"

//TODO: #include "liblwm2m.h"
#include "dep.h"

namespace wpp {

IWppConnection::IWppConnection() {}

IWppConnection::~IWppConnection() {
	clearPacketQueue();
}

/* ------------- Connection abilities ------------- */

bool IWppConnection::addPacketToQueue(Packet packet) {
	Packet tmpPkt = packet;

	tmpPkt.buffer = new uint8_t[packet.length];
	memcpy(tmpPkt.buffer, packet.buffer, packet.length);

	if (!packets.push(&tmpPkt)) {
		delete [] tmpPkt.buffer;
		return false;
	}

	return true;
}

uint8_t IWppConnection::getPacketQueueSize() {
	return packets.size();
}

void IWppConnection::clearPacketQueue() {
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			delete [] pkt->buffer;
		}
		packets.pop();
	}
}

bool IWppConnection::setDataBlockSize(uint16_t size) {
	return true;// TODO: lwm2m_set_coap_block_size(size);
}

uint16_t IWppConnection::getDataBlockSize()  {
	return 0; // TODO: lwm2m_get_coap_block_size();
}

void IWppConnection::handlePacketsInQueue(lwm2m_context_t *context) {
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			//lwm2m_handle_packet(context, pkt->buffer, pkt->length, pkt->session);
			delete [] pkt->buffer;
		}
		packets.pop();
	}
}

extern "C" {
// TODO: Investigate other approaches to share the connection() with wakaama core code
 /*---------- Connection bindings ----------*/
    void * lwm2m_connect_server(uint16_t secObjInstID, void * userData) {
		if (!wpp::WppClient::isCreated()) return NULL;

		wpp::Security *security = wpp::WppClient::client()->registry().security().getInstance(secObjInstID);
		if (!security) return NULL;

		return wpp::WppClient::client()->connection().connect(*security);
	}

	void lwm2m_close_connection(void * sessionH, void * userData) {
		if (!wpp::WppClient::isCreated()) return;
		return wpp::WppClient::client()->connection().disconnect(sessionH);
	}

	uint8_t lwm2m_buffer_send(void * sessionH, uint8_t * buffer, size_t length, void * userData) {
		if (!wpp::WppClient::isCreated()) return COAP_500_INTERNAL_SERVER_ERROR;
		return wpp::WppClient::client()->connection().sendPacket({sessionH, length, buffer})? COAP_NO_ERROR : COAP_500_INTERNAL_SERVER_ERROR;
	}

	bool lwm2m_session_is_equal(void * session1, void * session2, void * userData) {
		if (!wpp::WppClient::isCreated()) return false;
		return wpp::WppClient::client()->connection().sessionCmp(session1, session2);
	}
}

} /* namespace wpp */
