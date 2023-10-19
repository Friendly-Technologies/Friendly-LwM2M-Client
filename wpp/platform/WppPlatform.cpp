#include "WppPlatform.h"

#include <cstring>

//TODO: #include "liblwm2m.h"
#include "dep.h"

extern "C" {
    /*---------- Platform bindings ----------*/
    void * lwm2m_malloc(size_t s) {
        return new uint8_t[s];
    }

    void lwm2m_free(void * p) {
        delete [] p;
    }

    char * lwm2m_strdup(const char * str) {
        if (!str) return NULL;

        const int len = strlen(str) + 1;
        char * const buf = lwm2m_malloc(len);
        if (buf) {
            memset(buf, 0, len);
            memcpy(buf, str, len - 1);
        }

        return buf;
    }

    int lwm2m_strncmp(const char * s1, const char * s2, size_t n) {
        return strncmp(s1, s2, n);
    }

    int lwm2m_strcasecmp(const char * str1, const char * str2) {
        return strcasecmp(str1, str2);
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
