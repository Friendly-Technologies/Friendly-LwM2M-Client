# Project's name
project(WakaamaPlusExample CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include file with compiler config
include(../wpp/configs/compiler_config.cmake)

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

# Find the libcurl package
find_package(CURL REQUIRED)
# Include directories
include_directories(${CURL_INCLUDE_DIR})

# Variables that will be filled in subdirectories
set(SOURCES main.cpp)
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMPILE_DEFINITIONS "")

# General options
option(WITH_UDP_DTLS "Enable UDP DTLS" ON)
if (WITH_UDP_DTLS)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} WITH_UDP_DTLS)
    set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} DTLS_WITH_PSK)
    # set(COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} DTLS_WITH_RPK)
endif()

add_executable(WakaamaPlusExample)
# Link with Wpp library
target_link_libraries(WakaamaPlusExample Wpp pthread ${CURL_LIBRARIES})
# List of preprocessor defines that will be set for compiler with key -D
target_compile_definitions(WakaamaPlusExample PUBLIC ${COMPILE_DEFINITIONS})

# Add subdiretories with sources
add_subdirectory(platform)
add_subdirectory(Lwm2mServer)
add_subdirectory(Lwm2mSecurity)
add_subdirectory(Device)
if(OBJ_O_4_CONNECTIVITY_MONITORING_V13)
    add_subdirectory(ConnectivityMonitoring)
endif()
if(OBJ_O_2_LWM2M_ACCESS_CONTROL_V11)
    add_subdirectory(Lwm2mAccessControl)
endif()
if(OBJ_O_5_FIRMWARE_UPDATE_V11)
    add_subdirectory(FirmwareUpdate)
endif()

# Include sources
target_sources(WakaamaPlusExample PUBLIC ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WakaamaPlusExample PUBLIC ${INCLUDES})

# Add tiny dtls src
if (WITH_UDP_DTLS)
    include(../2305-Wakaama/examples/shared/tinydtls.cmake)
    target_sources_tinydtls(WakaamaPlusExample)
endif()

# message(STATUS "WakaamaPlusExample:INCLUDES: ${INCLUDES}")
# message(STATUS "WakaamaPlusExample:SOURCES: ${SOURCES}")
# message(STATUS "WakaamaPlusExample:COMPILE_DEFINITIONS: ${COMPILE_DEFINITIONS}")

# Generate a .bin file 
add_custom_command(TARGET WakaamaPlusExample POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:WakaamaPlusExample> $<TARGET_FILE_DIR:WakaamaPlusExample>/WakaamaPlusExample.bin
                   COMMENT "Creating binary file")
# Generate a .hex file 
add_custom_command(TARGET WakaamaPlusExample POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:WakaamaPlusExample> $<TARGET_FILE_DIR:WakaamaPlusExample>/WakaamaPlusExample.hex
                   COMMENT "Creating hex file")
# Get information about size compiled program
add_custom_command(TARGET WakaamaPlusExample POST_BUILD
                   COMMAND size $<TARGET_FILE:WakaamaPlusExample>
                   COMMENT "Compiled program size")