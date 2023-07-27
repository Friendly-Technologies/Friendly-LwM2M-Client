/*
 * WppConnection.cpp
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#include "WppConnection.h"

//TODO: #include "liblwm2m.h"

namespace wpp {

WppConnection *WppConnection::_manager = NULL;

WppConnection::WppConnection(CONNECT_T &connectClb, DISCONNECT_T &disconnectClb, SESSION_CMP_T &sessionCmpClb, SEND_T &sendClb):
	_connectClb(connectClb), _disconnectClb(disconnectClb), _sessionCmpClb(sessionCmpClb), _sendClb(sendClb) {}

WppConnection::~WppConnection() {}

/* ------------- WppConnection management ------------- */
bool WppConnection::create(CONNECT_T &connectClb, DISCONNECT_T &disconnectClb, SESSION_CMP_T &sessionCmpClb, SEND_T &sendClb) {
	if (!connectClb || !disconnectClb || !sessionCmpClb || !sendClb) return false;
	_manager = new WppConnection(connectClb, disconnectClb, sessionCmpClb, sendClb);
	return true;
}

bool WppConnection::isCreated() {
	return _manager != NULL;
}

WppConnection* WppConnection::instance() {
	return _manager;
}

/* ------------- Connection abilities ------------- */
WppConnection::SESSION_T WppConnection::connect(Security& security) {
	return _connectClb(security);
}

void WppConnection::disconnect(SESSION_T session) {
	_disconnectClb(session);
}

bool WppConnection::isSessionCmp(SESSION_T session1, SESSION_T session2) {
	return _sessionCmpClb(session1, session2);
}

bool WppConnection::receivedPacket(SESSION_T session, uint8_t *data, size_t length) {
	if (WppCore::isCreated()) return false;
	//lwm2m_handle_packet(WppCore::core()->getContext(), data, length, session);
	return true;
}

bool WppConnection::sendPacket(SESSION_T session, uint8_t* data, size_t length) {
	return _sendClb(session, data, length);
}

bool WppConnection::setDataBlockSize(uint16_t size) {
	return true;// TODO: lwm2m_set_coap_block_size(size);
}

uint16_t WppConnection::getDataBlockSize()  {
	return 0; // TODO: lwm2m_get_coap_block_size();
}

} /* namespace wpp */

extern "C" {
	void * lwm2m_connect_server(uint16_t secObjInstID, void * userData) {
		if (wpp::WppConnection::isCreated()) return NULL;

		wpp::Security *security = wpp::WppRegistry::instance().security().getInstance(secObjInstID);
		if (!security) return NULL;

		return wpp::WppConnection::instance()->connect(*security);
	}

	void lwm2m_close_connection(void * sessionH, void * userData) {
		if (wpp::WppConnection::isCreated()) return;
		return wpp::WppConnection::instance()->disconnect(sessionH);
	}

	uint8_t lwm2m_buffer_send(void * sessionH, uint8_t * buffer, size_t length, void * userData) {
		if (wpp::WppConnection::isCreated()) return COAP_500_INTERNAL_SERVER_ERROR;
		return wpp::WppConnection::instance()->sendPacket(sessionH, buffer, length)? COAP_NO_ERROR : COAP_500_INTERNAL_SERVER_ERROR;
	}

	bool lwm2m_session_is_equal(void * session1, void * session2, void * userData) {
		if (wpp::WppConnection::isCreated()) return false;
		return wpp::WppConnection::instance()->isSessionCmp(session1, session2);
	}
}
