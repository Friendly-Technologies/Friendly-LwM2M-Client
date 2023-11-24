# Project's name
project(Catch2Test CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Setup project name and languge
project(WaakamaPlus CXX)

# Include Catch2                                            
set(CATCH2_INCLUDE_DIR "catch2/")

# Serching all files with *.cpp tyoe in folders and subfolders
file(GLOB_RECURSE TEST_SOURCES "./*Test.cpp" "catch_amalgamated.cpp")
file(GLOB_RECURSE SOURCES "../examples/platform/Platform.cpp")

# Create executable file for tests
add_executable(Catch2Tests ${SOURCES} ${TEST_SOURCES})
# Include library
target_link_libraries(Catch2Tests Wpp pthread)
# Include Catch2 library for test executable file
target_include_directories(Catch2Tests PRIVATE ${CATCH2_INCLUDE_DIR})
