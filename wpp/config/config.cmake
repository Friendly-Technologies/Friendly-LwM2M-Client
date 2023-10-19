# Options that define here determine availability of general components
# Settings for the behavior of a particular component should be in a
# separate .h file near the definition of the component itself.

# Loging start
option(ENABLE_LOGS "Enable logs for WakaamaPlus" ON)
if (ENABLE_LOGS)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} ENABLE_LOGS=1)
endif()
# Loging end

# Mandatory object option starts with MANDATORY_
# Mandatory objects start
option(MANDATORY_SECURITY_OBJ "Include mandatory Security object in the build" ON)
if (MANDATORY_SECURITY_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} MANDATORY_SECURITY_OBJ=1)
endif()

option(MANDATORY_SERVER_OBJ "Include mandatory Server object in the build" ON)
if (MANDATORY_SERVER_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} MANDATORY_SERVER_OBJ=1)
endif()

option(MANDATORY_DEVICE_OBJ "Include mandatory Device object in the build" ON)
if (MANDATORY_DEVICE_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} MANDATORY_DEVICE_OBJ=1)
endif()
# Mandatory objects end

# Optional object option starts with OPTIONAL_
# Optional objects start
option(OPTIONAL_ACL_OBJ "Include optional ACL object in the build" OFF)
if (ACL_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} OPTIONAL_ACL_OBJ=1)
endif()

option(OPTIONAL_CONN_MONITORING_OBJ "Include optional Connection monitoring object in the build" OFF)
if (OPTIONAL_CONN_MONITORING_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} OPTIONAL_CONN_MONITORING_OBJ=1)
endif()

option(OPTIONAL_FIRMWARE_UPD_OBJ "Include optional Firmware update in the build" OFF)
if (OPTIONAL_FIRMWARE_UPD_OBJ)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} OPTIONAL_FIRMWARE_UPD_OBJ=1)
endif()
# Optional objects end

# Share defines with target
set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} PARENT_SCOPE)