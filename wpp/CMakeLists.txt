# Project's name
project(Wpp CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include file with compiler config
include(config/compiler_config.cmake)

# Variables that will be filled in subdirectories
set(WPP_SOURCES "")
set(WPP_INCLUDES "")
set(WPP_DEFINITIONS "")

# Include file with wpp config
include(config/wpp_config.cmake)
include(config/objects_config.cmake)
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