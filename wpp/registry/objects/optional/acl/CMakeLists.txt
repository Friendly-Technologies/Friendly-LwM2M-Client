if(ACL_OBJ)
    # Update SOURCES variable from parent scope.
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Acl.cpp PARENT_SCOPE)
endif()