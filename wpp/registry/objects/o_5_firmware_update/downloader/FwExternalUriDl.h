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
 * @note The implementation of each method must not use blocking functions and must terminate as quickly as
 * possible. If possible, operations that take a long time should be performed asynchronously.
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
     * @param security - Security object that contains the information necessary to download the firmware package.
     * For notify that the download is completed, isDownloaded() method should return true.
     * For notify download result, downloadResult() method should return the result of the download process.
     */
    virtual void startDownloading(const STRING_T &uri, Lwm2mSecurity &security) = 0;
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

    /*
     * @brief Reset the download process.
     * When this method is called, the download process is reset and the download
     * process can be started again. This methos can not be called when the updeate
     * process is started. All the information about the previous download process
     * is cleared. So downloadResult() should return R_INITIAL.
     */
    virtual void reset() = 0;
};

} // namespace wpp

#endif // WPP_FW_EXTERNAL_URI_DOWNLOADER_H
