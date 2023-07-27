# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.13)

# Project's name
project(WakaamaPlus CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

set(SOURCES main.cpp dep.cpp)

add_executable(WakaamaPlusTest ${SOURCES})
