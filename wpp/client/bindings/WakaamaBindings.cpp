#include "WppClient.h"

//TODO: #include "liblwm2m.h"
#include "dep.h"

namespace wpp {

WppClient * wpp_client() {
    return WppClient::client();
}

WppPlatformI * wpp_platform() {
    if (wpp_client()) return NULL;
    return &wpp_client()->platform();
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
        if (!wpp::WppClient::isCreated()) return NULL;
        return wpp::wpp_platform()->malloc(s);
    }

    void lwm2m_free(void * p) {
        if (!wpp::WppClient::isCreated()) return;
        wpp::wpp_platform()->free(p);
    }

    char * lwm2m_strdup(const char * str) {
        if (!wpp::WppClient::isCreated()) return NULL;
        return wpp::wpp_platform()->strdup(str);
    }

    int lwm2m_strncmp(const char * s1, const char * s2, size_t n) {
        if (!wpp::WppClient::isCreated()) return 0;
        return wpp::wpp_platform()->strncmp(s1, s2, n);
    }

    int lwm2m_strcasecmp(const char * str1, const char * str2) {
        if (!wpp::WppClient::isCreated()) return 0;
        return wpp::wpp_platform()->strcasecmp(str1, str2);
    }

    time_t lwm2m_gettime(void) {
        if (!wpp::WppClient::isCreated()) return 0;
        return wpp::wpp_platform()->getTime();
    }

    void lwm2m_printf(const char * format, ...) {
        if (!wpp::WppClient::isCreated()) return;
        
        va_list ap;
        va_start(ap, format);
        wpp::wpp_platform()->print(format, ap);
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