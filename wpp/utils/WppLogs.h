#ifndef WPP_LOGS_H
#define WPP_LOGS_H

// TODO: #include "liblwm2m.h"
#include "dep.h"

#if ENABLE_LOGS
#define LOG(STR) lwm2m_printf("[%s:%d] " STR "\r\n", __func__ , __LINE__)
#define LOG_ARG(FMT, ...) lwm2m_printf("[%s:%d] " FMT "\r\n", __func__ , __LINE__ , __VA_ARGS__)
#else
#define WPP_LOG(STR)
#define WPP_LOG_ARG(FMT, ...)
#endif

#endif //WPP_LOGS_H