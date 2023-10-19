if(MANDATORY_SERVER_OBJ)
    # Update SOURCES variable from parent scope.
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp PARENT_SCOPE)
endif()