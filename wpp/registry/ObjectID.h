#ifndef WPP_OBJECT_ID_H_
#define WPP_OBJECT_ID_H_

#include "types.h"

namespace wpp {

enum class OBJ_ID: ID_T {
	/* The start of the IDs of the mandatory objects. */
#ifdef OBJ_M_3_DEVICE_V12
	DEVICE = 3,
#endif /* OBJ_M_3_DEVICE_V12 */
#ifdef OBJ_M_0_LWM2M_SECURITY_V11
	LWM2M_SECURITY = 0,
#endif /* OBJ_M_0_LWM2M_SECURITY_V11 */
#ifdef OBJ_M_1_LWM2M_SERVER_V11
	LWM2M_SERVER = 1,
#endif /* OBJ_M_1_LWM2M_SERVER_V11 */
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