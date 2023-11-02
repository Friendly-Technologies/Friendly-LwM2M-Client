#ifndef WPP_OBJECT_ID_H_
#define WPP_OBJECT_ID_H_

#include "types.h"

namespace wpp {

enum class OBJ_ID: ID_T {
	/* ---------- Mandatory objects ID blok begin ---------- */
	#ifdef OBJ_M_3_DEVICE_V12
	DEVICE = 3,
	#endif
	#ifdef OBJ_M_1_LWM2M_SERVER_V11
	LWM2M_SERVER = 1,
	#endif
	#ifdef OBJ_M_0_LWM2M_SECURITY_V11
	LWM2M_SECURITY = 0,
	#endif
	/* ---------- Mandatory objects ID blok end ---------- */

	/* ---------- Optional objects ID blok begin ---------- */
	ACL = 2,
	FIRMWARE_UPD = 5,
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING_V13
	CONNECTIVITY_MONITORING = 4,
	#endif
	/* ---------- Optional objects ID blok end ---------- */

	MAX_ID = ID_T_MAX_VAL,
};

}; //wpp

#endif // WPP_OBJECT_ID_H_