# ---------- Mandatory objects config blok begin ----------
option(OBJ_M_3_DEVICE_V12 "Include mandatory Device object in the build" ON)
if (OBJ_M_3_DEVICE_V12)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_3_DEVICE_V12=1)
endif()
option(OBJ_M_0_LWM2M_SECURITY_V11 "Include mandatory Lwm2mSecurity object in the build" ON)
if (OBJ_M_0_LWM2M_SECURITY_V11)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_0_LWM2M_SECURITY_V11=1)
endif()
option(OBJ_M_1_LWM2M_SERVER_V11 "Include mandatory Lwm2mServer object in the build" ON)
if (OBJ_M_1_LWM2M_SERVER_V11)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_1_LWM2M_SERVER_V11=1)
endif()
# ---------- Mandatory objects config blok end ----------

# ---------- Optional objects config blok begin ----------
option(OPTIONAL_ACL_OBJ "Include optional ACL object in the build" OFF)
if (ACL_OBJ)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OPTIONAL_ACL_OBJ=1)
endif()
option(OPTIONAL_FIRMWARE_UPD_OBJ "Include optional Firmware update in the build" OFF)
if (OPTIONAL_FIRMWARE_UPD_OBJ)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OPTIONAL_FIRMWARE_UPD_OBJ=1)
endif()
option(OBJ_O_4_CONNECTIVITY_MONITORING_V13 "Include optional ConnectivityMonitoring object in the build" OFF)
if (OBJ_O_4_CONNECTIVITY_MONITORING_V13)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_4_CONNECTIVITY_MONITORING_V13=1)
endif()
# ---------- Optional objects config blok end ----------

# Share defines with target
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} PARENT_SCOPE)