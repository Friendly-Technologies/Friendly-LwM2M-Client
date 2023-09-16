#ifndef WPP_LOGS_H
#define WPP_LOGS_H

#include "WppPlatform.h"
// TODO: #include "liblwm2m.h"
#include "dep.h"

/* ---------- Logs config start ---------- */
#define LOGS_LEVEL_DEBUG    0
#define LOGS_LEVEL_INFO     1
#define LOGS_LEVEL_WARNING  2
#define LOGS_LEVEL_ERROR    3
#define LOGS_LEVEL          LOGS_LEVEL_DEBUG
/* ---------- Logs config end ---------- */

#if ENABLE_LOGS && LOGS_LEVEL <= LOGS_LEVEL_DEBUG
    #define WPP_LOGD(STR) wpp::WppPlatform::print("[%s():%d] [DEBUG] " STR "\r\n", __func__ , __LINE__)
    #define WPP_LOGD_ARG(FMT, ...) wpp::WppPlatform::print("[%s():%d] [DEBUG] " FMT "\r\n", __func__ , __LINE__ , __VA_ARGS__)
#else
    #define WPP_LOGD(STR) 
    #define WPP_LOGD_ARG(FMT, ...) 
#endif

#if ENABLE_LOGS && LOGS_LEVEL <= LOGS_LEVEL_INFO
    #define WPP_LOGI(STR) wpp::WppPlatform::print("[%s():%d] [INFO] " STR "\r\n", __func__ , __LINE__)
    #define WPP_LOGI_ARG(FMT, ...) wpp::WppPlatform::print("[%s():%d] [INFO] " FMT "\r\n", __func__ , __LINE__ , __VA_ARGS__)
#else
    #define WPP_LOGI(STR) 
    #define WPP_LOGI_ARG(FMT, ...) 
#endif

#if ENABLE_LOGS && LOGS_LEVEL <= LOGS_LEVEL_WARNING
    #define WPP_LOGW(STR) wpp::WppPlatform::print("[%s():%d] [WARNING] " STR "\r\n", __func__ , __LINE__)
    #define WPP_LOGW_ARG(FMT, ...) wpp::WppPlatform::print("[%s():%d] [WARNING] " FMT "\r\n", __func__ , __LINE__ , __VA_ARGS__)
#else
    #define WPP_LOGW(STR) 
    #define WPP_LOGW_ARG(FMT, ...) 
#endif

#if ENABLE_LOGS && LOGS_LEVEL <= LOGS_LEVEL_ERROR
    #define WPP_LOGE(STR) wpp::WppPlatform::print("[%s():%d] [ERROR] " STR "\r\n", __func__ , __LINE__)
    #define WPP_LOGE_ARG(FMT, ...) wpp::WppPlatform::print("[%s():%d] [ERROR] " FMT "\r\n", __func__ , __LINE__ , __VA_ARGS__)
#else
    #define WPP_LOGE(STR) 
    #define WPP_LOGE_ARG(FMT, ...) 
#endif

#endif //WPP_LOGS_H