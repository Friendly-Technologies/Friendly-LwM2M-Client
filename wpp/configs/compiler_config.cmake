# Setup compiler
# This function sets up the compiler for the target
# Parameters:
#   TARGET: The target to setup the compiler for
#   WITH_EXCEPTIONS: Enable exceptions
#   WITH_RTTI: Enable RTTI
#   FOR_64_BIT: Build for 64 bit system
function(wpp_setup_compiler TARGET WITH_EXCEPTIONS WITH_RTTI FOR_64_BIT INDEPENDENT_CODE)
    # Optimization comiler options
    target_compile_options(${TARGET} PRIVATE
        -ffunction-sections
        -fdata-sections
        -flto
    )
    target_link_options(${TARGET} PRIVATE
        -Wl,--gc-sections
        -flto
        -Wl,-Map=output.map
    )
    # Main comiler options
    target_compile_options(${TARGET} PRIVATE
        -Waggregate-return
        -Wall
        -Wcast-align
        -Wextra
        -Wfloat-equal
        -Wpointer-arith
        -Wshadow
        -Wswitch-default
        -Wwrite-strings
        # Unused parameters are common in this ifdef-littered code-base, but of no danger
        -Wno-unused-parameter
        # Too many false positives
        -Wno-uninitialized
        # Allow usage ##__VA_ARGS__ in macros
        -Wno-gnu-zero-variadic-macro-arguments
        -pedantic
        # Turn (most) warnings into errors
        -Werror
        # Disabled because of existing, non-trivially fixable code
        -Wno-error=cast-align
    )

    # Build for system type
    if (FOR_64_BIT)
        message(STATUS "Building for 64 bit system")
        target_compile_options(${TARGET} PRIVATE -m64)
        set_target_properties(${TARGET} PROPERTIES CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
    else()
        message(STATUS "Building for 32 bit system")
        target_compile_options(${TARGET} PRIVATE -m32)
        set_target_properties(${TARGET} PROPERTIES CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
    endif()
    # Exceptions comiler options
    if (WITH_EXCEPTIONS)
        message(STATUS "Exceptions are supported")
        target_compile_options(${TARGET} PRIVATE -fexceptions)
    else()
        message(STATUS "Exceptions are not supported")
        target_compile_options(${TARGET} PRIVATE -fno-exceptions)
        target_compile_options(${TARGET} PRIVATE -fno-asynchronous-unwind-tables)
    endif()
    # RTTI comiler options
    if (WITH_RTTI)
        message(STATUS "RTTI is supported")
        target_compile_options(${TARGET} PRIVATE -frtti)
    else()
        message(STATUS "RTTI is not supported")
        target_compile_options(${TARGET} PRIVATE -fno-rtti)
    endif()
    # Independent code comiler options
    if (INDEPENDENT_CODE)
        message(STATUS "Independent code is supported")
        set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    else()
        message(STATUS "Independent code is not supported")
        set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE OFF)
    endif()
endfunction()