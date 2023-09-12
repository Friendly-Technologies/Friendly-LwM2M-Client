/*
 * WppPlatformI.h
 *
 *  Created on: 31 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_PLATFORM_H_
#define WPP_PLATFORM_H_

#include <string>

namespace wpp {

class WppPlatformI {
	friend class WppClient;
	
public:
	WppPlatformI() {}
	virtual ~WppPlatformI() {}

    WppPlatformI(const WppPlatformI&) = delete;
	WppPlatformI(WppPlatformI&&) = delete;
	WppPlatformI& operator=(const WppPlatformI&) = delete;
	WppPlatformI& operator=(WppPlatformI&&) = delete;

    /* ------------- WppPlatformI dependent functionality ------------- */
    virtual void* malloc(size_t size) = 0;
	virtual void free(void *memPtr) = 0;
	virtual time_t getTime(void) = 0;
	virtual char* strdup(const char *str) = 0;
    virtual int strncmp(const char *str1, const char *str2, size_t num) = 0;
    virtual int strcasecmp(const char *str1, const char *str2) = 0;
    virtual void print(const char * msg, va_list arg) = 0;

    void print(const char * msg, ...);
};

} // namespace wpp

#endif /* WPP_PLATFORM_H_ */
