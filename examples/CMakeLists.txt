# Project's name
project(WakaamaPlusTest CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

set(SOURCES main.cpp)

add_executable(WakaamaPlusTest ${SOURCES})
target_link_libraries(WakaamaPlusTest WppLib)