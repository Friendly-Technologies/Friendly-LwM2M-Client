#include "catch_amalgamated.hpp"
#include "WppPlatform.h"
#include "liblwm2m.h"

#ifndef LWM2M_WITH_LOGS
extern "C" {
    extern void lwm2m_printf(const char * format, ...);
}
#endif

TEST_CASE("WppPlatform", "[lwm2m_malloc][lwm2m_strdup][lwm2m_strncmp][lwm2m_strcasecmp][lwm2m_gettime][lwm2m_printf]") {
    SECTION("lwm2m_malloc") {
        void * p = lwm2m_malloc(10);
        REQUIRE(p != NULL);
        lwm2m_free(p);
    }

    SECTION("lwm2m_strdup") {
        char * str = lwm2m_strdup("Hello World!");
        REQUIRE(str != NULL);
        REQUIRE(strcmp(str, "Hello World!") == 0);
        lwm2m_free(str);
        REQUIRE(lwm2m_strdup(NULL) == NULL);
    }

    SECTION("lwm2m_strncmp") {
        REQUIRE(lwm2m_strncmp("Hello World!", "Hello World!", 12) == 0);
        REQUIRE(lwm2m_strncmp("Hello World!", "Hello World!", 11) == 0);
        REQUIRE(lwm2m_strncmp("Hello World!", "Hello World?", 12) != 0);
        REQUIRE(lwm2m_strncmp("Hello World!", "Hello World?", 11) == 0);
    }

    SECTION("lwm2m_strcasecmp") {
        REQUIRE(lwm2m_strcasecmp("Hello World!", "HELLO World!") == 0);
        REQUIRE(lwm2m_strcasecmp("hello world!", "Hello World!") == 0);
    }

    SECTION("lwm2m_gettime") {
        REQUIRE(lwm2m_gettime() > 0);
    }

    SECTION("lwm2m_printf") {
        lwm2m_printf("Hello World! %d\n", 2023);
        wpp::WppPlatform::print("Hello World!");
    }
}