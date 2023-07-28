# Project's name
project(WppLib CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Position-independent code is code that can execute
# properly regardless of where in memory it is located.
# This is essential for shared libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# TODO: Delete this src file
set(SOURCES dep.cpp)

add_subdirectory(connection)
add_subdirectory(core)
add_subdirectory(registry)

#message("Source files for build: ${SOURCES}")

add_library(WppLib STATIC ${SOURCES})
# List of directories that will be set for copilator with key -I
target_include_directories(WppLib PUBLIC 
                            ${CMAKE_CURRENT_SOURCE_DIR}/
                            config/
                            connection/ 
                            core/ 
                            registry/
                            registry/data_model/instance/
                            registry/data_model/object/
                            registry/data_model/resource/
                            registry/data_model/types/
                            registry/objects/
                            )
