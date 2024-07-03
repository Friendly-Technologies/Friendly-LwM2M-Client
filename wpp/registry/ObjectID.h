#ifndef WPP_OBJECT_ID_H_
#define WPP_OBJECT_ID_H_

#include "WppTypes.h"

namespace wpp {

/**
 * @brief Enumeration of object IDs in the Wpp library.
 * 
 * This enumeration represents the object IDs used in the Wpp library.
 * It includes both mandatory and optional object IDs.
 */
enum OBJ_ID: ID_T {
	/* ---------- Mandatory objects ID block begin ---------- */
	#ifdef OBJ_M_3_DEVICE
	DEVICE = 3,
	#endif
	#ifdef OBJ_M_1_LWM2M_SERVER
	LWM2M_SERVER = 1,
	#endif
	#ifdef OBJ_M_0_LWM2M_SECURITY
	LWM2M_SECURITY = 0,
	#endif
	/* ---------- Mandatory objects ID block end ---------- */

	/* ---------- Optional objects ID block begin ---------- */
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
	CONNECTIVITY_MONITORING = 4,
	#endif
	#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
	LWM2M_ACCESS_CONTROL = 2,
	#endif
	#ifdef OBJ_O_3339_AUDIO_CLIP
	AUDIO_CLIP = 3339,
	#endif
	#ifdef OBJ_O_5_FIRMWARE_UPDATE
	FIRMWARE_UPDATE = 5,
	#endif
	#ifdef OBJ_O_3201_DIGITAL_OUTPUT
	DIGITAL_OUTPUT = 3201,
	#endif
	#ifdef OBJ_O_3347_PUSH_BUTTON
	PUSH_BUTTON = 3347,
	#endif
	#ifdef OBJ_O_3303_TEMPERATURE
	TEMPERATURE = 3303,
	#endif
	#ifdef OBJ_O_3313_ACCELEROMETER
	ACCELEROMETER = 3313,
	#endif
	/* ---------- Optional objects ID block end ---------- */

	MAX_ID = ID_T_MAX_VAL,
};

}; //wpp

#endif // WPP_OBJECT_ID_H_
