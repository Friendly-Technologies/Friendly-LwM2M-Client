#ifndef OBJECT_ID_H_
#define OBJECT_ID_H_

#include "types.h"

namespace wpp {

enum class OBJ_ID: ID_T {
	SECURITY = 0,
	SERVER = 1,
	ACL = 2,
	DEVICE = 3,
	CONN_MONITORING = 4,
	FIRMWARE_UPD = 5,
	MAX_ID = ID_T_MAX_VAL,
};

}; //wpp

#endif // OBJECT_ID_H_