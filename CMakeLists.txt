# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.13)

# Project's name
project(WakaamaPlus)

# Add subdirectory with wakaama core library target
add_subdirectory(wakaama_core)
# Add subdirectory with wpp library target
add_subdirectory(wpp)
# Add subdirectory with examples targets
add_subdirectory(examples)
