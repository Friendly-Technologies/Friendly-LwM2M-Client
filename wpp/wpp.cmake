# This file is used to build wpp library as part of other project
# It is not used to build wpp as standalone library, compilation
# options must be set in parent project

set(WPP_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

function(target_link_wpp target)
    # Variables that will be filled in subdirectories
    set(WPP_SOURCES "")
    set(WPP_INCLUDES "")
    set(WPP_DEFINITIONS "")

    # Include file with wpp config
    include(${WPP_DIRECTORY}/configs/wpp_config.cmake)
    include(${WPP_DIRECTORY}/configs/objects_config.cmake)
    # Include file with wakaama core config
    include(${WPP_DIRECTORY}/../wakaama_core/WakaamaCore.cmake)

    # Add subdiretories with sources
    add_subdirectory(${WPP_DIRECTORY}/client client)
    add_subdirectory(${WPP_DIRECTORY}/registry registry)
    add_subdirectory(${WPP_DIRECTORY}/platform platform)
    add_subdirectory(${WPP_DIRECTORY}/utils utils)

    # message(STATUS "Wpp:WPP_INCLUDES: ${WPP_INCLUDES}")
    # message(STATUS "Wpp:WPP_SOURCES: ${WPP_SOURCES}")
    # message(STATUS "Wpp:WPP_DEFINITIONS: ${WPP_DEFINITIONS}")

    # Link Wakaama that implements core functionality to Wpp
    target_link_wakaama_core(${target})
    # Add wakama core configs
    target_compile_definitions(${target} PUBLIC ${WPP_DEFINITIONS})
    # Include wakaama core sources
    target_sources(${target} PUBLIC ${WPP_SOURCES})
    # Make liblwm2m.h visible for users of target
    target_include_directories(${target} PUBLIC ${WPP_INCLUDES})
endfunction()