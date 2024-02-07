#ifndef WPP_OBJECT_ID_H_
#define WPP_OBJECT_ID_H_

#include "types.h"

namespace wpp {

enum class OBJ_ID: ID_T {
	/* ------------- Mandatory objects ID start ------------- */
	SECURITY = 0,
	SERVER = 1,
	DEVICE = 3,
	/* ------------- Mandatory objects ID end ------------- */

	/* ------------- Optional objects ID start ------------- */
	ACL = 2,
	CONN_MONITORING = 4,
	FIRMWARE_UPD = 5,
	/* ------------- Optional objects ID end ------------- */

	MAX_ID = ID_T_MAX_VAL,
};

}; //wpp

#endif // WPP_OBJECT_ID_H_