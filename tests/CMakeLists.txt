cmake_minimum_required(VERSION 3.12)

# Setup project name and languge
project(WaakamaPlus CXX)

# Include Catch2                                            
set(CATCH2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Сatch2/catch.hpp")

# Serching all files with *.cpp tyoe in folders and subfolders
file(GLOB_RECURSE TEST_SOURCES "./*Test.cpp")

# Create executable file for tests
add_executable(Catch2Tests ${TEST_SOURCES})

# Include Catch2 library for test executable file
target_include_directories(Catch2Tests PRIVATE ${CATCH2_INCLUDE_DIR})
