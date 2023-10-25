#ifndef CONNECTION_H
#define CONNECTION_H

#include <netinet/in.h>
#include "WppConnection.h"
#include "WppRegistry.h"

using namespace std;
using namespace wpp;

class Connection : public WppConnection {
private:
    struct connection_t {
        struct connection_t *  next;
        int                     sock;
        struct sockaddr_in6     addr;
        size_t                  addrLen;
    };

public:
    Connection(string port, int addressFamily);
    ~Connection();

    SESSION_T connect(Lwm2mSecurity& security) override;
	void disconnect(SESSION_T session) override;
	bool sessionCmp(SESSION_T session1, SESSION_T session2) override;
	bool sendPacket(const Packet &packet) override;

    void loop();

private: 
    bool openSocket();
    connection_t * createNewConn(struct sockaddr * addr, size_t addrLen);
    string uriToPort(string uri);
    string uriToHost(string uri);

private:
    string _port;
    int _addressFamily;
    int _connFd;

    connection_t * _connections;
};

#endif //CONNECTION_H