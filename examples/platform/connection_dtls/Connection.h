#ifndef CONNECTION_H
#define CONNECTION_H

#include <netinet/in.h>
#include "WppConnection.h"
#include "WppRegistry.h"

extern "C" {
#include "tinydtls.h"
#include "dtls.h"
// min defined as macro in numeric.h that conflicts with std::min
#undef min
}

using namespace std;
using namespace wpp;

// after 40sec of inactivity we rehandshake
#define DTLS_NAT_TIMEOUT 40

class Connection : public WppConnection {
    friend int get_psk_info(struct dtls_context_t *ctx, const session_t *session, dtls_credentials_type_t type, const unsigned char *id, size_t id_len, unsigned char *result, size_t result_length);
    friend int get_ecdsa_key(struct dtls_context_t *ctx, const session_t *session, const dtls_ecdsa_key_t **result);
    friend int verify_ecdsa_key(struct dtls_context_t *ctx, const session_t *session, const unsigned char *other_pub_x, const unsigned char *other_pub_y, size_t key_size);
    friend int send_to_peer(struct dtls_context_t *ctx, session_t *session, uint8 *data, size_t len);
    friend int read_from_peer(struct dtls_context_t *ctx, session_t *session, uint8 *data, size_t len);

private:
    struct dtls_connection_t {
        dtls_connection_t *  next;
        int                  sock;
        sockaddr_in6         addr;
        size_t               addrLen;
        session_t *          dtlsSession;
        #if DTLS_WITH_PSK
        OPAQUE_T             pubKey;
        OPAQUE_T             privKey;
        #elif DTLS_WITH_RPK
        dtls_ecdsa_key_t     ecdsa_key;
        #endif
        dtls_context_t *     dtlsContext;
        time_t               lastSend; // last time a data was sent to the server (used for NAT timeouts)
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
    dtls_connection_t * createNewConn(sockaddr * addr, size_t addrLen);
    int sendData(const Packet &packet);
    bool rehandshake(dtls_connection_t *conn, bool sendCloseNotify);
    dtls_connection_t * connFind(const sockaddr_storage * addr, size_t addrLen);
    bool sockAddrCmp(sockaddr *x, sockaddr *y);
    int getPort(sockaddr *x);

    #if DTLS_WITH_PSK
    const OPAQUE_T & getPublicKey(dtls_connection_t *conn);
    const OPAQUE_T & getSecretKey(dtls_connection_t *conn); 
    #elif DTLS_WITH_RPK
    const dtls_ecdsa_key_t & getEcdsaKey(dtls_connection_t *conn); 
    #endif

    bool setupSecurityKeys(Lwm2mSecurity& security, dtls_connection_t *conn);

    string uriToPort(string uri);
    string uriToHost(string uri);

private:
    string _port;
    int _addressFamily;
    int _connFd;

    dtls_context_t *_dtlsContext;
    dtls_connection_t *_connections;
};

#endif //CONNECTION_H