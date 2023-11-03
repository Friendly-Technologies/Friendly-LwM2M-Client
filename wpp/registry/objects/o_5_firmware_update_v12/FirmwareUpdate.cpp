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
	/* --------------- Code_cpp block 6 end --------------- */

	observerNotify(*this, resId, type);

	/* --------------- Code_cpp block 7 start --------------- */
	switch (type) {
	case ResOp::READ:
		WPP_LOGD_ARG(TAG, "Server READ -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE_UPD:
		WPP_LOGD_ARG(TAG, "Server WRITE_UPD -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE_REPLACE_INST:
		WPP_LOGD_ARG(TAG, "Server WRITE_REPLACE_INST -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE_REPLACE_RES:
		WPP_LOGD_ARG(TAG, "Server WRITE_REPLACE_RES -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::EXECUTE:
		WPP_LOGD_ARG(TAG, "Server EXECUTE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::DISCOVER:
		WPP_LOGD_ARG(TAG, "Server DISCOVER -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::DELETE:
		WPP_LOGD_ARG(TAG, "Server DELETE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 7 end --------------- */
}

void FirmwareUpdate::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 8 start --------------- */
	switch (type) {
	case ResOp::READ:
		WPP_LOGD_ARG(TAG, "User READ -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE_UPD:
		WPP_LOGD_ARG(TAG, "User WRITE_UPD -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::DELETE:
		WPP_LOGD_ARG(TAG, "User DELETE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
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
	resource(PACKAGE_0)->set( /* TODO */ );
	resource(PACKAGE_0)->setDataVerifier( /* TODO */ );
	resource(PACKAGE_URI_1)->set( /* TODO */ );
	resource(PACKAGE_URI_1)->setDataVerifier( /* TODO */ );
	resource(UPDATE_2)->set( /* TODO */ );
	resource(UPDATE_2)->setDataVerifier( /* TODO */ );
	resource(STATE_3)->set( /* TODO */ );
	resource(STATE_3)->setDataVerifier( /* TODO */ );
	resource(UPDATE_RESULT_5)->set( /* TODO */ );
	resource(UPDATE_RESULT_5)->setDataVerifier( /* TODO */ );
	#if RES_5_6
	resource(PKGNAME_6)->set( /* TODO */ );
	resource(PKGNAME_6)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_5_7
	resource(PKGVERSION_7)->set( /* TODO */ );
	resource(PKGVERSION_7)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_5_8
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->set( /* TODO */ );
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->setDataVerifier( /* TODO */ );
	#endif
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set( /* TODO */ );
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->setDataVerifier( /* TODO */ );
	#if RES_5_10
	resource(CANCEL_10)->set( /* TODO */ );
	resource(CANCEL_10)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_5_11
	resource(SEVERITY_11)->set( /* TODO */ );
	resource(SEVERITY_11)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_5_12
	resource(LAST_STATE_CHANGE_TIME_12)->set( /* TODO */ );
	resource(LAST_STATE_CHANGE_TIME_12)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_5_13
	resource(MAXIMUM_DEFER_PERIOD_13)->set( /* TODO */ );
	resource(MAXIMUM_DEFER_PERIOD_13)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_5_14
	resource(AUTOMATIC_UPGRADE_AT_DOWNLOAD_14)->set( /* TODO */ );
	resource(AUTOMATIC_UPGRADE_AT_DOWNLOAD_14)->setDataVerifier( /* TODO */ );
	#endif
	/* --------------- Code_cpp block 9 end --------------- */
}

/* --------------- Code_cpp block 10 start --------------- */
/* --------------- Code_cpp block 10 end --------------- */

} /* namespace wpp */
