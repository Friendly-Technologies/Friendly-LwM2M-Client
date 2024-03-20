/*
 * FirmwareUpdate
 * Generated on: 2024-03-19 13:06:08
 * Created by: SinaiR&D
 */

#ifndef WPP_O_5_FIRMWARE_UPDATE_H
#define WPP_O_5_FIRMWARE_UPDATE_H

#include "FirmwareUpdateConfig.h"
#include "FirmwareUpdateInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
#include "WppTaskQueue.h"
#include "fwUpdTypes.h"
#include "o_5_firmware_update/downloader/FwExternalUriDl.h"
#include "o_5_firmware_update/downloader/FwAutoDl.h"
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class FirmwareUpdate : public Instance {
public:
	enum ID: ID_T {
		PACKAGE_0 = 0,
		PACKAGE_URI_1 = 1,
		UPDATE_2 = 2,
		STATE_3 = 3,
		UPDATE_RESULT_5 = 5,
		#if RES_5_6
		PKGNAME_6 = 6,
		#endif
		#if RES_5_7
		PKGVERSION_7 = 7,
		#endif
		#if RES_5_8
		FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8 = 8,
		#endif
		FIRMWARE_UPDATE_DELIVERY_METHOD_9 = 9,
	};

	/* --------------- Code_h block 1 start --------------- */

	enum Event: EVENT_ID_T {
		E_URI_DOWNLOADING = 0,
		E_PKG_DOWNLOADING,
		E_DOWNLOADED,
		E_RESET
	};
	/* --------------- Code_h block 1 end --------------- */

public:
	FirmwareUpdate(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~FirmwareUpdate();

	/* --------------- Code_h block 2 start --------------- */
	#if RES_5_8
	/**
	 * @brief Return the list of supported protocols for downloading the firmware through uri.
	 */
	std::vector<FwUpdProtocol> supportedProtocols();
	
	/**
	 * @brief Set the FwExternalUriDl object for downloading the firmware package from the specified URI.
	 * @param downloader - FwExternalUriDl object.
	 * @return true if the FwExternalUriDl object is set successfully, otherwise false.
	 */
	bool setFwExternalUriDownloader(FwExternalUriDl &downloader);
	#endif

	/**
	 * @brief Set the FwAutoDl object for auto downloading firmware.
	 * @param downloader - FwAutoDl object.
	 * @return true if the FwAutoDl object is set successfully, otherwise false.
	 */
	bool setFwAutoDownloader(FwAutoDl &downloader);
	/* --------------- Code_h block 2 end --------------- */

protected:
	/* --------------- Instance implementation part --------------- */
	/*
	 * Reset all resources values and internal state to default.
	 */
	void setDefaultState() override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;

	/* --------------- Code_h block 3 start --------------- */
	#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	void serverBlockOperationNotifier(ResOp::TYPE type, const ResLink &resId, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) override;
	#endif
	/* --------------- Code_h block 3 end --------------- */

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Creates resources without initializing.
	 */
	void resourcesCreate();
	/*
	 * Initialize resources with default values
	 * Resource always must have at least one instance.
	 * Note: From server side, empty resource == undefined resource.
	 */	
	void resourcesInit();
	
	/* --------------- Code_h block 4 start --------------- */
	void changeUpdRes(FwUpdRes res);
	void changeState(FwUpdState state);
	void resetStateMachine();

	bool isPkgValid(OPAQUE_T uri);

	bool isUriValid(STRING_T uri);
	STRING_T extractSchemeFromUri(STRING_T uri);
	bool isSchemeValid(STRING_T scheme);
	#if RES_5_8
	bool isSchemeSupported(STRING_T scheme);
	FwUpdProtocol schemeToProtId(STRING_T scheme);
	#endif

	bool isNewStateValid(FwUpdState newState);

	bool isDeliveryTypeSupported(FwUpdDelivery type);
	/* --------------- Code_h block 4 end --------------- */

private:
	/* --------------- Class private properties --------------- */
	/* --------------- Code_h block 5 start --------------- */
	FwExternalUriDl *_uriDownloader;
	FwAutoDl *_autoDownloader;
	/* --------------- Code_h block 5 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_5_FIRMWARE_UPDATE_H */
