# This CMakeLists.txt file is used to build Wpp as static library
# with predefined configs in the configs directory

# Project's name
project(Wpp CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include file with compiler config
include(configs/compiler_config.cmake)
# Include file with Wakaama core config
include(configs/wakaama_config.cmake)
# Include file with wpp config
include(configs/wpp_config.cmake)
# Include file with wpp object configs
include(configs/objects_config.cmake)
# Include file with Wpp sources
include(wpp.cmake)

# Creating target as static library
add_library(Wpp STATIC)
# Set wpp properties
target_compile_definitions(Wpp PUBLIC ${WPP_DEFINITIONS})
# Link Wpp sources
target_link_wpp(Wpp)