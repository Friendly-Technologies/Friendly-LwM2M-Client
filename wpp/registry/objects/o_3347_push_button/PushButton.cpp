/*
 * PushButton
 * Generated on: 2024-06-23 17:09:01
 * Created by: Sinai RnD
 */

#include "o_3347_push_button/PushButton.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "PushButton"

namespace wpp {

PushButton::PushButton(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

PushButton::~PushButton() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & PushButton::object(WppClient &ctx) {
	return ctx.registry().pushButton();
}

PushButton * PushButton::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().pushButton().instance(instId);
	if (!inst) return NULL;
	return static_cast<PushButton*>(inst);
}

PushButton * PushButton::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().pushButton().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<PushButton*>(inst);
}

bool PushButton::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().pushButton().remove(instId);
}

void PushButton::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void PushButton::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void PushButton::resourcesCreate() {
	std::vector<Resource> resources = {
		{DIGITAL_INPUT_STATE_5500,   ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL },   
		#if RES_3347_5501                                                                                                                                                  
		{DIGITAL_INPUT_COUNTER_5501, ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },    
		#endif                                                                                                                                                             
		#if RES_3347_5750                                                                                                                                                  
		{APPLICATION_TYPE_5750,      ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                                             
	};
	setupResources(std::move(resources));
}

void PushButton::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(DIGITAL_INPUT_STATE_5500)->set<BOOL_T>( false );
	#if RES_3347_5501
	resource(DIGITAL_INPUT_COUNTER_5501)->set<INT_T>( 0 );
	#endif
	#if RES_3347_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>( 0 );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
