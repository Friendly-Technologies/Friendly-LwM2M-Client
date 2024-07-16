/*
 * AudioClip
 * Generated on: 2024-03-11 13:52:27
 * Created by: Sinai RnD
 */

#include "o_3339_audio_clip/AudioClip.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "AudioClip"

namespace wpp {

AudioClip::AudioClip(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

AudioClip::~AudioClip() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & AudioClip::object(WppClient &ctx) {
	return ctx.registry().audioClip();
}

AudioClip * AudioClip::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().audioClip().instance(instId);
	if (!inst) return NULL;
	return static_cast<AudioClip*>(inst);
}

AudioClip * AudioClip::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().audioClip().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<AudioClip*>(inst);
}

bool AudioClip::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().audioClip().remove(instId);
}

void AudioClip::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void AudioClip::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void AudioClip::resourcesCreate() {
	std::vector<Resource> resources = {
		{CLIP_5522,             ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },  
		#if RES_3339_5523                                                                                                                                           
		{TRIGGER_5523,          ItemOp(ItemOp::EXECUTE),           IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE }, 
		#endif                                                                                                                                                      
		#if RES_3339_5548                                                                                                                                           
		{LEVEL_5548,            ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT }, 
		#endif                                                                                                                                                      
		#if RES_3339_5524                                                                                                                                           
		{DURATION_5524,         ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT }, 
		#endif                                                                                                                                                      
		#if RES_3339_5750                                                                                                                                           
		{APPLICATION_TYPE_5750, ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                      
	};
	setupResources(std::move(resources));
}

void AudioClip::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(CLIP_5522)->set<OPAQUE_T>({0});
	#if RES_3339_5523
	resource(TRIGGER_5523)->set<EXECUTE_T>([](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	#endif
	#if RES_3339_5548
	resource(LEVEL_5548)->set<EXECUTE_T>([](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	#endif
	#if RES_3339_5524
	resource(DURATION_5524)->set<EXECUTE_T>([](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	#endif
	#if RES_3339_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>("");
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
