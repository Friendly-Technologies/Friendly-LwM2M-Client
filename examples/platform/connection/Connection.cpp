#include "Connection.h"

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

Connection::Connection(string port, int addressFamily): _port(port), _addressFamily(addressFamily) {
    _connFd = -1;
    _connections = NULL;
    openSocket();
}

Connection::~Connection() {}

Connection::SESSION_T Connection::connect(Security& security) {
    int s = -1;
    addrinfo hints, *servinfo = NULL, *p;
    sockaddr *sa;
    socklen_t sl;
    connection_t * connP = NULL;

    STRING_T uri;
    security.get(Security::SERVER_URI, uri);
    string host = uriToHost(uri);
    string port = uriToPort(uri);
    if (!host.length() || port.length()) return NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = _addressFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (0 != getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo) || servinfo == NULL) return NULL;

    // we test the various addresses
    for(s = -1, p = servinfo; p != NULL && s == -1 ; p = p->ai_next) {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0) {
            sa = p->ai_addr;
            sl = p->ai_addrlen;
            if (-1 == ::connect(s, p->ai_addr, p->ai_addrlen)) {
                close(s);
                s = -1;
            }
        }
    }
    if (s >= 0) {
        connP = createNewConn(sa, sl);
        close(s);
    }
    if (NULL != servinfo) {
        freeaddrinfo(servinfo);
    }

    return connP;
}

void Connection::disconnect(SESSION_T session) {
}

bool Connection::sessionCmp(SESSION_T session1, SESSION_T session2) {
    return true; //TODO: Implemnt
}

bool Connection::sendPacket(Packet packet) {
    return true; //TODO: Implemnt
}

bool Connection::openSocket() {
    if (_connFd != -1) return true;

    addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = _addressFamily;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (0 != getaddrinfo(NULL, _port.c_str(), &hints, &res)) {
        return false;
    }

    for(p = res ; p != NULL && _connFd == -1 ; p = p->ai_next) {
        _connFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_connFd >= 0) {
            if (-1 == bind(_connFd, p->ai_addr, p->ai_addrlen)) {
                close(_connFd);
                _connFd = -1;
            }
        }
    }

    freeaddrinfo(res);
    return _connFd != -1;
}

Connection::connection_t * Connection::createNewConn(struct sockaddr * addr, size_t addrLen) {
    connection_t * connP;

    connP = new connection_t;
    if (connP != NULL)
    {
        connP->sock = _connFd;
        memcpy(&(connP->addr), addr, addrLen);
        connP->addrLen = addrLen;
        connP->next = _connections;
    }

    return connP;
}

string Connection::uriToPort(string uri) {
    string portStart = ":";
    
    std::size_t start = uri.find_last_of(portStart);
    if (start != string::npos) return "";
    
    start++;
    if (start >= uri.length()) return "";
    
    return uri.substr(start);
}
    
string Connection::uriToHost(string uri) {
    string hostStart = "://";
    string hostEnd = ":";

    std::size_t start = uri.find(hostStart);
    if (start != string::npos) return "";
    start += hostStart.length();

    std::size_t end = uri.find(hostStart, start);
    if (end != string::npos) return "";

    // Maybe host name is empty
    if (start == end) return "";
    // remove brackets
    if (uri.at(start) == '[') {
        start++;
        if (uri.at(end-1) == ']') end--;
        else return "";
    }

    return uri.substr(start, end);
}