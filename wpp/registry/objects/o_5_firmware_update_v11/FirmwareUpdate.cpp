/*
 * FirmwareUpdate
 * Generated on: 2023-11-08 13:50:54
 * Created by: SinaiR&D
 */

#include "o_5_firmware_update_v11/FirmwareUpdate.h"

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
	// TODO Implement business logick for Defer Period
	switch (type) {
	case ResOp::WRITE_UPD:
	case ResOp::WRITE_REPLACE_RES: {
		switch (resId.resId) {
		case PACKAGE_0: {
			OPAQUE_T *pkg;
			resource(PACKAGE_0)->ptr(&pkg);
			WPP_LOGD_ARG(TAG, "PACKAGE_0 size: %d", pkg->size());
			WPP_LOGD_ARG(TAG, "PACKAGE_0 first: %d, last: %d", (*pkg)[0], (*pkg)[pkg->size()-1]);
			if (pkg->empty()) {
				WPP_LOGD_ARG(TAG, "Server reset state machine through PACKAGE_0", resId.resId, resId.resInstId);
				resetStateMachine();
				eventNotify(*this, E_RESET);
			} else {
				WPP_LOGD_ARG(TAG, "Server write package", resId.resId, resId.resInstId);
				changeUpdRes(R_INITIAL);
				changeState(S_DOWNLOADING);
				eventNotify(*this, E_PKG_DOWNLOADIN);
				changeState(S_DOWNLOADED);
				eventNotify(*this, E_DOWNLOADED);
			}
			break;
		}
		case PACKAGE_URI_1: {
			STRING_T pkgUri;
			resource(PACKAGE_URI_1)->get(pkgUri);
			if (pkgUri.empty()) {
				WPP_LOGD_ARG(TAG, "Server reset state machine through PACKAGE_URI_1", resId.resId, resId.resInstId);
				resetStateMachine();
				eventNotify(*this, E_RESET);
			} else {
				changeUpdRes(R_INITIAL);
				changeState(S_DOWNLOADING);
				eventNotify(*this, E_URI_DOWNLOADIN);
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
			if (state == S_DOWNLOADED) {
				changeUpdRes(R_INITIAL);
				changeState(S_UPDATING);
				eventNotify(*this, E_UPDATING);
			}
			break;
		}
		#if RES_5_10
		case CANCEL_10: 
			resetStateMachine();
			changeUpdRes(FW_UPD_CANCELLED);
			eventNotify(*this, E_RESET);
			break;
		#endif
		default: break;
		}
		break;
	}
	default: break;
	}
	/* --------------- Code_cpp block 6 end --------------- */

	operationNotify(*this, resId, type);

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
			notifyValueChanged({LAST_STATE_CHANGE_TIME_12,});
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
	};
	_resources = std::move(resources);
}

void FirmwareUpdate::resourcesInit() {
	/* --------------- Code_cpp block 9 start --------------- */
	resource(PACKAGE_0)->set(OPAQUE_T());
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	resource(PACKAGE_URI_1)->setDataVerifier((VERIFY_STRING_T)[this](const STRING_T& value) { return this->isUriValid(value); });
	resource(UPDATE_2)->set((EXECUTE_T)[](ID_T id, const OPAQUE_T& data) { return true; });
	resource(STATE_3)->set(INT_T(S_IDLE));
	resource(STATE_3)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return S_IDLE <= value && value < STATE_MAX; });
	resource(UPDATE_RESULT_5)->set(INT_T(R_INITIAL));
	resource(UPDATE_RESULT_5)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return R_INITIAL <= value && value < UPD_RES_MAX; });
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
	resource(CANCEL_10)->set((EXECUTE_T)[](ID_T id, const OPAQUE_T& data) { return true; });
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
	/* --------------- Code_cpp block 9 end --------------- */
}

/* --------------- Code_cpp block 10 start --------------- */
void FirmwareUpdate::fwFromUriDownloaded() {
	STRING_T *uri = NULL;
	INT_T state;
	resource(PACKAGE_URI_1)->ptr(&uri);
	resource(STATE_3)->get(state);
	if (uri && !uri->empty() && state == S_DOWNLOADING) changeState(S_DOWNLOADED);
}

void FirmwareUpdate::resetStateMachine() {
	resource(PACKAGE_0)->set(OPAQUE_T());
	notifyValueChanged({PACKAGE_0,});
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	notifyValueChanged({PACKAGE_URI_1,});
	changeState(S_IDLE);
	changeUpdRes(R_INITIAL);
}

void FirmwareUpdate::changeState(State state) {
	resource(STATE_3)->set(INT_T(state));
	notifyValueChanged({STATE_3,});
	#if RES_5_12
	resource(LAST_STATE_CHANGE_TIME_12)->set((TIME_T)WppPlatform::getTime());
	notifyValueChanged({LAST_STATE_CHANGE_TIME_12,});
	#endif
}

void FirmwareUpdate::changeUpdRes(UpdRes res) {
	resource(UPDATE_RESULT_5)->set(INT_T(res));
	notifyValueChanged({UPDATE_RESULT_5,});
}

bool FirmwareUpdate::isUriValid(STRING_T uri) {
	STRING_T protocolStr = extarctProtocolFormUri(uri);
	if (protocolStr.empty()) return false;

	#if RES_5_8
	FwUpdProtocol requiredProt = protocolStrToEnum(protocolStr);
	const std::vector<ID_T> &ids = resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->getInstIds();
	for (auto id : ids) {
		INT_T suppProt;
		resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->get(suppProt);
		if (requiredProt == suppProt) return true;
	}
	return false;
	#else
	return true;
	#endif
}

STRING_T FirmwareUpdate::extarctProtocolFormUri(STRING_T uri) {
	return "";
}

#if RES_5_8
FwUpdProtocol FirmwareUpdate::protocolStrToEnum(STRING_T protocol) {

}
#endif
/* --------------- Code_cpp block 10 end --------------- */

} /* namespace wpp */
