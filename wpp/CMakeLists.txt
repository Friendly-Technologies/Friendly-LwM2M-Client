# Project's name
project(WppLib CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Position-independent code is code that can execute
# properly regardless of where in memory it is located.
# This is essential for shared libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Compiler options
add_compile_options(
    -Waggregate-return
    -Wall
    -Wcast-align
    -Wextra
    -Wfloat-equal
    -Wpointer-arith
    -Wshadow
    -Wswitch-default
    -Wwrite-strings
    -pedantic

    # Reduce noise: Unused parameters are common in this ifdef-littered code-base, but of no danger
    -Wno-unused-parameter
    # Reduce noise: Too many false positives
    -Wno-uninitialized
    # TODO: Remove this two configs after integreting with wakaama
    -Wno-unused-but-set-variable
    -Wno-unused-variable

     # Turn (most) warnings into errors
    -Werror
    # Disabled because of existing, non-trivially fixable code
    -Wno-error=cast-align
)

# Variables that will be filled in subdirectories
set(SOURCES "")
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMPILE_DEFINITIONS "")

# Include file with wpp config
include(config/config.cmake)

# TODO: Delete this src file
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/dep.cpp)

# Add subdiretories with sources
add_subdirectory(client)
add_subdirectory(registry)
add_subdirectory(platform)
add_subdirectory(connection)
add_subdirectory(utils)

# message("INCLUDES: ${INCLUDES}")
# message("SOURCES: ${SOURCES}")
# message("COMPILE_DEFINITIONS: ${COMPILE_DEFINITIONS}")

# Creating target as static library
add_library(WppLib STATIC ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WppLib PUBLIC ${INCLUDES})
# List of preprocessor defines that will be set for compiler with key -D
target_compile_definitions(WppLib PUBLIC ${COMPILE_DEFINITIONS})
