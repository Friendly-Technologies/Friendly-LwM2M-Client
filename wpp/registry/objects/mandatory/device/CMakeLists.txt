if(MANDATORY_DEVICE_OBJ)
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Device.cpp)
    
    # --------------- CMake block 0 start ---------------
    # --------------- CMake block 0 end ---------------

    # Update SOURCES variable from parent scope.
    set(SOURCES ${SOURCES} PARENT_SCOPE)
endif()
