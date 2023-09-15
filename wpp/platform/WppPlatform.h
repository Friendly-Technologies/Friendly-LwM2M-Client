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

/*
 * This class uses the Linker Callback pattern.
 * Implementation of methods is carried out on the user side.
 */
class WppPlatform {
public:
	static void* malloc(size_t size);
	static void free(void *memPtr);
	static time_t getTime(void);
	static char* strdup(const char *str);
    static int strncmp(const char *str1, const char *str2, size_t num);
    static int strcasecmp(const char *str1, const char *str2);
    static void print(const char * msg, va_list arg);
    static void print(const char * msg, ...) {
        va_list ap;
        va_start(ap, msg);
        print(msg, ap);
        va_end(ap);
    }
};

} // namespace wpp

#endif /* WPP_PLATFORM_H_ */
