#include "Connection.h"

#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

Connection::Connection(string port, int addressFamily): _port(port), _addressFamily(addressFamily) {
    _connFd = -1;
    _connections = NULL;
    openSocket();
    cout << "Connection: socket id " << _connFd << endl;
}

Connection::~Connection() {}

Connection::SESSION_T Connection::connect(Lwm2mSecurity& security) {
    int s = -1;
    addrinfo hints, *servinfo = NULL, *p;
    sockaddr *sa;
    socklen_t sl;
    connection_t * connP = NULL;

    STRING_T uri = security.get<STRING_T>(Lwm2mSecurity::LWM2M_SERVER_URI_0);
    string host = uriToHost(uri);
    string port = uriToPort(uri);
    cout << "Connection: connect to host " << host << ", host len: " << strlen(host.c_str()) << ", port " << port << ", port len: " << strlen(port.c_str()) << endl;
    cout << "Connection: connect to uri " << uri << ", uri len: " << strlen(uri.c_str()) << endl;
    if (!host.length() || !port.length()) return NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = _addressFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo) || servinfo == NULL) return NULL;

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
    connection_t * conn = (connection_t *)session;

    if (conn == _connections) {
        _connections = conn->next;
        delete conn;
    } else {
        connection_t * parent;

        parent = _connections;
        while (parent != NULL && parent->next != conn) parent = parent->next;
        if (parent != NULL) {
            parent->next = conn->next;
            delete conn;
        }
    }
}

bool Connection::sessionCmp(SESSION_T session1, SESSION_T session2) {
    return (session1 == session2);
}

bool Connection::sendPacket(const Packet &packet) {
    int nbSent;
    size_t offset;
    connection_t * conn = (connection_t *)packet.session;

    offset = 0;
    while (offset != packet.length)
    {
        nbSent = sendto(_connFd, packet.buffer + offset, packet.length - offset, 0, (sockaddr *)&(conn->addr), conn->addrLen);
        if (nbSent == -1) return false;
        offset += nbSent;
    }
    return true;
}

void Connection::loop() {
    uint8_t buffer[1024];
    int16_t readDataSize = 0;
    sockaddr_storage addr;
    socklen_t addrLen = sizeof(sockaddr_storage);

    readDataSize = recvfrom(_connFd, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&addr, &addrLen);
    if (readDataSize == -1) return;
    cout << "Connection readDataSize: " << readDataSize << endl;

    connection_t * session = _connections;
    while (session != NULL) {
        if ((session->addrLen == addrLen) && (memcmp(&(session->addr), &addr, addrLen) == 0)) break;
        session = session->next;
    }

    cout << "Connection session: " << session << endl;
    if (session) addPacketToQueue({session, (size_t)readDataSize, buffer});
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
            // Set to non-blocking mode
            int flags = fcntl(_connFd, F_GETFL, 0);
            if (fcntl(_connFd, F_SETFL, flags | O_NONBLOCK) == -1 ||
                bind(_connFd, p->ai_addr, p->ai_addrlen) == -1) {
                close(_connFd);
                _connFd = -1;
            }
        }
    }

    freeaddrinfo(res);
    return _connFd != -1;
}

Connection::connection_t * Connection::createNewConn(struct sockaddr * addr, size_t addrLen) {
    connection_t * connection;

    connection = new connection_t;
    if (connection != NULL)
    {
        connection->sock = _connFd;
        memcpy(&(connection->addr), addr, addrLen);
        connection->addrLen = addrLen;
        connection->next = _connections;
        // Update connection list head
        _connections = connection;
    }

    return connection;
}

string Connection::uriToPort(string uri) {
    string portStart = ":";

    std::size_t start = uri.find_last_of(portStart);
    if (start == string::npos) return "";

    start++;
    if (start >= uri.length()) return "";

    string port = uri.substr(start);
    port.erase(std::remove_if(port.begin(), port.end(), static_cast<int(*)(int)>(std::isspace)), port.end());
    return port;
}
    
string Connection::uriToHost(string uri) {
    string hostStart = "://";
    string hostEnd = ":";

    std::size_t start = uri.find(hostStart);
    if (start == string::npos) return "";
    start += hostStart.length();

    std::size_t end = uri.find(hostEnd, start);
    if (end == string::npos) return "";

    // Maybe host name is empty
    if (start == end) return "";
    // remove brackets
    if (uri.at(start) == '[') {
        start++;
        if (uri.at(end-1) == ']') end--;
        else return "";
    }

    return uri.substr(start, end-start);
}