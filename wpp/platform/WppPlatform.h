/*
 * WppPlatform.h
 *
 *  Created on: 31 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_PLATFORM_H_
#define WPP_PLATFORM_H_

#include <stdarg.h>
#include <string>
#include <functional>

#include "WppRegistry.h"
#include "WppCore.h"


namespace wpp {

class WppPlatform {
public:
	using MALLOC_T = std::function<void*(size_t)>;
	using FREE_T = std::function<void(void*)>;
	using GET_TIME_T = std::function<time_t(void)>;
	using STRDUP_T = std::function<char*(const char *)>;
    using STRNCMP_T = std::function<int(const char *, const char *, size_t)>;
    using STRCASECMP_T = std::function<int(const char *, const char *)>;
    using PRINT_T = void(*)(const char * msg, va_list arg);
private:
	WppPlatform(MALLOC_T &malloc, FREE_T &free, GET_TIME_T &getTime, STRDUP_T &strdup, STRNCMP_T &strncmp, STRCASECMP_T &strcasecmp, PRINT_T &print);

public:
	~WppPlatform();

	/* ------------- WppPlatform management ------------- */
	static bool create(MALLOC_T &malloc, FREE_T &free, GET_TIME_T &getTime, STRDUP_T &strdup, STRNCMP_T &strncmp, STRCASECMP_T &strcasecmp, PRINT_T &print);
	static bool isCreated();
	static WppPlatform* instance();

    /* ------------- WppPlatform dependent functionality ------------- */
    void* malloc(size_t size);
	void free(void *memPtr);
	time_t getTime(void);
	char* strdup(const char *str);
    int strncmp(const char *str1, const char *str2, size_t num);
    int strcasecmp(const char *str1, const char *str2);
    void print(const char * msg, ...);
    void print(const char * msg, va_list arg);

private:
	static WppPlatform *_platform;

    MALLOC_T _malloc;
    FREE_T _free;
    GET_TIME_T _getTime;
    STRDUP_T _strdup;
    STRNCMP_T _strncmp;
    STRCASECMP_T _strcasecmp;
    PRINT_T _print;
};

} // namespace wpp

#endif /* WPP_PLATFORM_H_ */
