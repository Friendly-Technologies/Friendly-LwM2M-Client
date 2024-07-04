/*
 * Temperature
 * Generated on: 2024-06-27 12:56:50
 * Created by: Sinai RnD
 */

#include "o_3303_temperature/Temperature.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Temperature"

namespace wpp {

Temperature::Temperature(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Temperature::~Temperature() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & Temperature::object(WppClient &ctx) {
	return ctx.registry().temperature();
}

Temperature * Temperature::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().temperature().instance(instId);
	if (!inst) return NULL;
	return static_cast<Temperature*>(inst);
}

Temperature * Temperature::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().temperature().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<Temperature*>(inst);
}

bool Temperature::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().temperature().remove(instId);
}

void Temperature::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void Temperature::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void Temperature::resourcesCreate() {
	std::vector<Resource> resources = {
		{SENSOR_VALUE_5700,                      ItemOp(ItemOp::READ),    IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },   
		#if RES_3303_5601                                                                                                                                                    
		{MIN_MEASURED_VALUE_5601,                ItemOp(ItemOp::READ),    IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                               
		#if RES_3303_5602                                                                                                                                                    
		{MAX_MEASURED_VALUE_5602,                ItemOp(ItemOp::READ),    IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                               
		#if RES_3303_5603                                                                                                                                                    
		{MIN_RANGE_VALUE_5603,                   ItemOp(ItemOp::READ),    IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                               
		#if RES_3303_5604                                                                                                                                                    
		{MAX_RANGE_VALUE_5604,                   ItemOp(ItemOp::READ),    IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                               
		#if RES_3303_5701                                                                                                                                                    
		{SENSOR_UNITS_5701,                      ItemOp(ItemOp::READ),    IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                               
		#if RES_3303_5605                                                                                                                                                    
		{RESET_MIN_AND_MAX_MEASURED_VALUES_5605, ItemOp(ItemOp::EXECUTE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE }, 
		#endif                                                                                                                                                               
	};
	setupResources(std::move(resources));
}

void Temperature::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(SENSOR_VALUE_5700)->set<FLOAT_T>(0.0);
	#if RES_3303_5601
	resource(MIN_MEASURED_VALUE_5601)->set<FLOAT_T>(0.0);
	#endif
	#if RES_3303_5602
	resource(MAX_MEASURED_VALUE_5602)->set<FLOAT_T>(0.0);
	#endif
	#if RES_3303_5603
	resource(MIN_RANGE_VALUE_5603)->set<FLOAT_T>(0.0);
	#endif
	#if RES_3303_5604
	resource(MAX_RANGE_VALUE_5604)->set<FLOAT_T>(0.0);
	#endif
	#if RES_3303_5701
	resource(SENSOR_UNITS_5701)->set<STRING_T>("C");
	#endif
	#if RES_3303_5605
	resource(RESET_MIN_AND_MAX_MEASURED_VALUES_5605)->set<EXECUTE_T>(EXECUTE_T());
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
