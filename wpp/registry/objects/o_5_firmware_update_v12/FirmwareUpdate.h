/*
 * FirmwareUpdate
 * Generated on: 2023-11-03 15:18:29
 * Created by: SinaiR&D
 */

#ifndef WPP_O_5_FIRMWARE_UPDATE_V12_H
#define WPP_O_5_FIRMWARE_UPDATE_V12_H

#include "FirmwareUpdateConfig.h"
#include "FirmwareUpdateInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
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
		#if RES_5_14
		AUTOMATIC_UPGRADE_AT_DOWNLOAD_14 = 14,
		#endif
	};

	/* --------------- Code_h block 1 start --------------- */
	/* --------------- Code_h block 1 end --------------- */

public:
	FirmwareUpdate(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~FirmwareUpdate();

	/* --------------- Code_h block 2 start --------------- */
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
	
	/* --------------- Code_h block 3 start --------------- */
	/* --------------- Code_h block 3 end --------------- */

private:
	/* --------------- Code_h block 4 start --------------- */
	/* --------------- Code_h block 4 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_5_FIRMWARE_UPDATE_V12_H */
