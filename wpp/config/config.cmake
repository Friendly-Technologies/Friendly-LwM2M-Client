# Options that define here determine availability of general components
# Settings for the behavior of a particular component should be in a
# separate .h file near the definition of the component itself.

# Loging start
option(ENABLE_LOGS "Enable logs for Wakaama and WakaamaPlus" ON)
if (ENABLE_LOGS)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} ENABLE_LOGS=1)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} LWM2M_WITH_LOGS=1)
endif()
# Loging end

# Mandatory objects start
# Mandatory objects end

# Optional objects start
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
# Optional objects end

# Share defines with target
set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} PARENT_SCOPE)