#include "WppClient.h"
#include "dep.h"

extern "C" {
    /*---------- Platform bindings ----------*/
    // TODO: If we use user defined malloc for wakama then we also must
    // use it for WakaamaPlus, looks like we need custom allocator for
    // std containers or we should use custom containers
    void * lwm2m_malloc(size_t s) {
        if (!wpp::WppClient::isCreated()) return NULL;
        return wpp::WppClient::client()->platform().malloc(s);
    }

    void lwm2m_free(void * p) {
        if (!wpp::WppClient::isCreated()) return;
        wpp::WppClient::client()->platform().free(p);
    }

    char * lwm2m_strdup(const char * str) {
        if (!wpp::WppClient::isCreated()) return NULL;
        return wpp::WppClient::client()->platform().strdup(str);
    }

    int lwm2m_strncmp(const char * s1, const char * s2, size_t n) {
        if (!wpp::WppClient::isCreated()) return 0;
        return wpp::WppClient::client()->platform().strncmp(s1, s2, n);
    }

    int lwm2m_strcasecmp(const char * str1, const char * str2) {
        if (!wpp::WppClient::isCreated()) return 0;
        return wpp::WppClient::client()->platform().strcasecmp(str1, str2);
    }

    time_t lwm2m_gettime(void) {
        if (!wpp::WppClient::isCreated()) return 0;
        return wpp::WppClient::client()->platform().getTime();
    }

    void lwm2m_printf(const char * format, ...) {
        if (!wpp::WppClient::isCreated()) return;
        
        va_list ap;
        va_start(ap, format);
        wpp::WppClient::client()->platform().print(format, ap);
        va_end(ap);
    }

    /*---------- Connection bindings ----------*/
    void * lwm2m_connect_server(uint16_t secObjInstID, void * userData) {
		if (!wpp::WppClient::isCreated()) return NULL;

		wpp::Security *security = wpp::WppClient::client()->registry().security().getInstance(secObjInstID);
		if (!security) return NULL;

		return wpp::WppClient::client()->connection().connect(*security);
	}

	void lwm2m_close_connection(void * sessionH, void * userData) {
		if (!wpp::WppClient::isCreated()) return;
		return wpp::WppClient::client()->connection().disconnect(sessionH);
	}

	uint8_t lwm2m_buffer_send(void * sessionH, uint8_t * buffer, size_t length, void * userData) {
		if (!wpp::WppClient::isCreated()) return COAP_500_INTERNAL_SERVER_ERROR;
		return wpp::WppClient::client()->connection().sendPacket(sessionH, buffer, length)? COAP_NO_ERROR : COAP_500_INTERNAL_SERVER_ERROR;
	}

	bool lwm2m_session_is_equal(void * session1, void * session2, void * userData) {
		if (!wpp::WppClient::isCreated()) return false;
		return wpp::WppClient::client()->connection().sessionCmp(session1, session2);
	}

}