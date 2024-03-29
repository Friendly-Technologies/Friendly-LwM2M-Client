/*
 * AudioClip
 * Generated on: 2024-03-11 13:52:27
 * Created by: SinaiR&D
 */

#include "o_3339_audio_clip/AudioClip.h"

#include "Resource.h"
#include "ResOp.h"
#include "WppTypes.h"
#include "WppLogs.h"

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

void AudioClip::serverOperationNotifier(Instance *securityInst, ResOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 6 start --------------- */
	/* --------------- Code_cpp block 6 end --------------- */

	operationNotify(*this, resLink, type);

	/* --------------- Code_cpp block 7 start --------------- */
	switch (type) {
	case ResOp::READ:
		WPP_LOGD(TAG, "Server READ -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ResOp::WRITE:
		WPP_LOGD(TAG, "Server WRITE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ResOp::EXECUTE:
		WPP_LOGD(TAG, "Server EXECUTE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ResOp::DISCOVER:
		WPP_LOGD(TAG, "Server DISCOVER -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 7 end --------------- */
}

void AudioClip::userOperationNotifier(ResOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 8 start --------------- */
	switch (type) {
	case ResOp::READ:
		WPP_LOGD(TAG, "User READ -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ResOp::WRITE:
		WPP_LOGD(TAG, "User WRITE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ResOp::DELETE:
		WPP_LOGD(TAG, "User DELETE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 8 end --------------- */
}

/* --------------- Code_cpp block 9 start --------------- */
/* --------------- Code_cpp block 9 end --------------- */

void AudioClip::resourcesCreate() {
	std::vector<Resource> resources = {
		{CLIP_5522,             ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },  
		#if RES_3339_5523                                                                                                                                           
		{TRIGGER_5523,          ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE }, 
		#endif                                                                                                                                                      
		#if RES_3339_5548                                                                                                                                           
		{LEVEL_5548,            ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT }, 
		#endif                                                                                                                                                      
		#if RES_3339_5524                                                                                                                                           
		{DURATION_5524,         ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT }, 
		#endif                                                                                                                                                      
		#if RES_3339_5750                                                                                                                                           
		{APPLICATION_TYPE_5750, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                      
	};
	_resources = std::move(resources);
}

void AudioClip::resourcesInit() {
	/* --------------- Code_cpp block 10 start --------------- */
	resource(CLIP_5522)->set(OPAQUE_T());
	#if RES_3339_5523
	resource(TRIGGER_5523)->set((EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	#endif
	#if RES_3339_5548
	resource(LEVEL_5548)->set((EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	#endif
	#if RES_3339_5524
	resource(DURATION_5524)->set((EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	#endif
	#if RES_3339_5750
	resource(APPLICATION_TYPE_5750)->set(STRING_T());
	#endif
	/* --------------- Code_cpp block 10 end --------------- */
}

/* --------------- Code_cpp block 11 start --------------- */
/* --------------- Code_cpp block 11 end --------------- */

} /* namespace wpp */
