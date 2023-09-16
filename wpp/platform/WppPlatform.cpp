#include "WppPlatform.h"

//TODO: #include "liblwm2m.h"
#include "dep.h"

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
}
