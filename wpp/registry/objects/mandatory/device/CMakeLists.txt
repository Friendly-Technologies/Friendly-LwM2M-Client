if(MANDATORY_DEVICE_OBJ)
    # Update SOURCES variable from parent scope.
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Device.cpp PARENT_SCOPE)
endif()