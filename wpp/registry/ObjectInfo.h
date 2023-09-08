/*
 * info.h
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#ifndef OBJECTINFO_H_
#define OBJECTINFO_H_

#include <unordered_map>

#include "Operation.h"
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

struct ObjectInfo {
	std::string name;
	OBJ_ID objID;
	std::string urn;
	Version objVersion;
	Version lwm2mVersion;
	IS_SINGLE isSingle;
	IS_MANDATORY isMandatory;
	Operation operation;
};

}

#endif /* OBJECTINFO_H_ */
