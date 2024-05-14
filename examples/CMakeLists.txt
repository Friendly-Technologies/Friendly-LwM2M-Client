# Project's name
project(WakaamaPlusExample CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include file with compiler config
include(../wpp/configs/compiler_config.cmake)
# Include file with wpp config
include(../wpp/configs/wpp_config.cmake)
# Add file with wpp lib configs
include(../wpp/wpp.cmake)

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

# Build libcoap
execute_process(
    COMMAND bash libcoap_build.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/coap
    RESULT_VARIABLE result
    OUTPUT_VARIABLE libcoap_path
    ERROR_VARIABLE error
)
if(NOT result STREQUAL "0")
    message(FATAL_ERROR "Failed to build libcoap: ${error}")
else()
    message(STATUS "libcoap built successfully. Library path: ${libcoap_path}")
    # Move libcoap_path/lib to build directory
    file(COPY ${libcoap_path}/lib DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Variables that will be filled in subdirectories
set(SOURCES main.cpp objects.cpp)
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/)

# General options
option(WITH_UDP_DTLS "Enable UDP DTLS" OFF)
if (WITH_UDP_DTLS)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} WITH_UDP_DTLS)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} DTLS_WITH_PSK)
    # set(WPP_DEFINITIONS ${WPP_DEFINITIONS} DTLS_WITH_RPK)
endif()

add_executable(WakaamaPlusExample)
# List of preprocessor defines that will be set for compiler with key -D
target_compile_definitions(WakaamaPlusExample PUBLIC ${WPP_DEFINITIONS})
# Link wpp sources to tests
target_link_wpp(WakaamaPlusExample wpp)
# Link with Wpp library
target_link_libraries(WakaamaPlusExample Wpp pthread ${libcoap_path}/libcoap-3.so ${CURL_LIBRARIES})

# Add subdiretories with sources
add_subdirectory(platform)
check_target_definition(WakaamaPlusExample "OBJ_O_5_FIRMWARE_UPDATE" is_defined)
if(is_defined)
    add_subdirectory(FirmwareUpdater)
endif()

# Include sources
target_sources(WakaamaPlusExample PUBLIC ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WakaamaPlusExample PUBLIC ${INCLUDES} ${libcoap_path}/include/)

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