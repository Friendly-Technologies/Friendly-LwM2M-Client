#include "WppPlatform.h"

#include <cstring>
#include "liblwm2m.h"

/*
 * With this file we have problem related to linker optimization.
 * Unlike dynamic libraries, which are fully linked when they are created,
 * static libraries are essentially archives of object files (.o files).
 * When we link an executable against a static library, the linker only
 * pulls in the object files that contain symbols that are actually used 
 * by the executable. This is to reduce the final size of the executable.
 * If an object file inside the static library does not contain any symbols
 * that are referenced by the executable, that object file will not be 
 * included in the final link. 
 * 
 * This is exactly the problem with this file, since its functionality is 
 * not directly used by either the user code or the Wpp library, so the 
 * linker tries to optimize it, and the WakaamaCore library, due to the 
 * optimization of this file, does not get the implementation of the 
 * necessary functions. The solution can be the linker flag --whole-archive, 
 * which should be added to the user CMake file, it includes all object 
 * files from the Wpp library, while it does not extend to the WakaamaCore 
 * library, and when optimization is enabled, it does not add overhead to 
 * the .text section. Another approach for solving the problem is combining 
 * the source code of wakaama core and wpp library into a single library.
 * 
 * For now, uses the second approach.
 */
extern "C" {
    /*---------- Platform bindings ----------*/
    void * lwm2m_malloc(size_t s) {
        return new uint8_t[s];
    }

    void lwm2m_free(void * p) {
        delete [] (uint8_t *)p;
    }

    char * lwm2m_strdup(const char * str) {
        if (!str) return NULL;

        const int len = strlen(str) + 1;
        char * const buf = (char *)lwm2m_malloc(len);
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