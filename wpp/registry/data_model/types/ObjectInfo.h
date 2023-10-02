#ifndef WPP_OBJECT_INFO_H
#define WPP_OBJECT_INFO_H

#include "InstOp.h"
#include "ResOp.h"
#include "ObjectID.h"

namespace wpp {

struct ObjectInfo {
	std::string name;
	OBJ_ID objID;
	std::string urn;
	Version objVersion;
	Version lwm2mVersion;
	IS_SINGLE isSingle;
	IS_MANDATORY isMandatory;
	InstOp instOperation;
	ResOp resOperation;
};

}; // wpp

#endif // WPP_OBJECT_INFO_H