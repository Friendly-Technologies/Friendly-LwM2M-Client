#include "Connection.h"

#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

/* --------------- General DTLS handler callbacks--------------- */

#if DTLS_WITH_PSK
/* This function is the "key store" for tinyDTLS. It is called to
 * retrieve a key for the given identity within this particular
 * session. */
int get_psk_info(dtls_context_t *ctx, const session_t *session, dtls_credentials_type_t type, const unsigned char *id, size_t id_len, unsigned char *result, size_t result_length) {
    Connection *appContext = (Connection *)ctx->app;
    // find connection
    Connection::dtls_connection_t* conn = appContext->connFind(&(session->addr.st),session->size);
    if (!conn) {
        cout << "Connection: GET PSK session not found" << endl;
        return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
    }

    switch (type) {
        case DTLS_PSK_IDENTITY: {
            const OPAQUE_T &key = appContext->getPublicKey(conn);
            if (result_length < key.size()) {
                cout << "Connection: cannot set psk_identity -- buffer too small result_length:" << result_length << ", key size: " << key.size() << endl;
                return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
            }

            memcpy(result, key.data(), key.size());
            return key.size();
        }
        case DTLS_PSK_KEY: {
            const OPAQUE_T &key = appContext->getSecretKey(conn);

            if (result_length < key.size()) {
                cout << "Connection: cannot set psk -- buffer too small result_length:" << result_length << ", key size: " << key.size() << endl;
                return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
            }

            memcpy(result, key.data(), key.size());
            return key.size();
        }
        case DTLS_PSK_HINT: {
            // PSK_HINT is optional and can be empty.
            return 0;
        }
        default:
            cout << "Connection: unsupported request type: " << type << endl;
    }

    return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
}
#elif DTLS_WITH_RPK
int get_ecdsa_key(struct dtls_context_t *ctx, const session_t *session, const dtls_ecdsa_key_t **result) {
    cout << "Connection: get_ecdsa_key()" << endl;

    Connection *appContext = (Connection *)ctx->app;
    // find connection
    Connection::dtls_connection_t* conn = appContext->connFind(&(session->addr.st),session->size);
    if (!conn) {
        cout << "Connection: GET RPK session not found" << endl;
        return dtls_alert_fatal_create(DTLS_ALERT_INTERNAL_ERROR);
    }

    *result = &appContext->getEcdsaKey(conn);
    return 0;
}

int verify_ecdsa_key(struct dtls_context_t *ctx, const session_t *session, const unsigned char *other_pub_x, const unsigned char *other_pub_y, size_t key_size) {
    cout << "Connection: verify_ecdsa_key()" << endl;
    (void)ctx;
    (void)session;
    (void)other_pub_x;
    (void)other_pub_y;
    (void)key_size;
    return 0;
}
#endif

/* The callback function must return the number of bytes
 * that were sent, or a value less than zero to indicate an
 * error. */
int send_to_peer(struct dtls_context_t *ctx, session_t *session, uint8 *data, size_t len) {
     Connection *appContext = (Connection *)ctx->app;
    // find connection
    Connection::dtls_connection_t* conn = appContext->connFind(&(session->addr.st),session->size);
    if (conn) {
        // send data to peer
        int res = appContext->sendData({conn, len, data});
        if (res < 0) return -1;
        return res;
    }
    return -1;
}

int read_from_peer(dtls_context_t *ctx, session_t *session, uint8 *data, size_t len) {
    Connection *appContext = (Connection *)ctx->app;
    // find connection
    Connection::dtls_connection_t* conn = appContext->connFind(&(session->addr.st), session->size);
    if (conn) {
        cout << "Connection: read data len: " << len << ", session: " << session << endl;
        appContext->addPacketToQueue({conn, (size_t)len, data});
        return 0;
    }
    return -1;
}

static dtls_handler_t _dtlsClb = {
    .write = send_to_peer,
    .read  = read_from_peer,
    .event = NULL,
    #if DTLS_WITH_PSK
    .get_psk_info = get_psk_info,
    #elif DTLS_WITH_RPK
    .get_ecdsa_key = get_ecdsa_key,
    .verify_ecdsa_key = verify_ecdsa_key
    #endif
};

/* --------------- Connection implementation--------------- */

Connection::Connection(string port, int addressFamily): _port(port), _addressFamily(addressFamily) {
    _connFd = -1;
    _connections = NULL;
    // Opening socket
    openSocket();
    cout << "Connection: socket id " << _connFd << endl;
    dtls_init();
    // Create a DTLS context for the connection
    _dtlsContext = dtls_new_context(this);
    if (_dtlsContext == NULL) cout << "Failed to create the DTLS context" << endl;
    dtls_set_handler(_dtlsContext, &_dtlsClb);
}

Connection::~Connection() {}

Connection::SESSION_T Connection::connect(Lwm2mSecurity& security) {
    addrinfo hints, *servinfo = NULL, *p;
    int s;
    sockaddr *sa;
    socklen_t sl;
    dtls_connection_t * conn = NULL;

    STRING_T uri = security.get<STRING_T>(Lwm2mSecurity::LWM2M_SERVER_URI_0);
    string host = uriToHost(uri);
    string port = uriToPort(uri);
    cout << "Connection: connect to host " << host << ", host len: " << strlen(host.c_str()) << ", port " << port << ", port len: " << strlen(port.c_str()) << endl;
    cout << "Connection: connect to uri " << uri << ", uri len: " << strlen(uri.c_str()) << endl;
    if (!host.length() || !port.length()) return NULL;
   
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = _addressFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo) || !servinfo) return NULL;

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
        conn = createNewConn(sa, sl);
        close(s);
        if (conn == NULL) {
            if (NULL != servinfo) free(servinfo);
            return conn;
        }
        
        setupSecurityKeys(security, conn);

        INT_T mode = security.get<INT_T>(Lwm2mSecurity::SECURITY_MODE_2);
        if (mode != LWM2M_SECURITY_MODE_NONE) {
            conn->dtlsContext = _dtlsContext;
        } else if (conn->dtlsSession) {
            delete conn->dtlsSession;
            conn->dtlsSession = NULL;
        }
    }

    if (NULL != servinfo) free(servinfo);

    return conn;
}

void Connection::disconnect(SESSION_T session) {
    dtls_connection_t * conn = (dtls_connection_t *)session;

    if (conn == _connections) {
        _connections = conn->next;
        if (conn->dtlsSession) delete conn->dtlsSession;
        #if DTLS_WITH_RPK
        if (conn->ecdsa_key.priv_key) delete conn->ecdsa_key.priv_key;
        if (conn->ecdsa_key.pub_key_x) delete conn->ecdsa_key.pub_key_x;
        if (conn->ecdsa_key.pub_key_y) delete conn->ecdsa_key.pub_key_y;
        #endif
        delete conn;
    } else {
        dtls_connection_t * parent = _connections;
        while (parent != NULL && parent->next != conn) parent = parent->next;
        if (parent != NULL) {
            parent->next = conn->next;
            if (conn->dtlsSession) delete conn->dtlsSession;
            #if DTLS_WITH_RPK
            if (conn->ecdsa_key.priv_key) delete conn->ecdsa_key.priv_key;
            if (conn->ecdsa_key.pub_key_x) delete conn->ecdsa_key.pub_key_x;
            if (conn->ecdsa_key.pub_key_y) delete conn->ecdsa_key.pub_key_y;
            #endif
            delete conn;
        }
    }
}

bool Connection::sessionCmp(SESSION_T session1, SESSION_T session2) {
    return (session1 == session2);
}

bool Connection::sendPacket(const Packet &packet) {
    dtls_connection_t * conn = (dtls_connection_t*)packet.session;

    if (!conn) {
        cout << "Connection: failed sending " << packet.length << " bytes, missing connection" << endl;
        return false ;
    }

    if (conn->dtlsSession == NULL) {
        // no security
        if (sendData(packet) ) return false;
    } else {
        if (DTLS_NAT_TIMEOUT > 0 && (time(NULL) - conn->lastSend) > DTLS_NAT_TIMEOUT) {
            cout << "Connection: can't send due to NAT timeout" << endl;
            cout << "Connection: rehandshake" << endl;
            // we need to rehandhake because our source IP/port probably changed for the server
            if (!rehandshake(conn, false)) {
                cout << "Connection: can't send due to rehandshake error" << endl;
                return false;
            }
        }
        if (dtls_write(conn->dtlsContext, conn->dtlsSession, packet.buffer, packet.length) == -1) return false;
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
    cout << "Connection: readDataSize: " << readDataSize << endl;

    dtls_connection_t * conn = _connections;
    while (conn != NULL) {
        if ((conn->addrLen == addrLen) && (memcmp(&(conn->addr), &addr, addrLen) == 0)) break;
        conn = conn->next;
    }

    cout << "Connection: conn: " << conn << endl;
    if (!conn) {
        cout << "Connection: error conn not found " << endl;
        return;
    }

    if (conn->dtlsSession) {
        int result = dtls_handle_message(conn->dtlsContext, conn->dtlsSession, buffer, readDataSize);
        if (result != 0) cout << "Connection: error dtls handling message " << result << endl;
    }
    else {
        addPacketToQueue({conn, (size_t)readDataSize, buffer});
    }
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

Connection::dtls_connection_t * Connection::createNewConn(sockaddr * addr, size_t addrLen) {
    dtls_connection_t * conn;

    conn = new dtls_connection_t;
    if (conn) {
        memset(conn, 0, sizeof(dtls_connection_t));
        conn->sock = _connFd;
        memcpy(&(conn->addr), addr, addrLen);
        conn->addrLen = addrLen;
        conn->next = _connections;

        conn->dtlsSession = new session_t;
        memset(conn->dtlsSession, 0, sizeof(session_t));
        conn->dtlsSession->addr.sin6 = conn->addr;
        conn->dtlsSession->size = conn->addrLen;
        conn->lastSend = time(NULL);

        #if DTLS_WITH_RPK
        conn->ecdsa_key.priv_key = NULL;
        conn->ecdsa_key.pub_key_x = NULL;
        conn->ecdsa_key.pub_key_y = NULL;
        #endif

        // Update connection list head
        _connections = conn;
    }

    return conn;
}

int Connection::sendData(const Packet &packet) {
    int nbSent;
    size_t offset = 0;
    dtls_connection_t * conn = (dtls_connection_t*)packet.session;

    while (offset != packet.length) {
        nbSent = sendto(conn->sock, packet.buffer + offset, packet.length - offset, 0, (sockaddr *)&(conn->addr), conn->addrLen);
        if (nbSent == -1) return -1;
        offset += nbSent;
    }
    conn->lastSend = time(NULL);

    return offset;
}

bool Connection::rehandshake(dtls_connection_t *conn, bool sendCloseNotify) {
    // if not a dtls connection we do nothing
    if (conn->dtlsSession == NULL)  return 0;

    // reset current session
    dtls_peer_t * peer = dtls_get_peer(conn->dtlsContext, conn->dtlsSession);
    if (peer != NULL) {
        if (!sendCloseNotify) peer->state =  DTLS_STATE_CLOSED;
        dtls_reset_peer(conn->dtlsContext, peer);
    }

    // start a fresh handshake
    if (dtls_connect(conn->dtlsContext, conn->dtlsSession)) {
        cout << "Connection: error dtls reconnection" << endl;
        return false;
    }

    return true;
}

Connection::dtls_connection_t * Connection::connFind(const sockaddr_storage * addr, size_t addrLen) {
    dtls_connection_t * conn = _connections;
    while (conn != NULL) {
        if (sockAddrCmp((sockaddr*)(&conn->addr), (sockaddr*)addr)) return conn;
        conn = conn->next;
    }

    return conn;
}

bool Connection::sockAddrCmp(sockaddr *x, sockaddr *y) {
    int portX = getPort(x);
    int portY = getPort(y);

    // if the port is invalid or different
    if (portX == -1 || portX != portY) return 0;

    // IPV4?
    if (x->sa_family == AF_INET) {
        // is V4?
        if (y->sa_family == AF_INET) {
            // compare V4 with V4
            return ((sockaddr_in *)x)->sin_addr.s_addr == ((sockaddr_in *)y)->sin_addr.s_addr;
            // is V6 mapped V4?
        } else if (IN6_IS_ADDR_V4MAPPED(&((sockaddr_in6 *)y)->sin6_addr)) {
            in6_addr* addr6 = &((sockaddr_in6 *)y)->sin6_addr;
            uint32_t y6to4 = addr6->s6_addr[15] << 24 | addr6->s6_addr[14] << 16 | addr6->s6_addr[13] << 8 | addr6->s6_addr[12];
            return y6to4 == ((sockaddr_in *)x)->sin_addr.s_addr;
        } else {
            return 0;
        }
    } else if (x->sa_family == AF_INET6 && y->sa_family == AF_INET6) {
        // IPV6 with IPV6 compare
        return memcmp(((sockaddr_in6 *)x)->sin6_addr.s6_addr, ((sockaddr_in6 *)y)->sin6_addr.s6_addr, 16) == 0;
    } else {
        // unknown address type
        cout << "Connection: non IPV4 or IPV6 address" << endl;
        return 0;
    }
}

int Connection::getPort(sockaddr *x) {
    if (x->sa_family == AF_INET) return ((sockaddr_in *)x)->sin_port;
    else if (x->sa_family == AF_INET6) return ((sockaddr_in6 *)x)->sin6_port;
    else {
        cout << "Connection: non IPV4 or IPV6 address" << endl;
        return  -1;
    }
}

#if DTLS_WITH_PSK
const OPAQUE_T & Connection::getPublicKey(dtls_connection_t *conn) {
    return conn->pubKey;
}

const OPAQUE_T &  Connection::getSecretKey(dtls_connection_t *conn) {
    return conn->privKey;
}
#elif DTLS_WITH_RPK
    const dtls_ecdsa_key_t & Connection::getEcdsaKey(dtls_connection_t *conn) {
        return conn->ecdsa_key;
    }
#endif

bool Connection::setupSecurityKeys(Lwm2mSecurity& security, dtls_connection_t *conn) {
    #if DTLS_WITH_PSK
    conn->pubKey = security.get<OPAQUE_T>(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3);
    conn->privKey = security.get<OPAQUE_T>(Lwm2mSecurity::SECRET_KEY_5);
    #elif DTLS_WITH_RPK
    OPAQUE_T pubKey = security.get<OPAQUE_T>(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3);
    OPAQUE_T privKey = security.get<OPAQUE_T>(Lwm2mSecurity::SECRET_KEY_5);

    if (privKey.size() != DTLS_EC_KEY_SIZE) {
        cout << "Connection: private key size is incorrect and equals: " << privKey.size() << endl;
        return false;
    } 
    // prefix(1 byte) + x coordinate (32 bytes) + y coordinate (32 bytes)
    if (pubKey.size() != (1+DTLS_EC_KEY_SIZE*2)) {
        cout << "Connection: public key size is incorrect and equals: " << pubKey.size() << endl;
        return false;
    }
    
    conn->ecdsa_key.curve = DTLS_ECDH_CURVE_SECP256R1;

    uint8_t *priv = new uint8_t[DTLS_EC_KEY_SIZE];
    memcpy(priv, privKey.data(), DTLS_EC_KEY_SIZE);
    conn->ecdsa_key.priv_key = priv;
    
    uint8_t *pub_y = new uint8_t[DTLS_EC_KEY_SIZE];
    memcpy(pub_y, pubKey.data()+1, DTLS_EC_KEY_SIZE);
    conn->ecdsa_key.pub_key_x = pub_y;

    uint8_t *pub_x = new uint8_t[DTLS_EC_KEY_SIZE];
    memcpy(pub_x, pubKey.data()+1+DTLS_EC_KEY_SIZE, DTLS_EC_KEY_SIZE);
    conn->ecdsa_key.pub_key_y = pub_x;
    #endif

    return true;
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