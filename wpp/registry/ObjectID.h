#ifndef WPP_OBJECT_ID_H_
#define WPP_OBJECT_ID_H_

#include "types.h"

namespace wpp {

/**
 * @brief Enumeration of object IDs in the WakaamaPlus library.
 * 
 * This enumeration represents the object IDs used in the WakaamaPlus library.
 * It includes both mandatory and optional object IDs.
 */
enum class OBJ_ID: ID_T {
	/* ---------- Mandatory objects ID block begin ---------- */
	#ifdef OBJ_M_3_DEVICE_V12
	DEVICE = 3,
	#endif
	#ifdef OBJ_M_1_LWM2M_SERVER_V11
	LWM2M_SERVER = 1,
	#endif
	#ifdef OBJ_M_0_LWM2M_SECURITY_V11
	LWM2M_SECURITY = 0,
	#endif
	/* ---------- Mandatory objects ID block end ---------- */

	/* ---------- Optional objects ID block begin ---------- */
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING_V13
	CONNECTIVITY_MONITORING = 4,
	#endif
	#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
	LWM2M_ACCESS_CONTROL = 2,
	#endif
	#ifdef OBJ_O_3339_AUDIO_CLIP_V10
	AUDIO_CLIP = 3339,
	#endif
	#ifdef OBJ_O_5_FIRMWARE_UPDATE_V10
	FIRMWARE_UPDATE = 5,
	#endif
	/* ---------- Optional objects ID block end ---------- */

	MAX_ID = ID_T_MAX_VAL,
};

}; //wpp

#endif // WPP_OBJECT_ID_H_
