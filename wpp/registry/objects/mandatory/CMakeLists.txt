# This script gets a list of all files and directories in
# the current source directory, then for each one, if it is
# a directory, it adds it as a subdirectory.
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
        add_subdirectory(${child})
    endif()
endforeach()

# Save SOURCES variable value from subdirectories into
# the SOURCES variable of parent scope.
set(SOURCES ${SOURCES} PARENT_SCOPE)