# Project's name
project(WppLib CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Position-independent code is code that can execute
# properly regardless of where in memory it is located.
# This is essential for shared libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Include file with wpp config
include(config/config.cmake)

# Variables that will be filled in subdirectories
set(SOURCES "")
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMPILE_DEFINITIONS "")

# TODO: Delete this src file
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/dep.cpp)

# Add subdiretories with sources
add_subdirectory(platform)
add_subdirectory(connection)
add_subdirectory(core)
add_subdirectory(registry)

# message("INCLUDES: ${INCLUDES}")
# message("SOURCES: ${SOURCES}")
# message("COMPILE_DEFINITIONS: ${COMPILE_DEFINITIONS}")

# Creating target as static library
add_library(WppLib STATIC ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WppLib PUBLIC ${INCLUDES})
# List of directories that will be set for compiler with key -I
target_compile_definitions(WppLib PUBLIC ${COMPILE_DEFINITIONS})
