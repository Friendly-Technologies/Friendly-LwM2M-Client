# This file is used to build wpp library as part of other project
# It is not used to build wpp as standalone library, compilation
# options and configs must be set in parent project

set(WPP_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

function(target_link_wpp target)
    # Variables that will be filled in subdirectories
    set(WPP_SOURCES "")
    set(WPP_INCLUDES "")

    # Include file with wakaama core config
    include(${WPP_DIRECTORY}/wakaama.cmake)

    # Link wakaama that implements core functionality to Wpp
    target_link_wakaama(${target})

    # Add subdiretories with sources
    add_subdirectory(${WPP_DIRECTORY}/client client)
    add_subdirectory(${WPP_DIRECTORY}/registry registry)
    add_subdirectory(${WPP_DIRECTORY}/platform platform)
    add_subdirectory(${WPP_DIRECTORY}/utils utils)

    # Include wpp sources
    target_sources(${target} PUBLIC ${WPP_SOURCES})
    # Include wpp headers
    target_include_directories(${target} PUBLIC ${WPP_INCLUDES})

    # message(STATUS "Wpp:WPP_INCLUDES: ${WPP_INCLUDES}")
    # message(STATUS "Wpp:WPP_SOURCES: ${WPP_SOURCES}")
endfunction()