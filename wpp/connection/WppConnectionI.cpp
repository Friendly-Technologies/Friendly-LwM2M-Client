/*
 * WppConnectionI.cpp
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#include "WppConnectionI.h"

//TODO: #include "liblwm2m.h"

namespace wpp {

WppConnectionI::WppConnectionI() {}

WppConnectionI::~WppConnectionI() {
	clearPacketQueue();
}

/* ------------- Connection abilities ------------- */

bool WppConnectionI::addPacketToQueue(Packet packet) {
	Packet tmpPkt = packet;

	tmpPkt.buffer = new uint8_t[packet.length];
	memcpy(tmpPkt.buffer, packet.buffer, packet.length);

	if (!packets.push(&tmpPkt)) {
		delete [] tmpPkt.buffer;
		return false;
	}

	return true;
}

uint8_t WppConnectionI::getPacketQueueSize() {
	return packets.size();
}

void WppConnectionI::clearPacketQueue() {
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			delete [] pkt->buffer;
		}
		packets.pop();
	}
}

bool WppConnectionI::setDataBlockSize(uint16_t size) {
	return true;// TODO: lwm2m_set_coap_block_size(size);
}

uint16_t WppConnectionI::getDataBlockSize()  {
	return 0; // TODO: lwm2m_get_coap_block_size();
}

void WppConnectionI::handlePacketsInQueue(lwm2m_context_t *context) {
	while (packets.size()) {
		Packet *pkt = packets.front();
		if (pkt && pkt->buffer) {
			//lwm2m_handle_packet(context, pkt->buffer, pkt->length, pkt->session);
			delete [] pkt->buffer;
		}
		packets.pop();
	}
}

} /* namespace wpp */
