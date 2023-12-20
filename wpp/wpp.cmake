# This file is used to build wpp library as part of other project
# It is not used to build wpp as standalone library, compilation
# options and configs must be set in parent project

set(WPP_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

# Function to check if a compile definition is set for WPP_TARGET
# Arguments:
#   - TARGET_NAME: The target to check the compile definitions on
#   - DEFINITION_TO_CHECK: The compile definition to look for
#   - RESULT_VAR: The variable to store the result (TRUE or FALSE)
function(check_target_definition TARGET DEFINITION_TO_CHECK RESULT_VAR)
    # Get the compile definitions for the target
    get_target_property(TARGET_COMPILE_DEFINITIONS ${TARGET} COMPILE_DEFINITIONS)

    # Initialize result variable
    set(${RESULT_VAR} FALSE PARENT_SCOPE)

    # Loop through each definition and check if the desired definition is set
    foreach(DEFINITION ${TARGET_COMPILE_DEFINITIONS})
        if(DEFINITION STREQUAL "${DEFINITION_TO_CHECK}")
            set(${RESULT_VAR} TRUE PARENT_SCOPE)
            break()
        endif()
    endforeach()
endfunction()

# Function to check if a compile definition is set for WPP_TARGET
# Arguments:
#   - DEFINITION_TO_CHECK: The compile definition to look for
#   - RESULT_VAR: The variable to store the result (TRUE or FALSE)
function(wpp_check_definition DEFINITION_TO_CHECK RESULT_VAR)
    # Get the compile definitions for the target
    get_target_property(TARGET_COMPILE_DEFINITIONS ${WPP_TARGET} COMPILE_DEFINITIONS)

    # Initialize result variable
    set(${RESULT_VAR} FALSE PARENT_SCOPE)

    # Loop through each definition and check if the desired definition is set
    foreach(DEFINITION ${TARGET_COMPILE_DEFINITIONS})
        if(DEFINITION STREQUAL "${DEFINITION_TO_CHECK}")
            set(${RESULT_VAR} TRUE PARENT_SCOPE)
            break()
        endif()
    endforeach()
endfunction()

function(target_link_wpp target)
    # Save wpp target name to local variable for later use in subdirectories
    set(WPP_TARGET ${target})
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