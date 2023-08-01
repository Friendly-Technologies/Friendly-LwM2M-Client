#include "WppPlatform.h"

namespace wpp {

WppPlatform *WppPlatform::_platform = NULL;

WppPlatform::WppPlatform(MALLOC_T &malloc, FREE_T &free, GET_TIME_T &getTime, STRDUP_T &strdup, STRNCMP_T &strncmp, STRCASECMP_T &strcasecmp, PRINT_T &print):
    _malloc(malloc), _free(free), _getTime(getTime), _strdup(strdup), _strncmp(strncmp), _strcasecmp(strcasecmp), _print(print) {}

WppPlatform::~WppPlatform() {}

/* ------------- WppPlatform management ------------- */
bool WppPlatform::create(MALLOC_T &malloc, FREE_T &free, GET_TIME_T &getTime, STRDUP_T &strdup, STRNCMP_T &strncmp, STRCASECMP_T &strcasecmp, PRINT_T &print) {
    if (!malloc || !free || !getTime || !strdup || !strncmp || !strcasecmp || !print) return false;
    _platform = new WppPlatform(malloc, free, getTime, strdup, strncmp, strcasecmp, print);
	return true;
}

bool WppPlatform::isCreated() {
    return _platform != NULL;
}

WppPlatform* WppPlatform::instance() {
    return _platform;
}

/* ------------- WppPlatform dependent functionality ------------- */
void* WppPlatform::malloc(size_t size) {
    return _malloc(size);
}

void WppPlatform::free(void *memPtr) {
    _free(memPtr);
}

time_t WppPlatform::getTime(void) {
    return _getTime();
}

char* WppPlatform::strdup(const char *str) {
    return _strdup(str);
}

int WppPlatform::strncmp(const char *str1, const char *str2, size_t num) {
    return _strncmp(str1, str2, num);
}

int WppPlatform::strcasecmp(const char *str1, const char *str2) {
    return _strcasecmp(str1, str2);
}

void print(const char * msg, ...) {
    va_list ap;

    va_start(ap, msg);
    print(msg, ap);
    va_end(ap);
}

void WppPlatform::print(const char * format, va_list arg) {
    _print(format, arg);
}

} //namespace wpp

extern "C" {
    // TODO: If we use user defined malloc for wakama then we also must
    // use it for WakaamaPlus, looks like we need custom allocator for
    // std containers or we should use custom containers
    void * lwm2m_malloc(size_t s) {
        if (!wpp::WppPlatform::isCreated()) return NULL;
        return wpp::WppPlatform::instance()->malloc(s);
    }

    void lwm2m_free(void * p) {
        if (!wpp::WppPlatform::isCreated()) return;
        wpp::WppPlatform::instance()->free(p);
    }

    char * lwm2m_strdup(const char * str) {
        if (!wpp::WppPlatform::isCreated()) return NULL;
        return wpp::WppPlatform::instance()->strdup(str);
    }

    int lwm2m_strncmp(const char * s1, const char * s2, size_t n) {
        if (!wpp::WppPlatform::isCreated()) return 0;
        return wpp::WppPlatform::instance()->strncmp(s1, s2, n);
    }

    int lwm2m_strcasecmp(const char * str1, const char * str2) {
        if (!wpp::WppPlatform::isCreated()) return 0;
        return wpp::WppPlatform::instance()->strcasecmp(str1, str2);
    }

    time_t lwm2m_gettime(void) {
        if (!wpp::WppPlatform::isCreated()) return 0;
        return wpp::WppPlatform::instance()->getTime();
    }

    void lwm2m_printf(const char * format, ...) {
        if (!wpp::WppPlatform::isCreated()) return;
        
        va_list ap;
        va_start(ap, format);
        wpp::WppPlatform::instance()->print(format, ap);
        va_end(ap);
    }

}