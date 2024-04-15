#ifndef WPP_OBJECT_INFO_H
#define WPP_OBJECT_INFO_H

#include "ItemOp.h"
#include "ObjectID.h"

namespace wpp {

/**
 * @brief The ObjectInfo struct represents information about an object in the Wakaama data model.
 * 
 * This struct contains various properties that describe an object, such as its name, ID, URN, versions,
 * and operation types. It is used to define the characteristics and behavior of objects in the Wakaama
 * data model.
 */
struct ObjectInfo {
	const char *name;
	OBJ_ID objID;
	const char *urn;
	Version objVersion;
	Version lwm2mVersion;
	IS_SINGLE isSingle;
	IS_MANDATORY isMandatory;
	ItemOp operations;
};

}; // wpp

#endif // WPP_OBJECT_INFO_H