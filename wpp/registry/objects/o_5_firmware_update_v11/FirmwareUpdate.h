/*
 * FirmwareUpdate
 * Generated on: 2023-11-08 13:50:54
 * Created by: SinaiR&D
 */

#ifndef WPP_O_5_FIRMWARE_UPDATE_V11_H
#define WPP_O_5_FIRMWARE_UPDATE_V11_H

#include "FirmwareUpdateConfig.h"
#include "FirmwareUpdateInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
#include "WppTaskQueue.h"
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
	};

	/* --------------- Code_h block 1 start --------------- */
	enum Event: EVENT_ID_T {
		E_URI_DOWNLOADIN = 0,
		E_PKG_DOWNLOADIN,
		E_DOWNLOADED,
		E_RESET
	};

	enum State: uint8_t {
		S_IDLE = 0,
		S_DOWNLOADING = 1,
		S_DOWNLOADED = 2,
		S_UPDATING = 3,
		STATE_MAX
	};

	enum UpdRes: uint8_t {
		R_INITIAL = 0,
		R_FW_UPD_SUCCESS = 1,
		R_NOT_ENOUGH_FLASH = 2,
		R_OUT_OF_RAM = 3,
		R_CONN_LOST = 4,
		R_INTEGRITY_CHECK_FAIL = 5,
		R_UNSUPPORTED_PKG_TYPE = 6,
		R_INVALID_URI = 7,
		R_FW_UPD_FAIL = 8,
		R_UNSUPPORTED_PROTOCOL = 9,
		R_FW_UPD_CANCELLED = 10,
		#if RES_5_13
		R_FW_UPD_DEFERRED = 11,
		#endif
		UPD_RES_MAX
	};

	#if RES_5_8
	enum FwUpdProtocol: uint8_t {
		COAP = 0,
		COAPS = 1,
		HTTP = 2,
		HTTPS = 3,
		COAP_TCP = 4,
		COAP_TLS = 5,
		FW_UPD_PROTOCOL_MAX
	};
	#endif

	enum FwUpdDelivery: uint8_t {
		PULL = 0,
		PUSH = 1,
		BOTH = 2,
		FW_UPD_DELIVERY_MAX
	};

	#if RES_5_11
	enum Severity: uint8_t {
		CRITICAL = 0,
		MANDATORY = 1,
		OPTIONAL = 2,
		SEVERITY_MAX
	};
	#endif
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
	void changeUpdRes(UpdRes res);
	void changeState(State state);
	void resetStateMachine();

	bool isPkgValid(OPAQUE_T uri);

	bool isUriValid(STRING_T uri);
	STRING_T extractSchemeFromUri(STRING_T uri);
	bool isSchemeValid(STRING_T scheme);
	#if RES_5_8
	bool isSchemeSupported(STRING_T scheme);
	FwUpdProtocol schemeToProtId(STRING_T scheme);
	#endif

	bool isNewStateValid(State newState);

	bool isDeliveryTypeSupported(FwUpdDelivery type);

	#if RES_5_13
	void startDeferUpdateGuard();
	void stopDeferUpdateGuard();
	#endif
	/* --------------- Code_h block 4 end --------------- */

private:
	/* --------------- Class private properties --------------- */
	/* --------------- Code_h block 5 start --------------- */
	#if RES_5_13
	WppTaskQueue::task_id_t _deferUpdateTaskId;
	#endif
	/* --------------- Code_h block 5 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_5_FIRMWARE_UPDATE_V11_H */
