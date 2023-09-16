# Project's name
project(WakaamaPlusTest CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Variables that will be filled in subdirectories
set(SOURCES main.cpp)
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMPILE_DEFINITIONS "")

set(SOURCES main.cpp)

# Add subdiretories with sources
add_subdirectory(connection)
add_subdirectory(platform)

add_executable(WakaamaPlusTest ${SOURCES})
# List of directories that will be set for compiler with key -I
target_include_directories(WakaamaPlusTest PUBLIC ${INCLUDES})

target_link_libraries(WakaamaPlusTest WppLib)
