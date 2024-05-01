# Build options
option(WPP_BUILD_WITH_EXCEPTIONS "Enable support of Exceptions" OFF)
option(WPP_BUILD_WITH_RTTI "Enable support of RTTI" OFF)
option(WPP_BUILD_FOR_64_BIT "Build for 64-bit system or 32-bit" ON)

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
if (WPP_BUILD_FOR_64_BIT)
    message(STATUS "Building for 64 bit system")
    add_compile_options(-m64)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
else()
    message(STATUS "Building for 32 bit system")
    add_compile_options(-m32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
endif()

# Exceptions comiler options
if (WPP_BUILD_WITH_EXCEPTIONS)
    message(STATUS "Exceptions are supported")
    add_compile_options(-fexceptions)
else()
    message(STATUS "Exceptions are not supported")
    add_compile_options(-fno-exceptions)
    add_compile_options(-fno-asynchronous-unwind-tables)
endif()

# RTTI comiler options
if (WPP_BUILD_WITH_RTTI)
    message(STATUS "RTTI is supported")
    add_compile_options(-frtti)
else()
    message(STATUS "RTTI is not supported")
    add_compile_options(-fno-rtti)
endif()