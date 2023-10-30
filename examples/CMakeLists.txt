# Project's name
project(WakaamaPlusTest CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include file with compiler config
include(../wpp/config/compiler_config.cmake)

# Compiler options
add_compile_options(
    # Disabled because Connection for dtls cast from 'sockaddr *' to 'sockaddr_in *' increases required alignment from 2 to 4
    -Wno-cast-align
    # Reduce noise: Too many false positives
    -Wno-uninitialized
    # Allows to compile tinydtls without errors
    -Wno-c99-extensions
)

# Generating .map file
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=output.map")

# Variables that will be filled in subdirectories
set(SOURCES main.cpp)
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMPILE_DEFINITIONS "")

# General options
option(WITH_UDP_DTLS "Enable UDP DTLS" OFF)
if (WITH_UDP_DTLS)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} WITH_UDP_DTLS)
    # set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} DTLS_WITH_PSK)
    # set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} DTLS_WITH_RPK)
endif()

# Add subdiretories with sources
add_subdirectory(platform)
add_subdirectory(server)
add_subdirectory(security)
add_subdirectory(device)

# message(STATUS "WakaamaPlusTest:INCLUDES: ${INCLUDES}")
# message(STATUS "WakaamaPlusTest:SOURCES: ${SOURCES}")
# message(STATUS "WakaamaPlusTest:COMPILE_DEFINITIONS: ${COMPILE_DEFINITIONS}")

add_executable(WakaamaPlusTest ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WakaamaPlusTest PUBLIC ${INCLUDES})
# List of preprocessor defines that will be set for compiler with key -D
target_compile_definitions(WakaamaPlusTest PUBLIC ${COMPILE_DEFINITIONS})

# Link with Wpp library
target_link_libraries(WakaamaPlusTest Wpp pthread)
# Add tiny dtls src
if (WITH_UDP_DTLS)
    include(../2305-Wakaama/examples/shared/tinydtls.cmake)
    target_sources_tinydtls(WakaamaPlusTest)
endif()

# Generate a .bin file 
add_custom_command(TARGET WakaamaPlusTest POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:WakaamaPlusTest> $<TARGET_FILE_DIR:WakaamaPlusTest>/WakaamaPlusTest.bin
                   COMMENT "Creating binary file")
# Generate a .hex file 
add_custom_command(TARGET WakaamaPlusTest POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:WakaamaPlusTest> $<TARGET_FILE_DIR:WakaamaPlusTest>/WakaamaPlusTest.hex
                   COMMENT "Creating hex file")
# Get information about size compiled program
add_custom_command(TARGET WakaamaPlusTest POST_BUILD
                   COMMAND size $<TARGET_FILE:WakaamaPlusTest>
                   COMMENT "Compiled program size")