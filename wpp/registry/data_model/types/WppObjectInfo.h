#ifndef WPP_OBJECT_INFO_H
#define WPP_OBJECT_INFO_H

#include "WppOperation.h"
#include "ObjectID.h"

namespace wpp {

struct WppObjectInfo {
	std::string name;
	OBJ_ID objID;
	std::string urn;
	Version objVersion;
	Version lwm2mVersion;
	IS_SINGLE isSingle;
	IS_MANDATORY isMandatory;
	WppOperation operation;
};

}; // wpp

#endif // WPP_OBJECT_INFO_H