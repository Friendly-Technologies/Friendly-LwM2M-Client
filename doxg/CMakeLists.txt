# Check if Doxygen is installed
find_package(Doxygen)
if(DOXYGEN_FOUND)
    # Add a custom target to run Doxygen
    add_custom_target(
        Documentation
        COMMAND ${DOXYGEN_EXECUTABLE} Doxygen
        COMMAND xdg-open ${CMAKE_CURRENT_SOURCE_DIR}/../docs/index.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen in the docs directory"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found, documentation will not be generated")
endif()