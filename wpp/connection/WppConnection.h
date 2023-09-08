/*
 * WppConnection.h
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#ifndef WPPCONNECTION_H_
#define WPPCONNECTION_H_

#include <string>
#include <functional>

#include "WppRegistry.h"
#include "WppCore.h"

namespace wpp {

class WppConnection {
public:
	using SESSION_T = void*;
	using CONNECT_T = std::function<SESSION_T(Security&)>;
	using DISCONNECT_T = std::function<void(SESSION_T)>;
	using SESSION_CMP_T = std::function<bool(SESSION_T, SESSION_T)>;
	using SEND_T = std::function<bool(SESSION_T, uint8_t*, size_t)>;

private:
	WppConnection(CONNECT_T &connectClb, DISCONNECT_T &disconnectClb, SESSION_CMP_T &sessionCmpClb, SEND_T &sendClb);

public:
	~WppConnection();

	/* ------------- WppConnection management ------------- */
	static bool create(CONNECT_T &connectClb, DISCONNECT_T &disconnectClb, SESSION_CMP_T &sessionCmpClb, SEND_T &sendClb);
	static bool isCreated();
	static WppConnection* instance();

	/* ------------- Connection abilities ------------- */
	SESSION_T connect(Security& security);
	void disconnect(SESSION_T session);
	bool isSessionCmp(SESSION_T session1, SESSION_T session2);

	bool receivedPacket(SESSION_T session, uint8_t *data, size_t length);
	bool sendPacket(SESSION_T session, uint8_t* data, size_t length);

	bool setDataBlockSize(uint16_t size);
	uint16_t getDataBlockSize();


private:
	static WppConnection *_manager;

	CONNECT_T _connectClb;
	DISCONNECT_T _disconnectClb;
	SESSION_CMP_T _sessionCmpClb;
	SEND_T _sendClb;
};

} /* namespace wpp */

#endif /* WPPCONNECTION_H_ */
