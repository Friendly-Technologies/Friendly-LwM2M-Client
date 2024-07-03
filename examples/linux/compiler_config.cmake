# Setup compiler
# This function sets up the compiler for the target
# Parameters:
#   TARGET: The target to setup the compiler for
function(setup_compiler TARGET)
    # Include file with compiler config
    include(${CMAKE_SOURCE_DIR}/wpp/configs/compiler_config.cmake)
    # Apply wpp compiler config
    wpp_setup_compiler(${TARGET} ${WPP_BUILD_WITH_EXCEPTIONS} ${WPP_BUILD_WITH_RTTI} ${WPP_BUILD_FOR_64_BIT} ${WPP_BUILD_INDEPENDENT_CODE})
    # Setup target properties
    set_target_properties(${TARGET} PROPERTIES
        # Generating .map file
        CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=output.map"
    )
    # Main comiler options
    target_compile_options(${TARGET} PRIVATE
        # Disabled because Connection for dtls cast from 'sockaddr *' to 'sockaddr_in *' increases required alignment from 2 to 4
        -Wno-cast-align
        # Reduce noise: Too many false positives
        -Wno-uninitialized
        # Allows to compile tinydtls without errors
        -Wno-c99-extensions
    )
endfunction()
