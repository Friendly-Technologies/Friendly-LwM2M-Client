if (WITH_UDP_DTLS)
    add_subdirectory(connection_dtls)
else()
    add_subdirectory(connection)
endif()

# Update SOURCES variable from parent scope.
set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Platform.cpp PARENT_SCOPE)
# Update INCLUDES variable from parent scope.
set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)