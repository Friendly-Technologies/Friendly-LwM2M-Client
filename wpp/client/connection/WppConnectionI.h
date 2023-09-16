/*
 * WppConnectionI.h
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#ifndef WppConnectionI_H_
#define WppConnectionI_H_

#include <string>

#include "WppRegistry.h"

namespace wpp {

class WppConnectionI {
public:
	using SESSION_T = void*;

public:
	WppConnectionI() {}
	virtual ~WppConnectionI() {}

	WppConnectionI(const WppConnectionI&) = delete;
	WppConnectionI(WppConnectionI&&) = delete;
	WppConnectionI& operator=(const WppConnectionI&) = delete;
	WppConnectionI& operator=(WppConnectionI&&) = delete;

	/* ------------- Connection abilities ------------- */
	virtual SESSION_T connect(Security& security) = 0;
	virtual void disconnect(SESSION_T session) = 0;
	virtual bool sessionCmp(SESSION_T session1, SESSION_T session2) = 0;
	virtual bool sendPacket(SESSION_T session, uint8_t* data, size_t length) = 0;

	bool receivedPacket(SESSION_T session, uint8_t *data, size_t length);
	bool setDataBlockSize(uint16_t size);
	uint16_t getDataBlockSize();
};

} /* namespace wpp */

#endif /* WppConnectionI_H_ */
