#ifndef FW_AUTO_DL_H
#define FW_AUTO_DL_H

#include <vector>
#include "o_5_firmware_update/FwTypes.h"

namespace wpp {

/**
 * @interface FwInternalDl Interface for auto downloading the firmware through uri and package resource.
 * The FwInternalDl is registered in the FirmwareUpdate object. With using this interface, FirmwareUpdate
 * notifies user about downloaded blocks, required operation and gets the result of the operation. 
 * Downloading process is occured on the Wpp lib side.
 * @note The implementation of each method must not use blocking functions and must terminate as quickly as
 * possible. If possible, operations that take a long time should be performed asynchronously.
 */
class FwInternalDl {
public:
    FwInternalDl() = default;
    virtual ~FwInternalDl() = default;
    
    /**
     * @brief Notify about dwnloading process start.
     * @param pkgName - Name of the firmware package.
     * @param pkgVersion - Version of the firmware package.
     */
    virtual void downloadIsStarted() = 0;
    
    /**
     * @brief Request to save downloaded block of the firmware package.
     * @param dataBlock - Block of the firmware package.
     * If the saving block is not possible, the method should return false,
     * and set appropriate download result, that will be returned by downloadResult() method.
     * @return true if the block is saved successfully, otherwise false.
     */
    virtual bool saveDownloadedBlock(const OPAQUE_T &dataBlock) = 0;
    
    /**
     * @brief Notify about dwnloading process completion.
     * Regardless of the reason for the termination, whether it
     * is a complete firmware downloading or an downloading error.
     */
    virtual void downloadIsCompleted() = 0;

    /**
     * @brief Contains the result of the download process.
     * Possible results when download is successful: R_INITIAL
     * Possible results when error occured: R_NOT_ENOUGH_FLASH, R_OUT_OF_RAM, R_INTEGRITY_CHECK_FAIL, R_UNSUPPORTED_PKG_TYPE
     */
    virtual FwUpdRes downloadResult() = 0;

    /**
     * @brief Reset the download process.
     * When this method is called, the download process is reset and the download
     * process can be started again. This methos can not be called when the updeate
     * process is started. All the information about the previous download process
     * is cleared. So downloadResult() should return R_INITIAL.
     */
    virtual void reset() = 0;
};

}

#endif // FW_AUTO_DL_H
