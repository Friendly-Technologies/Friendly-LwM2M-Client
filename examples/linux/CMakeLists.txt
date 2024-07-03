# Project's name
project(WppLinuxExample CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Clear the list of preprocessor defines
set(WPP_DEFINITIONS "")

# Include file with compiler config
include(compiler_config.cmake)
# Include file with wpp config
include(${CMAKE_SOURCE_DIR}/wpp/configs/wpp_config.cmake)
# Add file with wpp lib configs
include(${CMAKE_SOURCE_DIR}/wpp/wpp.cmake)

# Find the libcurl package
find_package(CURL REQUIRED)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Variables that will be filled in subdirectories
set(SOURCES main.cpp objects.cpp)
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/)

# General options
set(LINUX_WITH_UDP_DTLS ON)

# Setup connection security
if (LINUX_WITH_UDP_DTLS)
    message(STATUS "WppLinuxExample: UDP DTLS is enabled")
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} WITH_UDP_DTLS)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} DTLS_WITH_PSK)
    # set(WPP_DEFINITIONS ${WPP_DEFINITIONS} DTLS_WITH_RPK)
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/shared/platform/connection_dtls connection_dtls)
else()
    message(STATUS "WppLinuxExample: UDP DTLS is disabled")
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/shared/platform/connection connection)
endif()

add_executable(WppLinuxExample)
# Setup compiler flags
setup_compiler(WppLinuxExample)
# List of preprocessor defines that will be set for compiler with key -D
target_compile_definitions(WppLinuxExample PUBLIC ${WPP_DEFINITIONS})
# Link wpp sources to tests
target_link_wpp(WppLinuxExample wpp)
# Link with Wpp library
target_link_libraries(WppLinuxExample pthread)
# Add subdiretories with sources
add_subdirectory(${CMAKE_SOURCE_DIR}/examples/shared/platform linux_platform)
check_target_definition(WppLinuxExample "OBJ_O_5_FIRMWARE_UPDATE" is_defined)
if(is_defined)
    message(STATUS "WppLinuxExample: Firmware update is enabled ${CMAKE_CURRENT_SOURCE_DIR}")
    # Build libcoap
    execute_process(
        COMMAND bash libcoap_build.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE libcoap_path
        ERROR_VARIABLE error
    )
    if(NOT result STREQUAL "0")
        message(FATAL_ERROR "WppLinuxExample: Failed to build libcoap: ${error}")
    else()
        message(STATUS "WppLinuxExample: libcoap built successfully. Library path: ${libcoap_path}")
    endif()
    # Add libcoap includes to the project
    target_include_directories(WppLinuxExample PUBLIC ${libcoap_path}/include/)
    # Add libcoap to the project
    target_link_libraries(WppLinuxExample ${libcoap_path}/libcoap-3.so CURL::libcurl OpenSSL::SSL OpenSSL::Crypto)
    # Add FirmwareUpdater to the project
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/shared/FirmwareUpdater linux_fw_upd)
endif()

# Include sources
target_sources(WppLinuxExample PUBLIC ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WppLinuxExample PUBLIC ${INCLUDES})

# Add tinydtls src
if (LINUX_WITH_UDP_DTLS)
    include(${CMAKE_SOURCE_DIR}/2305-Wakaama/examples/shared/tinydtls.cmake)
    target_sources_tinydtls(WppLinuxExample)
endif()

# message(STATUS "WppLinuxExample:INCLUDES: ${INCLUDES}")
# message(STATUS "WppLinuxExample:SOURCES: ${SOURCES}")
# message(STATUS "WppLinuxExample:COMPILE_DEFINITIONS: ${COMPILE_DEFINITIONS}")

# Generate a .bin file 
add_custom_command(TARGET WppLinuxExample POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:WppLinuxExample> $<TARGET_FILE_DIR:WppLinuxExample>/WppLinuxExample.bin
                   COMMENT "WppLinuxExample: Creating binary file")
# Generate a .hex file 
add_custom_command(TARGET WppLinuxExample POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:WppLinuxExample> $<TARGET_FILE_DIR:WppLinuxExample>/WppLinuxExample.hex
                   COMMENT "WppLinuxExample: Creating hex file")
# Get information about size compiled program
add_custom_command(TARGET WppLinuxExample POST_BUILD
                   COMMAND size $<TARGET_FILE:WppLinuxExample>
                   COMMENT "WppLinuxExample: Compiled program size")