add_subdirectory(downloader)

# Update SOURCES variable from parent scope.
set(SOURCES ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/FirmwareUpdater.cpp PARENT_SCOPE)
# Update INCLUDES variable from parent scope.
set(INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)