# Project's name
project(Catch2Test CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include Catch2
set(CATCH2_INCLUDE_DIR "catch2/")

# Set compiler flags for coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

# Serching all files with *.cpp type in folders and subfolders
file(GLOB_RECURSE TEST_SOURCES "./*Test.cpp" "catch_amalgamated.cpp")
file(GLOB_RECURSE SOURCES "../examples/platform/Platform.cpp" "../examples/platform/Guard.cpp")
file(GLOB_RECURSE LISTENING "../wpp/*")

# Create executable file for tests
add_executable(Catch2Tests ${SOURCES} ${TEST_SOURCES})

# add_executable(Catch2Tests33 ${SOURCES} ${TEST_SOURCES})

# Include library
target_link_options(Catch2Tests PRIVATE -Wl,--whole-archive -Wl,--no-whole-archive)
target_link_libraries(Catch2Tests Wpp pthread)

# Include Catch2 library for test executable file
target_include_directories(Catch2Tests PRIVATE ${CATCH2_INCLUDE_DIR})

# Add coverage target
add_custom_target(coverage_llvm-cov
    
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
    
    COMMAND cat coverage/index.txt | grep -E 'Filename|TOTAL' | column -t
)

add_custom_target(tests_catch2
    COMMAND Catch2Tests --list-tests
    COMMAND Catch2Tests
)

add_custom_target(statick-anylizer_cppcheck
    COMMAND cppcheck --language=c++ --std=c++17 --enable=all --force ./../../wpp/* --output-file=cppchect_report.txt --file-filter=*.cpp --file-filter=*.h
)

add_custom_command(TARGET Catch2Tests POST_BUILD
    COMMAND cppcheck --language=c++ --std=c++17 --enable=all --force ./../../wpp/* --output-file=cppchect_report.txt --file-filter=*.cpp --file-filter=*.h
)

