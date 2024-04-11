# Wakaama should be always in the client mode
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_CLIENT_MODE)

# CoAP block size used by CoAP layer when performing block-wise transfers. Possible values: 16, 32, 64, 128, 256, 512 and 1024.
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_COAP_DEFAULT_BLOCK_SIZE=1024)

# Define your own endian if the endian is different from the platform default
# If your target platform uses big-endian format
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_BIG_ENDIAN)
# If your target platform uses little-endian format
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_LITTLE_ENDIAN)

option(LWM2M_BOOTSTRAP "Enable LWM2M Bootstrap support in a LWM2M Client" OFF)
if (LWM2M_BOOTSTRAP)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_BOOTSTRAP)
endif()

option(LWM2M_SUPPORT_SENML_JSON "Enable SenML JSON payload support" ON)
if (LWM2M_SUPPORT_SENML_JSON)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_SENML_JSON)
endif()

option(LWM2M_SUPPORT_JSON "Enable JSON payload support" ON)
if (LWM2M_SUPPORT_JSON)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_JSON)
endif()

option(LWM2M_SUPPORT_TLV "Enable TLV payload support" ON)
if (LWM2M_SUPPORT_TLV)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_TLV)
endif()

option(LWM2M_SUPPORT_CBOR "Enable CBOR payload support" ON)
if (LWM2M_SUPPORT_CBOR)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_CBOR)
endif()

option(LWM2M_SUPPORT_SENML_CBOR "Enable SenML CBOR payload support" ON)
if (LWM2M_SUPPORT_SENML_CBOR)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_SENML_CBOR)
endif()

option(LWM2M_OLD_CONTENT_FORMAT_SUPPORT "Support the deprecated content format values for TLV and JSON" OFF)
if (LWM2M_OLD_CONTENT_FORMAT_SUPPORT)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_OLD_CONTENT_FORMAT_SUPPORT)
endif()

# Preferred content type for bootstrap server and registration. 
# If not defined, then the server chooses its own content type.
# Possible values:
# SENML_JSON = 110
# JSON = 11543
# TLV = 11542
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_BS_PREFERRED_CONTENT_TYPE=110)
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_REG_PREFERRED_CONTENT_TYPE=110)

option(LWM2M_VERSION_1_0 "Support only version 1.0" OFF)
if (LWM2M_VERSION_1_0)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_VERSION_1_0)
endif()

option(LWM2M_RAW_BLOCK1_REQUESTS "For low memory client devices where it is not possible to keep a large post
                                  or put request in memory to be parsed. Control over such operations is provided
                                  entirely to the user. At the moment, there are certain restrictions regarding
                                  the use of this mode, only two operations are supported BLOCK_EXECUTE without
                                  restrictions, and BLOCK_WRITE with the following restrictions: recording only
                                  one SINGLE resource, recording is possible in the following formats: TEXT, OPAQUE, TLV." OFF)
if (LWM2M_RAW_BLOCK1_REQUESTS)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_RAW_BLOCK1_REQUESTS)
endif()

option(LWM2M_WITH_LOGS "Enable logs for wakaama core" ON)
if (LWM2M_WITH_LOGS)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_WITH_LOGS)
endif()

# Share defines with target
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} PARENT_SCOPE)
