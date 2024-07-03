# Generated on: 2024-06-27 12:56:50
# Created by: Sinai RnD

wpp_check_definition("OBJ_O_3303_TEMPERATURE" is_defined)
if(is_defined)
	# Update SOURCES variable from parent scope.
	set(WPP_SOURCES ${WPP_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Temperature.cpp)

	# --------------- CMake block 0 start ---------------
	# --------------- CMake block 0 end ---------------

	# Update WPP_SOURCES variable from parent scope.
	set(WPP_SOURCES ${WPP_SOURCES} PARENT_SCOPE)

endif()