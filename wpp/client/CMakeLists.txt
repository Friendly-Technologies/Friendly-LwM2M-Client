#Add subdiretories with sources
add_subdirectory(platform)
add_subdirectory(connection)
add_subdirectory(bindings)

# Update SOURCES variable from parent scope.
set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/WppClient.cpp PARENT_SCOPE)
# Update INCLUDES variable from parent scope.
set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)
