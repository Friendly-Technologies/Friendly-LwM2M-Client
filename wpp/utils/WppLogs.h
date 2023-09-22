#ifndef WPP_LOGS_H
#define WPP_LOGS_H

#include "WppPlatform.h"
#include "liblwm2m.h"

/* ---------- WPP components log TAGs start ---------- */
#define TAG_WPP_CLIENT      "WppClient"
#define TAG_WPP_CONN        "IWppConnection"
#define TAG_WPP_REG         "WppRegistry"
#define TAG_WPP_OBJ         "WppObject<T>"
#define TAG_WPP_INST        "IWppInstance"
/* ---------- WPP components log TAGs end ---------- */

/* ---------- Logs config start ---------- */
#define LOGS_LEVEL_DEBUG    0
#define LOGS_LEVEL_INFO     1
#define LOGS_LEVEL_WARNING  2
#define LOGS_LEVEL_ERROR    3
#define LOGS_LEVEL          LOGS_LEVEL_DEBUG
/* ---------- Logs config end ---------- */

#if ENABLE_LOGS && LOGS_LEVEL <= LOGS_LEVEL_DEBUG
    #define WPP_LOGD(TAG, STR) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [DEBUG] " STR "\r\n", TAG, __func__ , __LINE__)
    #define WPP_LOGD_ARG(TAG, FMT, ...) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [DEBUG] " FMT "\r\n", TAG, __func__ , __LINE__ , __VA_ARGS__)
#else
    #define WPP_LOGD(TAG, STR) 
    #define WPP_LOGD_ARG(TAG, FMT, ...) 
#endif

#if ENABLE_LOGS && LOGS_LEVEL <= LOGS_LEVEL_INFO
    #define WPP_LOGI(TAG, STR) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [INFO] " STR "\r\n", TAG, __func__ , __LINE__)
    #define WPP_LOGI_ARG(TAG, FMT, ...) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [INFO] " FMT "\r\n", TAG, __func__ , __LINE__ , __VA_ARGS__)
#else
    #define WPP_LOGI(TAG, STR) 
    #define WPP_LOGI_ARG(TAG, FMT, ...) 
#endif

#if ENABLE_LOGS && LOGS_LEVEL <= LOGS_LEVEL_WARNING
    #define WPP_LOGW(TAG, STR) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [WARNING] " STR "\r\n", TAG, __func__ , __LINE__)
    #define WPP_LOGW_ARG(TAG, FMT, ...) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [WARNING] " FMT "\r\n", TAG, __func__ , __LINE__ , __VA_ARGS__)
#else
    #define WPP_LOGW(TAG, STR) 
    #define WPP_LOGW_ARG(TAG, FMT, ...) 
#endif

#if ENABLE_LOGS && LOGS_LEVEL <= LOGS_LEVEL_ERROR
    #define WPP_LOGE(TAG, STR) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [ERROR] " STR "\r\n", TAG, __func__ , __LINE__)
    #define WPP_LOGE_ARG(TAG, FMT, ...) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [ERROR] " FMT "\r\n", TAG, __func__ , __LINE__ , __VA_ARGS__)
#else
    #define WPP_LOGE(TAG, STR) 
    #define WPP_LOGE_ARG(TAG, FMT, ...) 
#endif

#endif //WPP_LOGS_H