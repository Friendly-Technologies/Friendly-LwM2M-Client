#ifndef WPP_FW_EXTERNAL_URI_DOWNLOADER_H
#define WPP_FW_EXTERNAL_URI_DOWNLOADER_H

#include <vector>
#include "m_0_lwm2m_security/Lwm2mSecurity.h"
#include "o_5_firmware_update/fwUpdTypes.h"

namespace wpp {

/**
 * @interface FwExternalUriDl Interface for downloading the firmware package from the specified URI.
 * The FwExternalUriDl is registered in the FirmwareUpdate object. With using this interface, FirmwareUpdate
 * notifies user about required operation and gets the result of the operation. But downloading process is
 * occured on the user side. When user downloader implement the same protocol as the FirmwareUpdate, 
 * in this case user downloader will be preferred.
 */
class FwExternalUriDl {
public:
    FwExternalUriDl() = default;
    virtual ~FwExternalUriDl() = default;

    /**
     * @brief Returns the list of supported protocols for downloading the firmware package.
     * After the registration FwExternalUriDl, the list of supported protocols should not be changed.
     * The list should contain at least one protocol.
     */
    virtual std::vector<FwUpdProtocol> supportedProtocols() = 0;
    
    /**
     * @brief Request to start downloading the firmware package from the specified URI.
     * @param uri - URI of the firmware package.
     * @param pkgName - Name of the firmware package.
     * @param pkgVersion - Version of the firmware package.
     * @param security - Security object that contains the information necessary to download the firmware package.
     * For notify that the download is completed, isDownloaded() method should return true.
     * For notify download result, downloadResult() method should return the result of the download process.
     */
    virtual void startDownloading(const STRING_T &uri, const STRING_T &pkgName, const STRING_T &pkgVersion, Lwm2mSecurity &security) = 0;
    /**
     * @brief Returns true if the download process is completed.
     * For notify download result, downloadResult() method should return the result of the download process.
     * Regardless of the reason for the termination, whether it is a complete firmware download or a download error.
     */
    virtual bool isDownloaded() = 0;
    /**
     * @brief Contains the result of the download process.
     * Possible results when download is successful: R_INITIAL
     * Possible results when error occured: R_NOT_ENOUGH_FLASH, R_OUT_OF_RAM, R_CONN_LOST, R_INTEGRITY_CHECK_FAIL, R_UNSUPPORTED_PKG_TYPE
     */
    virtual FwUpdRes downloadResult() = 0;

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
     * This method is called whenever the FwExternalUriDl is registered to get
     * information about the result of the last update. If there were no updates,
     * the method should return R_INITIAL
     * Possible results when update is successful: R_INITIAL, R_FW_UPD_SUCCESS
     * Possible results when error occured: R_FW_UPD_FAIL, R_INTEGRITY_CHECK_FAIL
     */
    virtual FwUpdRes lastUpdateResult() = 0;

    /**
     * @brief Reset the download process.
     * When this method is called, the download process is reset and the download
     * process can be started again. This methos can not becalled when the updeate
     * process is started. All the information about the previous download process
     * is cleared. So downloadResult() and lastUpdateResult() should return R_INITIAL.
     */
    virtual void reset() = 0;
};

} // namespace wpp

#endif // WPP_FW_EXTERNAL_URI_DOWNLOADER_H
