# Build options
option(BUILD_WITH_EXCEPTIONS "Enable support of Exceptions" ON)
option(BUILD_WITH_RTTI "Enable support of RTTI" ON)
option(BUILD_FOR_64_BIT "Enable support of 32-bit system" ON)

# Position-independent code is code that can execute
# properly regardless of where in memory it is located.
# This is essential for shared libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Main comiler options
add_compile_options(    
    -Waggregate-return
    -Wall
    -Wcast-align
    -Wextra
    -Wfloat-equal
    -Wpointer-arith
    -Wshadow
    -Wswitch-default
    -Wwrite-strings
    -pedantic
    # Reduce noise: Unused parameters are common in this ifdef-littered code-base, but of no danger
    -Wno-unused-parameter
    # Reduce noise: Too many false positives
    -Wno-uninitialized
    # Turn (most) warnings into errors
    -Werror
    # Disabled because of existing, non-trivially fixable code
    -Wno-error=cast-align
)

# Build for system type
if (BUILD_FOR_64_BIT)
    message(STATUS "Building for 64 bit system")
    set(CMAKE_C_FLAGS "-m64")
    set(CMAKE_CXX_FLAGS "-m64")
    set(CMAKE_EXE_LINKER_FLAGS "-m64")
else()
    message(STATUS "Building for 32 bit system")
    set(CMAKE_C_FLAGS "-m32")
    set(CMAKE_CXX_FLAGS "-m32")
    set(CMAKE_EXE_LINKER_FLAGS "-m32")  
endif()

# Exceptions comiler options
if (BUILD_WITH_EXCEPTIONS)
    message(STATUS "Exceptions is supported")
    add_compile_options(-fno-exceptions)
else()
    message(STATUS "Exceptions is not supported")
endif()

# RTTI comiler options
if (BUILD_WITH_RTTI)
    message(STATUS "RTTI is supported")
    add_compile_options(-fno-rtti)
else()
    message(STATUS "RTTI is not supported")
endif()

# Share defines with target
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} PARENT_SCOPE)