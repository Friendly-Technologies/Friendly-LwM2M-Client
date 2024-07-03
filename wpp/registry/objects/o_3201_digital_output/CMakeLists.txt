# Generated on: 2024-06-23 14:34:27
# Created by: Sinai RnD

wpp_check_definition("OBJ_O_3201_DIGITAL_OUTPUT" is_defined)
if(is_defined)
	# Update SOURCES variable from parent scope.
	set(WPP_SOURCES ${WPP_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/DigitalOutput.cpp)

	# --------------- CMake block 0 start ---------------
	# --------------- CMake block 0 end ---------------

	# Update WPP_SOURCES variable from parent scope.
	set(WPP_SOURCES ${WPP_SOURCES} PARENT_SCOPE)

endif()