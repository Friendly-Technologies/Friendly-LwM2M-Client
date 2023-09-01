if(FIRMWARE_UPD_OBJ)
    # Update SOURCES variable from parent scope.
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/FirmwareUpd.cpp PARENT_SCOPE)
    # Set compile definition 
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} FIRMWARE_UPD_OBJ=1 PARENT_SCOPE)
endif()