# Generated on: 2023-11-02 16:41:01
# Created by: Sinai RnD

wpp_check_definition("OBJ_M_1_LWM2M_SERVER" is_defined)
if(is_defined)
	# Update SOURCES variable from parent scope.
	set(WPP_SOURCES ${WPP_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/Lwm2mServer.cpp)

	# --------------- CMake block 0 start ---------------
	# --------------- CMake block 0 end ---------------

	# Update WPP_SOURCES variable from parent scope.
	set(WPP_SOURCES ${WPP_SOURCES} PARENT_SCOPE)

endif()