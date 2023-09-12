/*
 * WppConnectionI.cpp
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#include "WppConnectionI.h"

//TODO: #include "liblwm2m.h"

namespace wpp {

/* ------------- Connection abilities ------------- */

bool WppConnectionI::addPacketToQueue(Packet packet) {
	return packets.push(&packet);
}

uint8_t WppConnectionI::getPacketQueueSize() {
	return packets.size();
}

bool WppConnectionI::setDataBlockSize(uint16_t size) {
	return true;// TODO: lwm2m_set_coap_block_size(size);
}

uint16_t WppConnectionI::getDataBlockSize()  {
	return 0; // TODO: lwm2m_get_coap_block_size();
}

void WppConnectionI::handlePacketsInQueue(lwm2m_context_t *context) {
	for (size_t i = 0; i < packets.size(); i++) {
		Packet *pkt = packets.at(i);
		//lwm2m_handle_packet(context, pkt->buffer, pkt->length, pkt->session);
		packets.pop();
	}
}

} /* namespace wpp */
