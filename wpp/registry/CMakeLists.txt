add_subdirectory(data_model)
add_subdirectory(objects)

set(WPP_SOURCES ${WPP_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/WppRegistry.cpp)
# Save WPP_SOURCES variable value from subdirectories into
# the WPP_SOURCES variable of parent scope.
set(WPP_SOURCES ${WPP_SOURCES} PARENT_SCOPE)
# Update WPP_INCLUDES variable from parent scope.
set(WPP_INCLUDES ${WPP_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)