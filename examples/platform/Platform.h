#ifndef PLATFORM_H
#define PLATFORM_H

#include "WppPlatformI.h"

class Platform : public wpp::WppPlatformI {
public:
    Platform() {}
    ~Platform() {}

    void* malloc(size_t size) override;
	void free(void *memPtr) override;
	time_t getTime(void) override;
	char* strdup(const char *str) override;
    int strncmp(const char *str1, const char *str2, size_t num) override;
    int strcasecmp(const char *str1, const char *str2) override;
    void print(const char * msg, va_list arg) override;
};

#endif //PLATFORM_H