# This file is used to build wakaama core library as part of other project
# It is not used to build wakaama as standalone library, compilation
# options and configs must be set in parent project

set(WAKAAMA_CORE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

function(target_link_wakaama target)
    # Include file with wakaama sources config
    include(${WAKAAMA_CORE_DIRECTORY}/../2305-Wakaama/wakaama.cmake)

    # Add wakama core configs
    target_compile_definitions(${target} PUBLIC LWM2M_CLIENT_MODE)
    # Include wakaama core sources
    target_sources_wakaama(${target})
    # Make liblwm2m.h visible for users of target
    target_include_directories(${target} PUBLIC ${WAKAAMA_CORE_DIRECTORY}/../2305-Wakaama/include)
endfunction()
