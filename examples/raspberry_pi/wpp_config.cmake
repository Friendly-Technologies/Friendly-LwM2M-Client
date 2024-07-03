####################################################################
######################### Wakaama configs ##########################
####################################################################

# Wakaama should be always in the client mode
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_CLIENT_MODE)

# Support only version 1.0
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_VERSION_1_0)

# Support the deprecated content format values for TLV and JSON
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_OLD_CONTENT_FORMAT_SUPPORT)

# For low memory client devices where it is not possible to keep a large post
# or put request in memory to be parsed. Control over such operations is provided
# entirely to the user. At the moment, there are certain restrictions regarding
# the use of this mode, only two operations are supported BLOCK_EXECUTE without
# restrictions, and BLOCK_WRITE with the following restrictions: recording only
# one SINGLE resource, recording is possible in the following formats: TEXT, OPAQUE, TLV.
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_RAW_BLOCK1_REQUESTS)

# Define your own endian if the endian is different from the platform default
# If your target platform uses big-endian format
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_BIG_ENDIAN)
# If your target platform uses little-endian format
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_LITTLE_ENDIAN)

# CoAP block size used by CoAP layer when performing block-wise transfers. Possible values: 16, 32, 64, 128, 256, 512 and 1024.
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_COAP_DEFAULT_BLOCK_SIZE=1024)

# Enable LWM2M Bootstrap support in a LWM2M Client
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_BOOTSTRAP)

# Enable CBOR payload support
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_CBOR)

# Enable SenML CBOR payload support
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_SENML_CBOR)

# Enable SenML JSON payload support
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_SENML_JSON)

#Enable JSON payload support
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_JSON)

# Enable TLV payload support
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_SUPPORT_TLV)

# Preferred content type for bootstrap server and registration. 
# If not defined, then the server chooses its own content type.
# Possible values:
# SENML_JSON = 110
# JSON = 11543
# TLV = 11542
# SENML_CBOR = 112
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_BS_PREFERRED_CONTENT_TYPE=110)
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_REG_PREFERRED_CONTENT_TYPE=110)

# Enable logs for wakaama core
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} LWM2M_WITH_LOGS)



####################################################################
########################### Wpp configs ############################
####################################################################

# Enable logs for Wpp
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} WPP_ENABLE_LOGS)

# Set Wpp log level
# Possible values:
# 0 - Debug logs
# 1 - Info logs
# 2 - Warning logs
# 3 - Error logs
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} WPP_LOGS_LEVEL=0)



####################################################################
############## Mandatory and Optional object configs ###############
####################################################################

# ---------- Mandatory objects config block begin ----------
# Include mandatory Device object in the build
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_3_DEVICE)
# Include mandatory Lwm2mServer object in the build
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_1_LWM2M_SERVER)
# Include mandatory Lwm2mSecurity object in the build
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_0_LWM2M_SECURITY)
# ---------- Mandatory objects config block end ----------

# ---------- Optional objects config block begin ----------
# Include optional ConnectivityMonitoring object in the build
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_4_CONNECTIVITY_MONITORING)
# Include optional Lwm2mAccessControl object in the build
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_2_LWM2M_ACCESS_CONTROL)
# Include optional AudioClip object in the build
# set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_3339_AUDIO_CLIP)
# Include optional FirmwareUpdate object in the build
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_5_FIRMWARE_UPDATE)
# Include optional DigitalOutput object in the build"
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_3201_DIGITAL_OUTPUT)
# Include optional PushButton object in the build"
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_3347_PUSH_BUTTON)
# Include optional Temperature object in the build"
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_3303_TEMPERATURE)
# Include optional Accelerometer object in the build"
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_3313_ACCELEROMETER)
# ---------- Optional objects config block end ----------



# Share defines with target
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} PARENT_SCOPE)
