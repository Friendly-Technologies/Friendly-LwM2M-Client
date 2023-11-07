/*
 * FirmwareUpdate
 * Generated on: 2023-11-03 15:18:29
 * Created by: SinaiR&D
 */

#include "o_5_firmware_update_v12/FirmwareUpdate.h"

#include "Resource.h"
#include "ResOp.h"
#include "types.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
#include "WppPlatform.h"
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "FirmwareUpdate"

namespace wpp {

FirmwareUpdate::FirmwareUpdate(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

FirmwareUpdate::~FirmwareUpdate() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

void FirmwareUpdate::setDefaultState() {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	_resources.clear();
	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void FirmwareUpdate::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 6 start --------------- */
	switch (type) {
	case ResOp::WRITE_UPD:
	case ResOp::WRITE_REPLACE_RES: {
		switch (resId.resId) {
		case PACKAGE_0: {
			OPAQUE_T pkg;
			// TODO avoid coping of big data package
			resource(PACKAGE_0)->get(pkg);
			if (pkg.empty()) {
				WPP_LOGD_ARG(TAG, "Server reset state machine through PACKAGE_0", resId.resId, resId.resInstId);
				resetStateMachine();
			} else {
				WPP_LOGD_ARG(TAG, "Server write package", resId.resId, resId.resInstId);
				resource(STATE_3)->set(INT_T(DOWNLOADED));
				#if RES_5_12
				resource(LAST_STATE_CHANGE_TIME_12)->set((TIME_T)WppPlatform::getTime());
				#endif
			}
			break;
		}
		case PACKAGE_URI_1: {
			STRING_T pkgUri;
			resource(PACKAGE_URI_1)->get(pkgUri);
			if (pkgUri.empty()) {
				WPP_LOGD_ARG(TAG, "Server reset state machine through PACKAGE_URI_1", resId.resId, resId.resInstId);
				resetStateMachine();
			} else {
				resource(STATE_3)->set(INT_T(DOWNLOADING));
				#if RES_5_12
				resource(LAST_STATE_CHANGE_TIME_12)->set((TIME_T)WppPlatform::getTime());
				#endif
			}
			break;
		}
		default: break;
		}
	}
	case ResOp::EXECUTE: {
		switch (resId.resId) {
		case UPDATE_2: {
			INT_T state;
			resource(STATE_3)->get(state);
			if (state == DOWNLOADED) {
				resource(STATE_3)->set(INT_T(UPDATING));
				#if RES_5_12
				resource(LAST_STATE_CHANGE_TIME_12)->set((TIME_T)WppPlatform::getTime());
				#endif
			}
			break;
		}
		#if RES_5_10
		case CANCEL_10: 
			resetStateMachine();
			break;
		#endif
		default: break;
		}
		break;
	}
	default: break;
	}
	/* --------------- Code_cpp block 6 end --------------- */

	observerNotify(*this, resId, type);

	/* --------------- Code_cpp block 7 start --------------- */
	/* --------------- Code_cpp block 7 end --------------- */
}

void FirmwareUpdate::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 8 start --------------- */
	switch (type) {
	case ResOp::WRITE_UPD:
		WPP_LOGD_ARG(TAG, "User WRITE_UPD -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		if (resId.resId == STATE_3) {
			#if RES_5_12
			resource(LAST_STATE_CHANGE_TIME_12)->set((TIME_T)WppPlatform::getTime());
			#endif
		}
		break;
	default: break;
	}
	/* --------------- Code_cpp block 8 end --------------- */
}

void FirmwareUpdate::resourcesCreate() {
	std::vector<Resource> resources = {
		{PACKAGE_0,                          ResOp(ResOp::WRITE),             IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },  
		{PACKAGE_URI_1,                      ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING },  
		{UPDATE_2,                           ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE }, 
		{STATE_3,                            ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		{UPDATE_RESULT_5,                    ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		#if RES_5_6                                                                                                                                                                
		{PKGNAME_6,                          ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                     
		#if RES_5_7                                                                                                                                                                
		{PKGVERSION_7,                       ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                     
		#if RES_5_8                                                                                                                                                                
		{FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8, ResOp(ResOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                     
		{FIRMWARE_UPDATE_DELIVERY_METHOD_9,  ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		#if RES_5_10                                                                                                                                                               
		{CANCEL_10,                          ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE }, 
		#endif                                                                                                                                                                     
		#if RES_5_11                                                                                                                                                               
		{SEVERITY_11,                        ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                     
		#if RES_5_12                                                                                                                                                               
		{LAST_STATE_CHANGE_TIME_12,          ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },    
		#endif                                                                                                                                                                     
		#if RES_5_13                                                                                                                                                               
		{MAXIMUM_DEFER_PERIOD_13,            ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },    
		#endif                                                                                                                                                                     
		#if RES_5_14                                                                                                                                                               
		{AUTOMATIC_UPGRADE_AT_DOWNLOAD_14,   ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL },    
		#endif                                                                                                                                                                     
	};
	_resources = std::move(resources);
}

void FirmwareUpdate::resourcesInit() {
	/* --------------- Code_cpp block 9 start --------------- */
	resource(PACKAGE_0)->set(OPAQUE_T());
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	resource(UPDATE_2)->set((EXECUTE_T)[](ID_T id, const OPAQUE_T& data) {});
	resource(STATE_3)->set(INT_T(IDLE));
	resource(STATE_3)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return IDLE <= value && value < STATE_MAX; });
	resource(UPDATE_RESULT_5)->set(INT_T(INITIAL));
	resource(UPDATE_RESULT_5)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return INITIAL <= value && value < UPD_RES_MAX; });
	#if RES_5_6
	resource(PKGNAME_6)->set(STRING_T(""));
	#endif
	#if RES_5_7
	resource(PKGVERSION_7)->set(STRING_T(""));
	#endif
	#if RES_5_8
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->set(INT_T(COAP));
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return COAP <= value && value < FW_UPD_PROTOCOL_MAX; });
	#endif
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(PUSH));
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return PULL <= value && value < FW_UPD_DELIVERY_MAX; });
	#if RES_5_10
	resource(CANCEL_10)->set((EXECUTE_T)[](ID_T id, const OPAQUE_T& data) {});
	#endif
	#if RES_5_11
	resource(SEVERITY_11)->set(INT_T(MANDATORY));
	resource(SEVERITY_11)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return CRITICAL <= value && value < SEVERITY_MAX; });
	#endif
	#if RES_5_12
	resource(LAST_STATE_CHANGE_TIME_12)->set(TIME_T(0));
	#endif
	#if RES_5_13
	resource(MAXIMUM_DEFER_PERIOD_13)->set(UINT_T(0));
	#endif
	#if RES_5_14
	resource(AUTOMATIC_UPGRADE_AT_DOWNLOAD_14)->set(BOOL_T(false));
	#endif
	/* --------------- Code_cpp block 9 end --------------- */
}

/* --------------- Code_cpp block 10 start --------------- */
void FirmwareUpdate::resetStateMachine() {
	resource(PACKAGE_0)->set(OPAQUE_T());
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	resource(STATE_3)->set(INT_T(IDLE));
	#if RES_5_12
	resource(LAST_STATE_CHANGE_TIME_12)->set((TIME_T)WppPlatform::getTime());
	#endif
	resource(UPDATE_RESULT_5)->set(INT_T(INITIAL));
}
/* --------------- Code_cpp block 10 end --------------- */

} /* namespace wpp */
