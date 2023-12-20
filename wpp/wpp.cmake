# This file is used to build wpp library as part of other project
# It is not used to build wpp as standalone library, compilation
# options and configs must be set in parent project

set(WPP_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

function(target_link_wpp target)
    # Include file with wakaama core config
    include(${WPP_DIRECTORY}/wakaama.cmake)

    # Add subdiretories with sources
    add_subdirectory(${WPP_DIRECTORY}/client client)
    add_subdirectory(${WPP_DIRECTORY}/registry registry)
    add_subdirectory(${WPP_DIRECTORY}/platform platform)
    add_subdirectory(${WPP_DIRECTORY}/utils utils)

    # message(STATUS "Wpp:WPP_INCLUDES: ${WPP_INCLUDES}")
    # message(STATUS "Wpp:WPP_SOURCES: ${WPP_SOURCES}")
    # message(STATUS "Wpp:WPP_DEFINITIONS: ${WPP_DEFINITIONS}")

    # Link Wakaama that implements core functionality to Wpp
    target_link_wakaama(${target})
    # Add wakama core configs
    target_compile_definitions(${target} PUBLIC ${WPP_DEFINITIONS})
    # Include wakaama core sources
    target_sources(${target} PUBLIC ${WPP_SOURCES})
    # Make liblwm2m.h visible for users of target
    target_include_directories(${target} PUBLIC ${WPP_INCLUDES})
endfunction()