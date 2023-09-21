# Project's name
project(WakaamaPlusTest CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

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

     # Turn (most) warnings into errors
    -Werror
    # Disabled because of existing, non-trivially fixable code
    -Wno-error=cast-align
)

# Variables that will be filled in subdirectories
set(SOURCES main.cpp)
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMPILE_DEFINITIONS "")

set(SOURCES main.cpp)

# Add subdiretories with sources
add_subdirectory(connection)
add_subdirectory(platform)

add_executable(WakaamaPlusTest ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WakaamaPlusTest PUBLIC ${INCLUDES})

target_link_libraries(WakaamaPlusTest Wpp)
