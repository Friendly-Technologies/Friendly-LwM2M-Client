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

bool WppConnectionI::receivedPacket(SESSION_T session, uint8_t *data, size_t length) {
	//lwm2m_handle_packet(WppCore::core()->getContext(), data, length, session);
	return true;
}

bool WppConnectionI::setDataBlockSize(uint16_t size) {
	return true;// TODO: lwm2m_set_coap_block_size(size);
}

uint16_t WppConnectionI::getDataBlockSize()  {
	return 0; // TODO: lwm2m_get_coap_block_size();
}

} /* namespace wpp */
