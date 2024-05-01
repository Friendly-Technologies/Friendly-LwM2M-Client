/*
 * AudioClip
 * Generated on: 2024-03-11 13:52:27
 * Created by: SinaiR&D
 */

#ifndef WPP_O_3339_AUDIO_CLIP_H
#define WPP_O_3339_AUDIO_CLIP_H

#include "AudioClipConfig.h"
#include "AudioClipInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class AudioClip : public Instance {
public:
	enum ID: ID_T {
		CLIP_5522 = 5522,
		#if RES_3339_5523
		TRIGGER_5523 = 5523,
		#endif
		#if RES_3339_5548
		LEVEL_5548 = 5548,
		#endif
		#if RES_3339_5524
		DURATION_5524 = 5524,
		#endif
		#if RES_3339_5750
		APPLICATION_TYPE_5750 = 5750,
		#endif
	};

	/* --------------- Code_h block 1 start --------------- */
	/* --------------- Code_h block 1 end --------------- */

public:
	AudioClip(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~AudioClip();

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

protected:
	/* --------------- Instance implementation part --------------- */
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Instance *securityInst, ResOp::TYPE type, const ResLink &resLink) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ResOp::TYPE type, const ResLink &resLink) override;

	/* --------------- Code_h block 3 start --------------- */
	/* --------------- Code_h block 3 end --------------- */

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Creates resources without initializing.
	 */
	void resourcesCreate();
	/*
	 * Initialize resources with default values
	 * Resource always must have at least one instance.
	 * Note: From server side, empty resource == undefined resource.
	 */	
	void resourcesInit();
	
	/* --------------- Code_h block 4 start --------------- */
	/* --------------- Code_h block 4 end --------------- */

private:
	/* --------------- Class private properties --------------- */
	/* --------------- Code_h block 5 start --------------- */
	/* --------------- Code_h block 5 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_3339_AUDIO_CLIP_H */
