/*
 * Lwm2mAccessControl
 * Generated on: 2023-11-03 15:05:47
 * Created by: SinaiR&D
 */

#include "o_2_lwm2m_access_control_v11/Lwm2mAccessControl.h"

#include <unordered_map>
#include <iostream>

#include "Resource.h"
#include "ResOp.h"
#include "types.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Lwm2mAccessControl"

namespace wpp {

Lwm2mAccessControl::Lwm2mAccessControl(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Lwm2mAccessControl::~Lwm2mAccessControl() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

void Lwm2mAccessControl::setDefaultState() {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	_resources.clear();
	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void Lwm2mAccessControl::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
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

void Lwm2mAccessControl::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
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

void Lwm2mAccessControl::resourcesCreate() {
	std::vector<Resource> resources = {
		{OBJECT_ID_0,            ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
		{OBJECT_INSTANCE_ID_1,   ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
		#if RES_2_2                                                                                                                                                
		{ACL_2,                  ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT }, 
		#endif                                                                                                                                                     
		{ACCESS_CONTROL_OWNER_3, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
	};
	_resources = std::move(resources);
}

void Lwm2mAccessControl::resourcesInit() {
	/* --------------- Code_cpp block 9 start --------------- */
	resource(OBJECT_ID_0)->set( /* TODO */ );
	resource(OBJECT_ID_0)->setDataVerifier( /* TODO */ );
	resource(OBJECT_INSTANCE_ID_1)->set( /* TODO */ );
	resource(OBJECT_INSTANCE_ID_1)->setDataVerifier( /* TODO */ );
	#if RES_2_2
	resource(ACL_2)->set( /* TODO */ );
	resource(ACL_2)->setDataVerifier( /* TODO */ );
	#endif
	resource(ACCESS_CONTROL_OWNER_3)->set( /* TODO */ );
	resource(ACCESS_CONTROL_OWNER_3)->setDataVerifier( /* TODO */ );
	/* --------------- Code_cpp block 9 end --------------- */
}

/* --------------- Code_cpp block 10 start --------------- */
/* --------------- Code_cpp block 10 end --------------- */

} /* namespace wpp */
