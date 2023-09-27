# Project's name
project(Wpp CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Position-independent code is code that can execute
# properly regardless of where in memory it is located.
# This is essential for shared libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Comiler options
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
     # Turn (most) warnings into errors
    -Werror
    # Disabled because of existing, non-trivially fixable code
    -Wno-error=cast-align
)

# Variables that will be filled in subdirectories
set(WPP_SOURCES "")
set(WPP_INCLUDES "")
set(WPP_DEFINITIONS "")

# Include file with wpp config
include(config/config.cmake)
# Include file with wakaama core config
include(../wakaama_core/WakaamaCore.cmake)

# Add subdiretories with sources
add_subdirectory(client)
add_subdirectory(registry)
add_subdirectory(platform)
add_subdirectory(utils)

# message(STATUS "Wpp:WPP_INCLUDES: ${WPP_INCLUDES}")
# message(STATUS "Wpp:WPP_SOURCES: ${WPP_SOURCES}")
# message(STATUS "Wpp:WPP_DEFINITIONS: ${WPP_DEFINITIONS}")

# Creating target as static library
add_library(Wpp STATIC ${WPP_SOURCES})
# Link Wakaama that implements core functionality to Wpp
target_link_wakaama_core(Wpp)
# List of directories that will be set for compiler with key -I
target_include_directories(Wpp PUBLIC ${WPP_INCLUDES})
# List of preprocessor defines that will be set for compiler with key -D
target_compile_definitions(Wpp PUBLIC ${WPP_DEFINITIONS})