if(ACL_OBJ)
    # Update SOURCES variable from parent scope.
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Acl.cpp PARENT_SCOPE)
    # Set compile definition 
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} ACL_OBJ=1 PARENT_SCOPE)
endif()