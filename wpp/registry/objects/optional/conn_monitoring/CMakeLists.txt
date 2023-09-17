if(OPTIONAL_CONN_MONITORING_OBJ)
    # Update SOURCES variable from parent scope.
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/ConnMonitoring.cpp PARENT_SCOPE)
endif()