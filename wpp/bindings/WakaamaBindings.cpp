#include "WppClient.h"
#include "WppPlatform.h"

//TODO: #include "liblwm2m.h"
#include "dep.h"

namespace wpp {

// TODO: Investigate other approaches to share the connection() with wakaama core code
WppClient * wpp_client() {
    return WppClient::client();
}

WppConnectionI * wpp_connection() {
    if (wpp_client()) return NULL;
    return &wpp_client()->connection();
}

} // wpp

extern "C" {
    /*---------- Platform bindings ----------*/
    // TODO: If we use user defined malloc for wakama then we also must
    // use it for WakaamaPlus, looks like we need custom allocator for
    // std containers or we should use custom containers
    void * lwm2m_malloc(size_t s) {
        return wpp::WppPlatform::malloc(s);
    }

    void lwm2m_free(void * p) {
        wpp::WppPlatform::free(p);
    }

    char * lwm2m_strdup(const char * str) {
        return wpp::WppPlatform::strdup(str);
    }

    int lwm2m_strncmp(const char * s1, const char * s2, size_t n) {
        return wpp::WppPlatform::strncmp(s1, s2, n);
    }

    int lwm2m_strcasecmp(const char * str1, const char * str2) {
        return wpp::WppPlatform::strcasecmp(str1, str2);
    }

    time_t lwm2m_gettime(void) {
        return wpp::WppPlatform::getTime();
    }

    void lwm2m_printf(const char * format, ...) {
        va_list ap;
        va_start(ap, format);
        wpp::WppPlatform::print(format, ap);
        va_end(ap);
    }

    /*---------- Connection bindings ----------*/
    void * lwm2m_connect_server(uint16_t secObjInstID, void * userData) {
		if (!wpp::WppClient::isCreated()) return NULL;

		wpp::Security *security = wpp::wpp_client()->registry().security().getInstance(secObjInstID);
		if (!security) return NULL;

		return wpp::wpp_connection()->connect(*security);
	}

	void lwm2m_close_connection(void * sessionH, void * userData) {
		if (!wpp::WppClient::isCreated()) return;
		return wpp::wpp_connection()->disconnect(sessionH);
	}

	uint8_t lwm2m_buffer_send(void * sessionH, uint8_t * buffer, size_t length, void * userData) {
		if (!wpp::WppClient::isCreated()) return COAP_500_INTERNAL_SERVER_ERROR;
		return wpp::wpp_connection()->sendPacket({sessionH, length, buffer})? COAP_NO_ERROR : COAP_500_INTERNAL_SERVER_ERROR;
	}

	bool lwm2m_session_is_equal(void * session1, void * session2, void * userData) {
		if (!wpp::WppClient::isCreated()) return false;
		return wpp::wpp_connection()->sessionCmp(session1, session2);
	}
}