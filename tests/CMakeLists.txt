# Project's name
project(Catch2Test CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add file with wpp lib configs
include(../wpp/wpp.cmake)

# Include file with compiler config
include(../wpp/configs/compiler_config.cmake)

# Compiler options
add_compile_options(-Wno-unused-function)

# Set compiler flags for coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

# Serching all files with *.cpp type in folders and subfolders
file(GLOB_RECURSE SOURCES "Platform.cpp" "Guard.cpp")
file(GLOB_RECURSE TEST_SOURCES "./*Test.cpp" "catch_amalgamated.cpp")

file(GLOB_RECURSE LISTENING "../wpp/*")

# Create executable file for tests
add_executable(Catch2Tests ${SOURCES} ${TEST_SOURCES})

# Set the compilation configuration for this target into the release mode.
# This is due to the fact that the Debug mode cannot be launched due to the
# peculiarities of testing such components as WppTaskQueue, InstSubject,
# ObjSubject, ..., which require the creation of stub classes for WppClient,
# Object, Instance. And it seems that in the Debug mode, two implementation
# conflicts with each other leading to segmentation fault.
target_compile_options(Catch2Tests PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:
    -O3 -DNDEBUG>  # Replace with your desired release flags
)

# Link wpp sources to tests
target_link_wpp(Catch2Tests)
# Link pthread library to tests
target_link_libraries(Catch2Tests pthread)
# Include Catch2 library for test executable file
target_include_directories(Catch2Tests PRIVATE "catch2/")


# Add static analyzer target for cppcheck
add_custom_command(TARGET Catch2Tests PRE_BUILD
    COMMAND cppcheck --language=c++ --std=c++17 --enable=all --force ./../../wpp/* --output-file=cppchect_report.txt --file-filter=*.cpp --file-filter=*.h --suppress=unusedFunction --suppress=missingInclude --suppress=useInitializationList
    COMMAND ${CMAKE_COMMAND} -E echo "---###---"
    COMMAND ${CMAKE_COMMAND} -E echo "Static analyzer cppcheck"
    COMMAND ${CMAKE_COMMAND} -E echo "---###---"
    )

# Add tests target for Catch2
add_custom_command(TARGET Catch2Tests POST_BUILD
    COMMAND Catch2Tests --list-tests
    COMMAND Catch2Tests
    COMMAND ${CMAKE_COMMAND} -E echo "---###---"
    COMMAND ${CMAKE_COMMAND} -E echo "Running tests catch2"
    COMMAND ${CMAKE_COMMAND} -E echo "---###---"
)

# Add coverage target for llvm-cov
add_custom_command(TARGET Catch2Tests POST_BUILD
    COMMAND llvm-profdata merge -sparse default.profraw -o default.profdata
    COMMAND llvm-cov show
            -format=text
            -use-color
            -instr-profile=default.profdata
            -show-expansions
            -output-dir=coverage
            Catch2Tests ${LISTENING}
    COMMAND llvm-cov show
            -format=html
            -use-color
            -instr-profile=default.profdata
            -show-expansions
            -output-dir=coverage
            Catch2Tests ${LISTENING}
    COMMAND ${CMAKE_COMMAND} -E echo "---###---"
    COMMAND cp ./../../tests/tests.sh ./tests.sh
    COMMAND chmod +x ./tests.sh
    COMMAND ./tests.sh
    COMMAND ${CMAKE_COMMAND} -E echo "Coverage determination gcov"
    COMMAND ${CMAKE_COMMAND} -E echo "---###---"
)
