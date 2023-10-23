set(WAKAAMA_CORE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

function(target_link_wakaama_core target)
    # Include file with Wakaama core config
    include(${WAKAAMA_CORE_DIRECTORY}/config/config.cmake)
    # Include file with wakaama sources config
    include(${WAKAAMA_CORE_DIRECTORY}/../2305-Wakaama/wakaama.cmake)

    # message(STATUS "WakaamaCore:WAKAAMA_CORE_DEFINITIONS: ${WAKAAMA_CORE_DEFINITIONS}")

    # Add wakama core configs
    target_compile_definitions(${target} PUBLIC LWM2M_CLIENT_MODE ${WAKAAMA_CORE_DEFINITIONS})
    # Include wakaama core sources
    target_sources_wakaama(${target})
    # Make liblwm2m.h visible for users of target
    target_include_directories(${target} PUBLIC ${WAKAAMA_CORE_DIRECTORY}/../2305-Wakaama/include)
endfunction()
