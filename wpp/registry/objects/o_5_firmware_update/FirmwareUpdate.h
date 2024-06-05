/*
 * FirmwareUpdate
 * Generated on: 2024-04-03 15:26:42
 * Created by: Sinai RnD
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

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

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
		#if RES_5_10
		CANCEL_10 = 10,
		#endif
		#if RES_5_11
		SEVERITY_11 = 11,
		#endif
		#if RES_5_12
		LAST_STATE_CHANGE_TIME_12 = 12,
		#endif
		#if RES_5_13
		MAXIMUM_DEFER_PERIOD_13 = 13,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

public:
	FirmwareUpdate(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~FirmwareUpdate();

	/* --------------- Helpful methods --------------- */
	/**
	 * @brief Gets the Object reference.
	 * @param ctx - WppClient context.
	 * @return A reference to the Object.
	 */
	static Object & object(WppClient &ctx);

	/**
	 * @brief Gets an instance of the object.
	 * @param ctx - WppClient context.
	 * @param instId The instance ID. If not provided, the first available instance is returned.
	 * @return A pointer to the Instance object or NULL.
	 */
	static FirmwareUpdate * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static FirmwareUpdate * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Removes an instance of the object.
	 * @param ctx - WppClient context.
	 * @param instId The instance ID.
	 * @return True if the instance was successfully removed, false otherwise.
	 */
	static bool removeInst(WppClient &ctx, ID_T instId);

	/* --------------- Code_h block 3 start --------------- */
	/**
	 * @brief Set the FwUpdater object for updating the firmware package.
	 * @param ctx - WppClient context.
	 * @param updater - FwUpdater object.
	 * @note Call of this method is reset the current state of the FirmwareUpdate object.
	 * @return true if the FwUpdater object is set successfully, otherwise false.
	 */
	static bool setFwUpdater(WppClient &ctx, FwUpdater &updater);

	#if RES_5_8
	/**
	 * @brief Return the list of supported protocols for downloading the firmware through uri.
	 * @param ctx - WppClient context.
	 */
	static std::vector<FwUpdProtocol> supportedProtocols(WppClient &ctx);
	
	/**
	 * @brief Set the FwExternalDl object for downloading the firmware package from the specified URI.
	 * @param ctx - WppClient context.
	 * @param downloader - FwExternalDl object.
	 * @note Call of this method is reset the current state of the FirmwareUpdate object.
	 * @return true if the FwExternalDl object is set successfully, otherwise false.
	 */
	static bool setFwExternalDownloader(WppClient &ctx, FwExternalDl &downloader);
	#endif

	/**
	 * @brief Set the FwInternalDl object for auto downloading firmware.
	 * @param ctx - WppClient context.
	 * @param downloader - FwInternalDl object.
	 * @note Call of this method is reset the current state of the FirmwareUpdate object.
	 * @return true if the FwInternalDl object is set successfully, otherwise false.
	 */
	static bool setFwInternalDownloader(WppClient &ctx, FwInternalDl &downloader);
	/* --------------- Code_h block 3 end --------------- */

protected:
	/* --------------- Instance implementation part --------------- */
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) override;

	/* --------------- Code_h block 4 start --------------- */
	/* --------------- Code_h block 4 end --------------- */

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Creates resources without initializing.
	 */
	void resourcesCreate();
	/*
	 * Initialize resources with default values
	 * SINGLE resource always must have at least one instance.
	 */	
	void resourcesInit();
	
	/* --------------- Code_h block 5 start --------------- */
	bool pkgUpdaterHandler();
	#if RES_5_8
	void externalDownloaderHandler(Instance *securityInst);
	#endif
	void internalDownloaderHandler();

	void changeUpdRes(FwUpdRes res);
	void changeState(FwUpdState state);
	void resetStateMachine();
	void clearArtifacts();

	bool isUriValid(STRING_T uri);
	STRING_T extractSchemeFromUri(STRING_T uri);
	bool isSchemeValid(STRING_T scheme);
	#if RES_5_8
	bool isSchemeSupported(STRING_T scheme);
	FwUpdProtocol schemeToProtId(STRING_T scheme);
	#endif

	bool isDeliveryTypeSupported(FwUpdDelivery type);
	/* --------------- Code_h block 5 end --------------- */

private:
	/* --------------- Class private properties --------------- */
	/* --------------- Code_h block 6 start --------------- */
	FwUpdater *_pkgUpdater;
	FwInternalDl *_internalDownloader;
	#if RES_5_8
	FwExternalDl *_externalDownloader;
	WppTaskQueue::task_id_t _externalDownloaderTaskId;
	#endif
	WppTaskQueue::task_id_t _internalDownloaderTaskId;
	WppTaskQueue::task_id_t _updaterTaskId;
	/* --------------- Code_h block 6 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_5_FIRMWARE_UPDATE_H */
