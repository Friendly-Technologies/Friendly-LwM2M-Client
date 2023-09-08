#ifndef CONNECTION_H
#define CONNECTION_H

#include "WppConnectionI.h"

class Connection : public wpp::WppConnectionI {
public:
    Connection() {}
    ~Connection() {}

    SESSION_T connect(wpp::Security& security) override;
	void disconnect(SESSION_T session) override;
	bool sessionCmp(SESSION_T session1, SESSION_T session2) override;
	bool sendPacket(SESSION_T session, uint8_t* data, size_t length) override;
};

#endif //CONNECTION_H