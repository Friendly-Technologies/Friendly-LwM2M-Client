option(WPP_ENABLE_LOGS "Enable logs for WakaamaPlus" ON)
if (WPP_ENABLE_LOGS)
    set(WPP_DEFINITIONS ${WPP_DEFINITIONS} WPP_ENABLE_LOGS=1)
endif()

# Share defines with target
set(WPP_DEFINITIONS ${WPP_DEFINITIONS} PARENT_SCOPE)