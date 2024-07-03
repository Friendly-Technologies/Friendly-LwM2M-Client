/*
 * Accelerometer
 * Generated on: 2024-06-27 12:57:44
 * Created by: Sinai RnD
 */

#include "o_3313_accelerometer/Accelerometer.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Accelerometer"

namespace wpp {

Accelerometer::Accelerometer(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Accelerometer::~Accelerometer() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & Accelerometer::object(WppClient &ctx) {
	return ctx.registry().accelerometer();
}

Accelerometer * Accelerometer::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().accelerometer().instance(instId);
	if (!inst) return NULL;
	return static_cast<Accelerometer*>(inst);
}

Accelerometer * Accelerometer::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().accelerometer().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<Accelerometer*>(inst);
}

bool Accelerometer::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().accelerometer().remove(instId);
}

void Accelerometer::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	operationNotify(*this, resLink, type);

	/* --------------- Code_cpp block 5 start --------------- */
	switch (type) {
	case ItemOp::WRITE:
		WPP_LOGD(TAG, "Server WRITE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ItemOp::EXECUTE:
		WPP_LOGD(TAG, "Server EXECUTE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 5 end --------------- */
}

void Accelerometer::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
	if (type == ItemOp::WRITE || type == ItemOp::DELETE) notifyResChanged(resLink.resId, resLink.resInstId);

	/* --------------- Code_cpp block 6 start --------------- */
	switch (type) {
	case ItemOp::WRITE:
		WPP_LOGD(TAG, "User WRITE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ItemOp::DELETE:
		WPP_LOGD(TAG, "User DELETE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 6 end --------------- */
}

void Accelerometer::resourcesCreate() {
	std::vector<Resource> resources = {
		{X_VALUE_5702,         ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },  
		#if RES_3313_5703                                                                                                                              
		{Y_VALUE_5703,         ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                         
		#if RES_3313_5704                                                                                                                              
		{Z_VALUE_5704,         ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                         
		#if RES_3313_5701                                                                                                                              
		{SENSOR_UNITS_5701,    ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                         
		#if RES_3313_5603                                                                                                                              
		{MIN_RANGE_VALUE_5603, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                         
		#if RES_3313_5604                                                                                                                              
		{MAX_RANGE_VALUE_5604, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                         
	};
	setupResources(std::move(resources));
}

void Accelerometer::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(X_VALUE_5702)->set<FLOAT_T>(0.0);
	#if RES_3313_5703
	resource(Y_VALUE_5703)->set<FLOAT_T>(0.0);
	#endif
	#if RES_3313_5704
	resource(Z_VALUE_5704)->set<FLOAT_T>(0.0);
	#endif
	#if RES_3313_5701
	resource(SENSOR_UNITS_5701)->set<STRING_T>("");
	#endif
	#if RES_3313_5603
	resource(MIN_RANGE_VALUE_5603)->set<FLOAT_T>(0.0);
	#endif
	#if RES_3313_5604
	resource(MAX_RANGE_VALUE_5604)->set<FLOAT_T>(0.0);
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
