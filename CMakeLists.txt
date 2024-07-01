# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.13)

# Project's name
project(WppClient)

# Add subdirectory with docs target
add_subdirectory(doxg)
# Add subdirectory with wpp library target
add_subdirectory(wpp)
# Add subdirectory with examples targets
add_subdirectory(examples)
# Add subdirectory with Catch2 tests
add_subdirectory(tests)