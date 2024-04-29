#ifndef WPP_LOGS_H
#define WPP_LOGS_H

#include "WppPlatform.h"
#include "liblwm2m.h"

/* ---------- WPP components log TAGs start ---------- */
#define TAG_WPP_CLIENT      "WppClient"
#define TAG_WPP_CONN        "WppConnection"
#define TAG_WPP_REG         "WppRegistry"
#define TAG_WPP_OBJ         "Object"
#define TAG_WPP_INST        "Instance"
#define TAG_WPP_RES         "Resource"
#define TAG_WPP_RES_CON     "ResourceContainer"
#define TAG_WPP_TASK        "WppTaskQueue"
/* ---------- WPP components log TAGs end ---------- */

/* ---------- Logs config start ---------- */
#define WPP_LOGS_LEVEL_DEBUG    0
#define WPP_LOGS_LEVEL_INFO     1
#define WPP_LOGS_LEVEL_WARNING  2
#define WPP_LOGS_LEVEL_ERROR    3
#ifndef WPP_LOGS_LEVEL
    #define WPP_LOGS_LEVEL WPP_LOGS_LEVEL_DEBUG
#endif
/* ---------- Logs config end ---------- */

#if defined(WPP_ENABLE_LOGS) && WPP_LOGS_LEVEL <= WPP_LOGS_LEVEL_DEBUG
    #define WPP_LOGD(TAG, FMT, ...) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [DEBUG] " FMT "\r\n", TAG, __func__ , __LINE__ , ##__VA_ARGS__)
#else
    #define WPP_LOGD(TAG, FMT, ...) 
#endif

#if defined(WPP_ENABLE_LOGS) && WPP_LOGS_LEVEL <= WPP_LOGS_LEVEL_INFO
    #define WPP_LOGI(TAG, FMT, ...) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [INFO] " FMT "\r\n", TAG, __func__ , __LINE__ , ##__VA_ARGS__)
#else
    #define WPP_LOGI(TAG, FMT, ...) 
#endif

#if defined(WPP_ENABLE_LOGS) && WPP_LOGS_LEVEL <= WPP_LOGS_LEVEL_WARNING
    #define WPP_LOGW(TAG, FMT, ...) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [WARNING] " FMT "\r\n", TAG, __func__ , __LINE__ , ##__VA_ARGS__)
#else
    #define WPP_LOGW(TAG, FMT, ...) 
#endif

#if defined(WPP_ENABLE_LOGS) && WPP_LOGS_LEVEL <= WPP_LOGS_LEVEL_ERROR
    #define WPP_LOGE(TAG, FMT, ...) wpp::WppPlatform::print("[wpp:%s] [%s():%d] [ERROR] " FMT "\r\n", TAG, __func__ , __LINE__ , ##__VA_ARGS__)
#else
    #define WPP_LOGE(TAG, FMT, ...) 
#endif

#endif //WPP_LOGS_H