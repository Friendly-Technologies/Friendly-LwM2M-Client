# Log configs
option(ENABLE_LOGS "Enable logs for Wakaama and WakaamaPlus" ON)
if (ENABLE_LOGS)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} ENABLE_LOGS=1)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} LWM2M_WITH_LOGS=1)
endif()

# Optional object configs
option(ACL_OBJ "Include ACL object in the build" OFF)
if (ACL_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} ACL_OBJ=1)
endif()

option(CONN_MONITORING_OBJ "Include Connection monitoring object in the build" OFF)
if (CONN_MONITORING_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} CONN_MONITORING_OBJ=1)
endif()

option(FIRMWARE_UPD_OBJ "Include Firmware update in the build" OFF)
if (FIRMWARE_UPD_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} FIRMWARE_UPD_OBJ=1)
endif()

# Share defines with target
set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} PARENT_SCOPE)