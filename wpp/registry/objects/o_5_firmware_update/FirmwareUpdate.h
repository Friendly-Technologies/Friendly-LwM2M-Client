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
#include "FwTypes.h"
#include "o_5_firmware_update/interfaces/FwUpdater.h"
#if RES_5_8
#include "o_5_firmware_update/interfaces/FwExternalDl.h"
#endif
#include "o_5_firmware_update/interfaces/FwInternalDl.h"
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
	/* --------------- Code_h block 1 end --------------- */

public:
	FirmwareUpdate(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~FirmwareUpdate();

	/* --------------- Code_h block 2 start --------------- */
	/**
	 * @brief Set the FwUpdater object for updating the firmware package.
	 * @param updater - FwUpdater object.
	 * @note Call of this method is reset the current state of the FirmwareUpdate object.
	 * @return true if the FwUpdater object is set successfully, otherwise false.
	 */
	bool setFwUpdater(FwUpdater &updater);

	#if RES_5_8
	/**
	 * @brief Return the list of supported protocols for downloading the firmware through uri.
	 */
	std::vector<FwUpdProtocol> supportedProtocols();
	
	/**
	 * @brief Set the FwExternalDl object for downloading the firmware package from the specified URI.
	 * @param downloader - FwExternalDl object.
	 * @note Call of this method is reset the current state of the FirmwareUpdate object.
	 * @return true if the FwExternalDl object is set successfully, otherwise false.
	 */
	bool setFwExternalDownloader(FwExternalDl &downloader);
	#endif

	/**
	 * @brief Set the FwInternalDl object for auto downloading firmware.
	 * @param downloader - FwInternalDl object.
	 * @note Call of this method is reset the current state of the FirmwareUpdate object.
	 * @return true if the FwInternalDl object is set successfully, otherwise false.
	 */
	bool setFwInternalDownloader(FwInternalDl &downloader);
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
	void serverOperationNotifier(ID_T securityInstId, ResOp::TYPE type, const ResLink &resId) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;

	/* --------------- Code_h block 3 start --------------- */
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
	void pkgUpdaterHandler();
	#if RES_5_8
	void externalDownloaderHandler(ID_T securityInstId);
	#endif
	void internalDownloaderHandler();

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
	FwUpdater *_pkgUpdater;
	FwInternalDl *_internalDownloader;
	#if RES_5_8
	FwExternalDl *_externalDownloader;
	WppTaskQueue::task_id_t _externalDownloaderTaskId;
	#endif
	WppTaskQueue::task_id_t _internalDownloaderTaskId;
	WppTaskQueue::task_id_t _updaterTaskId;
	/* --------------- Code_h block 5 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_5_FIRMWARE_UPDATE_H */
