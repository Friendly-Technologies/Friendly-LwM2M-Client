#include "WppPlatform.h"


void* wpp::WppPlatform::malloc(size_t size) {
    return NULL; //TODO: Implement
}

void wpp::WppPlatform::free(void *memPtr) {
    //TODO: Implement
}

time_t wpp::WppPlatform::getTime(void) {
    return 0; //TODO: Implement
}

char* wpp::WppPlatform::strdup(const char *str) {
    return NULL; //TODO: Implement
}

int wpp::WppPlatform::strncmp(const char *str1, const char *str2, size_t num) {
    return 0; //TODO: Implement
}

int wpp::WppPlatform::strcasecmp(const char *str1, const char *str2) {
    return 0; //TODO: Implement
}

void wpp::WppPlatform::print(const char * msg, va_list arg) {
    //TODO: Implement
}
