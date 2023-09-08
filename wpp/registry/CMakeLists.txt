add_subdirectory(data_model)
add_subdirectory(objects)

# Save SOURCES variable value from subdirectories into
# the SOURCES variable of parent scope.
set(SOURCES ${SOURCES} PARENT_SCOPE)