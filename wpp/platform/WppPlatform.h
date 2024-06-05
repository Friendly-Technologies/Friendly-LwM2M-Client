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

/**
 * @brief The WppPlatform class provides a platform-specific implementation for the Wpp library.
 * 
 * This class uses the Linker Callback pattern, where the implementation of methods is carried out on the user side.
 * It provides functions for retrieving the elapsed time, printing messages, and simplifying the usage of the print function.
 * 
 * The WppPlatform class is designed to be platform-independent and can be used with different operating systems or environments.
 *
 * @see wpp::getTime
 * @see wpp::print
 */
class WppPlatform {
public:
    /**
     * @brief Returns the number of seconds elapsed since a specific origin.
     * 
     * This function must be implemented by the user and should return the number of seconds elapsed since a specific origin.
     * The origin can be any reference point, such as the system boot time or the Epoch.
     * 
     * @note Per POSIX specifications, time_t is a signed integer.
     */
    static time_t getTime(void);

    /**
     * @brief Prints a formatted message.
     * 
     * This function is used to print a formatted message to the output.
     * It follows the same usage as the C89 printf() function.
     * 
     * @param msg The format string for the message.
     * @param arg The variable arguments list.
     * 
     * @see wpp::print(const char *, ...)
     */
    static void print(const char * msg, va_list arg);

    /**
     * @brief Prints a formatted message.
     * 
     * This function is a helpful implementation that simplifies the usage of the print(const char *, va_list) function.
     * It allows the user to directly pass the format string and variable arguments without explicitly using va_list.
     * 
     * @param msg The format string for the message.
     * @param ... The variable arguments.
     * 
     * @see wpp::print(const char *, va_list)
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
