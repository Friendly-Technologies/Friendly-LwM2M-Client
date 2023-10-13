# Options that define here determine availability of general components
# Settings for the behavior of a particular component should be in a
# separate .h file near the definition of the component itself.

# The start of loging.
option(ENABLE_LOGS "Enable logs for WakaamaPlus" ON)
if (ENABLE_LOGS)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} ENABLE_LOGS=1)
endif()
# The end of loging.

# The option of the mandatory object ends with the letter M.
# The start of the options of the mandatory objects.
option(OBJ_M_3_DEVICE_V12 "Include mandatory Device object in the build" ON)
if (OBJ_M_3_DEVICE_V12)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_3_DEVICE_V12=1)
endif()
option(OBJ_M_0_LWM2M_SECURITY_V11 "Include mandatory Lwm2mSecurity object in the build" ON)
if (OBJ_M_0_LWM2M_SECURITY_V11)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_0_LWM2M_SECURITY_V11=1)
endif()
option(OBJ_M_1_LWM2M_SERVER_V11 "Include mandatory LwM2MServer object in the build" ON)
if (OBJ_M_1_LWM2M_SERVER_V11)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_1_LWM2M_SERVER_V11=1)
endif()
# The end of the options of the mandatory objects.
# !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!!

# The option of the optional object ends with the letter O.
# The start of the options of the optional objects.
option(OPTIONAL_ACL_OBJ "Include optional ACL object in the build" OFF)
if (ACL_OBJ)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OPTIONAL_ACL_OBJ=1)
endif()

option(OPTIONAL_CONN_MONITORING_OBJ "Include optional Connection monitoring object in the build" OFF)
if (OPTIONAL_CONN_MONITORING_OBJ)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OPTIONAL_CONN_MONITORING_OBJ=1)
endif()

option(OPTIONAL_FIRMWARE_UPD_OBJ "Include optional Firmware update in the build" OFF)
if (OPTIONAL_FIRMWARE_UPD_OBJ)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OPTIONAL_FIRMWARE_UPD_OBJ=1)
endif()
# The end of the options of the optional objects.
# !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!!

# Share defines with target
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} PARENT_SCOPE)