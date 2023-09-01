add_subdirectory(mandatory)
add_subdirectory(optional)

# Save SOURCES variable value from subdirectories into
# the SOURCES variable of parent scope.
set(SOURCES ${SOURCES} PARENT_SCOPE)
# Update INCLUDES variable from parent scope.
set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)