/*
 * Lwm2mObject.h
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#ifndef LWM2MOBJECT_H_
#define LWM2MOBJECT_H_

#include "types.h"
#include "ObjectInfo.h"

//TODO: #include "liblwm2m.h"
#include <dep.h>

namespace wpp {

// TODO: Add ability to get IInstance class by ID
class Lwm2mObject {
public:
	Lwm2mObject(const ObjectInfo &info): _objInfo(info) {}
	virtual ~Lwm2mObject() {}

	Lwm2mObject(const Lwm2mObject&) = delete;
	Lwm2mObject(Lwm2mObject&&) = delete;
	Lwm2mObject& operator=(const Lwm2mObject&) = delete;
	Lwm2mObject& operator=(Lwm2mObject&&) = delete;

	OBJ_ID getObjectId() { return _objInfo.objID; }
	lwm2m_object_t& getLwm2mObject() { return _lwm2m_object; }
	const ObjectInfo& getObjectInfo() { return _objInfo; }

	virtual size_t instanceCnt() = 0;
	virtual bool isInstanceExist(ID_T instanceID) = 0;

	virtual void clear() = 0;

protected:
	lwm2m_object_t _lwm2m_object;
	ObjectInfo _objInfo;
};

} // namespace wpp


#endif /* LWM2MOBJECT_H_ */
