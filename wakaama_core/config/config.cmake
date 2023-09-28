option(LWM2M_BOOTSTRAP "Enable LWM2M Bootstrap support in a LWM2M Client" OFF)
if (LWM2M_BOOTSTRAP)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_BOOTSTRAP)
endif()

option(LWM2M_SUPPORT_SENML_JSON "Enable SenML JSON payload support" ON)
if (LWM2M_SUPPORT_SENML_JSON)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_SUPPORT_SENML_JSON)
endif()

option(LWM2M_SUPPORT_JSON "Enable JSON payload support" OFF)
if (LWM2M_SUPPORT_JSON)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_SUPPORT_JSON)
endif()

option(LWM2M_SUPPORT_TLV "Enable TLV payload support" ON)
if (LWM2M_SUPPORT_TLV)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_SUPPORT_TLV)
endif()

option(LWM2M_OLD_CONTENT_FORMAT_SUPPORT "Support the deprecated content format values for TLV and JSON" OFF)
if (LWM2M_OLD_CONTENT_FORMAT_SUPPORT)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_OLD_CONTENT_FORMAT_SUPPORT)
endif()

option(LWM2M_VERSION_1_0 "Support only version 1.0" OFF)
if (LWM2M_VERSION_1_0)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_VERSION_1_0)
endif()

option(LWM2M_RAW_BLOCK1_REQUESTS "For low memory client devices where it is not possible to keep a large post or put request in memory to be parsed" OFF)
if (LWM2M_RAW_BLOCK1_REQUESTS)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_RAW_BLOCK1_REQUESTS)
endif()

option(LWM2M_RAW_BLOCK1_REQUESTS "For low memory client devices where it is not possible to keep a large post or put request in memory to be parsed" OFF)
if (LWM2M_RAW_BLOCK1_REQUESTS)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_RAW_BLOCK1_REQUESTS)
endif()

option(LWM2M_LITTLE_ENDIAN "For low memory client devices where it is not possible to keep a large post or put request in memory to be parsed" OFF)
if (LWM2M_RAW_BLOCK1_REQUESTS)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_RAW_BLOCK1_REQUESTS)
endif()

option(LWM2M_WITH_LOGS "Enable logs for wakaama core" ON)
if (LWM2M_WITH_LOGS)
    set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_WITH_LOGS)
endif()

# CoAP block size used by CoAP layer when performing block-wise transfers. Possible values: 16, 32, 64, 128, 256, 512 and 1024.
set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_COAP_DEFAULT_BLOCK_SIZE=1024)

# Define your own endian if the endian is different from the platform default
# If your target platform uses big-endian format
# set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_BIG_ENDIAN)
# If your target platform uses little-endian format
# set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} LWM2M_LITTLE_ENDIAN)

# Share defines with target
set(WAKAAMA_CORE_DEFINITIONS ${WAKAAMA_CORE_DEFINITIONS} PARENT_SCOPE)