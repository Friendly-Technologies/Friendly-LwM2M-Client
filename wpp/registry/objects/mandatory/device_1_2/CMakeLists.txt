if(OBJ_DEVICE_1_2_M)
	# Update SOURCES variable from parent scope.
	set(WPP_SOURCES ${WPP_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/WppDevice.cpp)

	# --------------- CMake block 0 start ---------------
	# --------------- CMake block 0 end ---------------

	# Update WPP_SOURCES variable from parent scope.
	set(WPP_SOURCES ${WPP_SOURCES} PARENT_SCOPE)

endif()