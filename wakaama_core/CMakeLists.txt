# Project's name
project(WakaamaCore C)

set(COMPILE_DEFINITIONS "")

# Include file with wpp config
include(config/config.cmake)
# Include file with wpp config
include(../2305-Wakaama/wakaama.cmake)

# Creating target as static library
add_library(WakaamaCore STATIC)
# Add wakama core configs
target_compile_definitions(WakaamaCore PUBLIC LWM2M_CLIENT_MODE ${COMPILE_DEFINITIONS})
# Include wakaama core sources
target_sources_wakaama(WakaamaCore)
# Make liblwm2m.h visible for users of WakaamaCore lib
target_include_directories(WakaamaCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../2305-Wakaama/include)