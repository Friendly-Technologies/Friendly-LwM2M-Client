#include "WppPlatform.h"
#include <ctime>

time_t wpp::WppPlatform::getTime(void) {
    return std::time(NULL);
}

void wpp::WppPlatform::print(const char * msg, va_list arg) {
}
