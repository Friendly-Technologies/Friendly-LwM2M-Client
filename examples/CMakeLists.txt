# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.13)

# Project's name
project(WakaamaPlusTest CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

set(SOURCES main.cpp)

# Client without DTLS support
add_executable(WakaamaPlusTest ${SOURCES})
