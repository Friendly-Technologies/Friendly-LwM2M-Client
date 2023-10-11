#ifndef WPP_OBJECT_ID_H_
#define WPP_OBJECT_ID_H_

#include "types.h"

namespace wpp {

enum class OBJ_ID: ID_T {
	/* The start of the IDs of the mandatory objects. */
#ifdef OBJ_DEVICE_1_2_M
	WPPDEVICE = 3,
#endif /* OBJ_DEVICE_1_2_M */
#ifdef OBJ_LWM2M_SECURITY_1_2_M
	WPPLWM2MSECURITY = 0,
#endif /* OBJ_LWM2M_SECURITY_1_2_M */
#ifdef OBJ_LWM2M_SERVER_1_2_M
	WPPLWM2MSERVER = 1,
#endif /* OBJ_LWM2M_SERVER_1_2_M */
	/* The end of the IDs of the mandatory objects. */
	/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!! */

	/* The start of the IDs of the optional objects. */
	ACL = 2,
	CONN_MONITORING = 4,
	FIRMWARE_UPD = 5,
	/* The end of the IDs of the optional objects. */
	/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!! */

	MAX_ID = ID_T_MAX_VAL,
};

}; //wpp

#endif // WPP_OBJECT_ID_H_