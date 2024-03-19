# ---------- Mandatory objects config block begin ----------
option(OBJ_M_3_DEVICE "Include mandatory Device object in the build" ON)
if (OBJ_M_3_DEVICE)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_3_DEVICE)
endif()
option(OBJ_M_1_LWM2M_SERVER "Include mandatory Lwm2mServer object in the build" ON)
if (OBJ_M_1_LWM2M_SERVER)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_1_LWM2M_SERVER)
endif()
option(OBJ_M_0_LWM2M_SECURITY "Include mandatory Lwm2mSecurity object in the build" ON)
if (OBJ_M_0_LWM2M_SECURITY)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_M_0_LWM2M_SECURITY)
endif()
# ---------- Mandatory objects config block end ----------

# ---------- Optional objects config block begin ----------
option(OBJ_O_4_CONNECTIVITY_MONITORING "Include optional ConnectivityMonitoring object in the build" ON)
if (OBJ_O_4_CONNECTIVITY_MONITORING)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_4_CONNECTIVITY_MONITORING)
endif()
option(OBJ_O_2_LWM2M_ACCESS_CONTROL "Include optional Lwm2mAccessControl object in the build" ON)
if (OBJ_O_2_LWM2M_ACCESS_CONTROL)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_2_LWM2M_ACCESS_CONTROL)
endif()
option(OBJ_O_3339_AUDIO_CLIP "Include optional AudioClip object in the build" ON)
if (OBJ_O_3339_AUDIO_CLIP)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_3339_AUDIO_CLIP)
endif()
option(OBJ_O_5_FIRMWARE_UPDATE "Include optional FirmwareUpdate object in the build" ON)
if (OBJ_O_5_FIRMWARE_UPDATE)
	set(WPP_DEFINITIONS ${WPP_DEFINITIONS} OBJ_O_5_FIRMWARE_UPDATE)
endif()
# ---------- Optional objects config block end ----------

# Share defines with target
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} PARENT_SCOPE)
