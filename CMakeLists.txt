# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.13)

# Project's name
project(WakaamaPlus CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(examples)

# Add library target
add_library(WakaamaPlus STATIC ${SOURCES})
