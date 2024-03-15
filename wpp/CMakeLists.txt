# This CMakeLists.txt file is used to build Wpp as static library
# with predefined configs in the configs directory

# Project's name
project(Wpp CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include file with compiler config
include(configs/compiler_config.cmake)
# Include file with Wpp configs
include(wpp.cmake)

# Creating target as static library
add_library(Wpp STATIC ${WPP_SOURCES})
# Link Wpp sources
target_link_wpp(Wpp)