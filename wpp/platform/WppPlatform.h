/*
 * WppPlatform.h
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
    /*
     * This function must return the number of seconds elapsed since origin.
     * The origin (Epoch, system boot, etc...) does not matter as this
     * function is used only to determine the elapsed time since the last
     * call to it.
     * In case of error, this must return a negative value.
     * Per POSIX specifications, time_t is a signed integer.
     */
    static time_t getTime(void);
    /*
     * Same usage as C89 printf()
     */
    static void print(const char * msg, va_list arg);
    /*
     * Helpful implementation for simplifying usage of print(const char *, va_list);
     */
    static void print(const char * msg, ...) {
        va_list ap;
        va_start(ap, msg);
        print(msg, ap);
        va_end(ap);
    }
};

} // namespace wpp

#endif /* WPP_PLATFORM_H_ */
