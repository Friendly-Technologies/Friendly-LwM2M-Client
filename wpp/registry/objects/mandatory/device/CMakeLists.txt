if(MANDATORY_DEVICE_OBJ)
    set(WPP_SOURCES ${WPP_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Device.cpp)
    
    # --------------- CMake block 0 start ---------------
    # --------------- CMake block 0 end ---------------

    # Update WPP_SOURCES variable from parent scope.
    set(WPP_SOURCES ${WPP_SOURCES} PARENT_SCOPE)
endif()
