# Update WPP_SOURCES variable from parent scope.
set(WPP_SOURCES ${WPP_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/WppBindings.cpp PARENT_SCOPE)
# Update WPP_INCLUDES variable from parent scope.
set(WPP_INCLUDES ${WPP_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)