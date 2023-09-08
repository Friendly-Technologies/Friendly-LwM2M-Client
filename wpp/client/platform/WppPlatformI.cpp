#include "WppPlatformI.h"

namespace wpp {

void WppPlatformI::print(const char * msg, ...) {
    va_list ap;

    va_start(ap, msg);
    print(msg, ap);
    va_end(ap);
}

} //namespace wpp