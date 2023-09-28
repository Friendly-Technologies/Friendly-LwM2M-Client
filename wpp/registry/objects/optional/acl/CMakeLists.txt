if(OPTIONAL_ACL_OBJ)
    # Update WPP_SOURCES variable from parent scope.
    set(WPP_SOURCES ${WPP_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Acl.cpp PARENT_SCOPE)
endif()