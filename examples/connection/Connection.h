#ifndef CONNECTION_H
#define CONNECTION_H

#include "IWppConnection.h"

class Connection : public wpp::IWppConnection {
public:
    Connection() {}
    ~Connection() {}

    SESSION_T connect(wpp::Security& security) override;
	void disconnect(SESSION_T session) override;
	bool sessionCmp(SESSION_T session1, SESSION_T session2) override;
	bool sendPacket(Packet packet) override;
};

#endif //CONNECTION_H