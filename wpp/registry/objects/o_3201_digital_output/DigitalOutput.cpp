/*
 * DigitalOutput
 * Generated on: 2024-06-23 14:34:27
 * Created by: Sinai RnD
 */

#include "o_3201_digital_output/DigitalOutput.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "DigitalOutput"

namespace wpp {

DigitalOutput::DigitalOutput(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

DigitalOutput::~DigitalOutput() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & DigitalOutput::object(WppClient &ctx) {
	return ctx.registry().digitalOutput();
}

DigitalOutput * DigitalOutput::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().digitalOutput().instance(instId);
	if (!inst) return NULL;
	return static_cast<DigitalOutput*>(inst);
}

DigitalOutput * DigitalOutput::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().digitalOutput().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<DigitalOutput*>(inst);
}

bool DigitalOutput::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().digitalOutput().remove(instId);
}

void DigitalOutput::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void DigitalOutput::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void DigitalOutput::resourcesCreate() {
	std::vector<Resource> resources = {
		{DIGITAL_OUTPUT_STATE_5550,    ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL },   
		#if RES_3201_5551                                                                                                                                                    
		{DIGITAL_OUTPUT_POLARITY_5551, ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL },   
		#endif                                                                                                                                                               
		#if RES_3201_5750                                                                                                                                                    
		{APPLICATION_TYPE_5750,        ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                                               
	};
	setupResources(std::move(resources));
}

void DigitalOutput::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(DIGITAL_OUTPUT_STATE_5550)->set<BOOL_T>( 0 );
	#if RES_3201_5551
	resource(DIGITAL_OUTPUT_POLARITY_5551)->set<BOOL_T>( 0 );
	#endif
	#if RES_3201_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>( "" );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
