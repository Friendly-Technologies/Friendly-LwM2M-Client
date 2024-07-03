# This CMakeLists.txt file is used to build Wpp as static library
# with predefined configs in the configs directory but user can use
# build wpp as purt of own target with own configs by including wpp.cmake,
# in this case user can set wpp properties through target_compile_definitions.

# Project's name
project(Wpp CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

set(WPP_DEFINITIONS "")

# Include file with compiler config
include(configs/compiler_config.cmake)
# Include file with Wakaama core config
include(configs/wpp_config.cmake)
# Include file with Wpp sources
include(wpp.cmake)

# Build options
option(WPP_BUILD_WITH_EXCEPTIONS "Enable support of Exceptions" OFF)
option(WPP_BUILD_WITH_RTTI "Enable support of RTTI" OFF)
option(WPP_BUILD_FOR_64_BIT "Build for 64-bit system or 32-bit" ON)
option(WPP_BUILD_INDEPENDENT_CODE "Build as independent code" ON)

# Creating target as static library
add_library(Wpp STATIC)
# Add Wpp compiler flags
wpp_setup_compiler(Wpp ${WPP_BUILD_WITH_EXCEPTIONS} ${WPP_BUILD_WITH_RTTI} ${WPP_BUILD_FOR_64_BIT} ${WPP_BUILD_INDEPENDENT_CODE})
# Set wpp properties
target_compile_definitions(Wpp PUBLIC ${WPP_DEFINITIONS})
# Link Wpp sources
target_link_wpp(Wpp wpp)

# Custom command to generate libWpp.zip
add_custom_command(
    TARGET Wpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DARGV0="${WPP_DEFINITIONS}" -DARGV1="${CMAKE_CURRENT_BINARY_DIR}/wpp_definitions.cmake" -P ${CMAKE_CURRENT_SOURCE_DIR}/build_utils/wpp_definitions_gen.cmake
    COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/build_utils/wpp_headers_gen.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/headers
    COMMAND zip -r libWpp.zip  headers/* libWpp.a wpp_definitions.cmake
    COMMENT "Generating libWpp.zip"
)
