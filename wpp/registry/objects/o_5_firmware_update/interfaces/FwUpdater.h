#ifndef WPP_FWPKGUPDATER_H
#define WPP_FWPKGUPDATER_H

#include "o_5_firmware_update/FwTypes.h"

namespace wpp {

class FwUpdater {
public:
    FwUpdater() = default;
    virtual ~FwUpdater() = default;

    /**
     * @brief Request to start updating the firmware.
     * For notify that the update is completed, isUpdated() method should return true.
     * For notify update result, lastUpdateResult() method should return the result of the update process.
     */
    virtual void startUpdating() = 0;
    
    /**
     * @brief Returns true if the update process is completed.
     * For notify update result, lastUpdateResult() method should return the result of the update process.
     * Regardless of the reason for the termination, whether it is a complete firmware update or a update error.
     */
    virtual bool isUpdated() = 0;

    /**
     * @brief Contains the result of the last update process.
     * This method is called whenever the FwExternalDl is registered to get
     * information about the result of the last update. If there were no updates,
     * the method should return R_INITIAL
     * Possible results when update is successful: R_INITIAL, R_FW_UPD_SUCCESS
     * Possible results when error occured: R_FW_UPD_FAIL, R_INTEGRITY_CHECK_FAIL
     */
    virtual FwUpdRes lastUpdateResult() = 0;

    /**
     * @brief Returns the name and version of the last installed
     * firmware package. 
     * These methods return the value of the last working firmware.
     * If the update process was unsuccessful, these methods should
     * return the value of the previous firmware. They can be called
     * regardless of the state of the Downloader. Calling reset() does
     * not affect the value they return. The returned value always
     * corresponds to the version and name of the last successfully
     * installed package.
     */
    #if RES_5_6
    virtual STRING_T pkgName() = 0;
    #endif
    #if RES_5_7
    virtual STRING_T pkgVersion() = 0;
    #endif

    /**
     * @brief Reset the update process.
     * This methos can not be called when the updeate
     * process is started. All the information about the previous updating process
     * is cleared. So lastUpdateResult() should return R_INITIAL.
     */
    virtual void reset() = 0;
};

}

#endif // WPP_FWPKGUPDATER_H
