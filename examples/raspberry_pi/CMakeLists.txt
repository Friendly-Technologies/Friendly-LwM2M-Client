# Project's name
project(WppRPiExample CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# TODO: It is bad approach to use CMAKE_CXX_FLAGS, but it is necessary for now
# Set the compiler target architecture
set(CMAKE_C_COMPILER_TARGET aarch64-linux-gnu)
set(CMAKE_CXX_COMPILER_TARGET aarch64-linux-gnu)

# Clear the list of preprocessor defines
set(WPP_DEFINITIONS "")

# Include file with compiler config
include(compiler_config.cmake)
# Include file with wpp config
include(wpp_config.cmake)
# Add file with wpp lib configs
include(${CMAKE_SOURCE_DIR}/wpp/wpp.cmake)

# Variables that will be filled in subdirectories
set(RPI_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/rpi_libs)
set(SOURCES main.cpp objects.cpp ${CMAKE_CURRENT_SOURCE_DIR}/drivers/gpio.cpp ${CMAKE_CURRENT_SOURCE_DIR}/drivers/mki217.cpp)
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/ ${RPI_LIBRARY_PATH}/includes ${CMAKE_CURRENT_SOURCE_DIR}/drivers)

# General options
set(RPI_WITH_UDP_DTLS ON)

# Setup connection security
if (RPI_WITH_UDP_DTLS)
    message(STATUS "WppRPiExample: UDP DTLS is enabled")
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} WITH_UDP_DTLS)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} DTLS_WITH_PSK)
    # set(WPP_DEFINITIONS ${WPP_DEFINITIONS} DTLS_WITH_RPK)
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/shared/platform/connection_dtls connection_dtls)
else()
    message(STATUS "WppRPiExample: UDP DTLS is disabled")
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/shared/platform/connection connection)
endif()

add_executable(WppRPiExample)
# Set compiler flags
setup_compiler(WppRPiExample)
# List of preprocessor defines that will be set for compiler with key -D
target_compile_definitions(WppRPiExample PUBLIC ${WPP_DEFINITIONS})
# Link wpp sources to tests
target_link_wpp(WppRPiExample wpp)
# Link with Wpp library
target_link_libraries(WppRPiExample pthread)
# Set the path to the libraries)
link_directories(${RPI_LIBRARY_PATH})
# Add subdiretories with sources
add_subdirectory(${CMAKE_SOURCE_DIR}/examples/shared/platform linux_platform)

check_target_definition(WppRPiExample "OBJ_O_5_FIRMWARE_UPDATE" is_defined)
if(is_defined)
    message(STATUS "WppRPiExample: Firmware update is enabled ${CMAKE_CURRENT_SOURCE_DIR}")
    # Build libcoap
    execute_process(
        COMMAND bash libcoap_build.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE libcoap_path
        ERROR_VARIABLE error
    )
    if(NOT result STREQUAL "0")
        message(FATAL_ERROR "WppRPiExample: Failed to build libcoap: ${error}")
    else()
        message(STATUS "WppRPiExample: libcoap built successfully. Library path: ${libcoap_path}")
    endif()
    # Add libcoap includes to the project
    target_include_directories(WppLinuxExample PUBLIC ${libcoap_path}/include/)
    # Add libcoap to the project
    target_link_libraries(WppRPiExample ${libcoap_path}/libcoap-3.a ${RPI_LIBRARY_PATH}/libcurl.so.4)

    # Add FirmwareUpdater to the project
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/shared/FirmwareUpdater linux_fw_upd)
endif()

# Link the GPIOD library
target_link_libraries(WppRPiExample ${RPI_LIBRARY_PATH}/libgpiod.so)

# Include sources
target_sources(WppRPiExample PUBLIC ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WppRPiExample PUBLIC ${INCLUDES} ${libcoap_path}/include/)

# Add tinydtls src
include(${CMAKE_SOURCE_DIR}/2305-Wakaama/examples/shared/tinydtls.cmake)
target_sources_tinydtls(WppRPiExample)

# message(STATUS "WppRPiExample:INCLUDES: ${INCLUDES}")
# message(STATUS "WppRPiExample:SOURCES: ${SOURCES}")
# message(STATUS "WppRPiExample:COMPILE_DEFINITIONS: ${COMPILE_DEFINITIONS}")

# Generate a .bin file 
add_custom_command(TARGET WppRPiExample POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:WppRPiExample> $<TARGET_FILE_DIR:WppRPiExample>/WppRPiExample.bin
                   COMMENT "WppRPiExample: Creating binary file")
# Generate a .hex file 
add_custom_command(TARGET WppRPiExample POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:WppRPiExample> $<TARGET_FILE_DIR:WppRPiExample>/WppRPiExample.hex
                   COMMENT "WppRPiExample: Creating hex file")
# Get information about size compiled program
add_custom_command(TARGET WppRPiExample POST_BUILD
                   COMMAND size $<TARGET_FILE:WppRPiExample>
                   COMMENT "WppRPiExample: Compiled program size")