# This script gets a list of all files and directories in
# the current source directory, then for each one, if it is
# a directory, it adds it as a subdirectory.
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
        add_subdirectory(${child})
    endif()
endforeach()

# Save WPP_SOURCES variable value from subdirectories into
# the WPP_SOURCES variable of parent scope.
set(WPP_SOURCES ${WPP_SOURCES} PARENT_SCOPE)
# Update WPP_INCLUDES variable from parent scope.
set(WPP_INCLUDES ${WPP_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)