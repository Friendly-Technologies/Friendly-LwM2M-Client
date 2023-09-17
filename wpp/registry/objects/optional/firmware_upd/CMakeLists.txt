if(OPTIONAL_FIRMWARE_UPD_OBJ)
    # Update SOURCES variable from parent scope.
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/FirmwareUpd.cpp PARENT_SCOPE)
endif()