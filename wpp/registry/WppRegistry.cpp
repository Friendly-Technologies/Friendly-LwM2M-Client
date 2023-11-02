#include "WppRegistry.h"
#include "WppLogs.h"
#include "liblwm2m.h"

namespace wpp {

WppRegistry::WppRegistry(lwm2m_context_t &context): _context(context) {
    WPP_LOGD(TAG_WPP_REG, "Creating registry instance");
	
	/* ---------- Creating mandatory objects blok begin ---------- */
	# if OBJ_M_3_DEVICE_V12
	_objects.push_back(new ObjectImpl<Device>(_context, DEVICE_OBJ_INFO));
	# endif
	# if OBJ_M_0_LWM2M_SECURITY_V11
	_objects.push_back(new ObjectImpl<Lwm2mSecurity>(_context, LWM2M_SECURITY_OBJ_INFO));
	# endif
	# if OBJ_M_1_LWM2M_SERVER_V11
	_objects.push_back(new ObjectImpl<Lwm2mServer>(_context, LWM2M_SERVER_OBJ_INFO));
	#endif
	/* ---------- Creating mandatory objects blok end ---------- */

	/* ---------- Creating optional objects blok begin ---------- */
	# if OBJ_O_4_CONNECTIVITY_MONITORING_V13
	_objects.push_back(new ObjectImpl<ConnectivityMonitoring>(_context, CONNECTIVITY_MONITORING_OBJ_INFO));
	#endif
	/* ---------- Creating optional objects blok end ---------- */
}

WppRegistry::~WppRegistry() {
	for (auto obj : _objects) {
		delete obj;
	}
	_objects.clear();
}

bool WppRegistry::registerObj(Object &object) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Register object with id: %d", object.getObjectID());
	return !lwm2m_add_object(&_context, &object.getLwm2mObject());
}

bool WppRegistry::deregisterObj(Object &object) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Deregister object with id: %d", object.getObjectID());
	return !lwm2m_remove_object(&_context, object.getLwm2mObject().objID);
}

bool WppRegistry::isObjRegistered(Object &object) {
	lwm2m_object_t * lwm2m_object = (lwm2m_object_t *)LWM2M_LIST_FIND(_context.objectList, object.getLwm2mObject().objID);
	return lwm2m_object != NULL;
}

bool WppRegistry::isObjExist(OBJ_ID objId) {
	return object(objId) != NULL;
}

Object * WppRegistry::object(OBJ_ID objId) {
	auto finder = [objId](const Object *obj) -> bool { return obj->getObjectID() == objId; };
	auto objIter = std::find_if(_objects.begin(), _objects.end(), finder);
	return objIter != _objects.end()? *objIter : NULL;
}

/* The start of the prototypes of the mandatory objects. */
# if OBJ_M_3_DEVICE_V12
ObjectImpl<Device> & WppRegistry::device() {
	return *static_cast<ObjectImpl<Device>*>(object(OBJ_ID::DEVICE));
}
# endif
# if OBJ_M_0_LWM2M_SECURITY_V11
ObjectImpl<Lwm2mSecurity> & WppRegistry::lwm2mSecurity() {
	return *static_cast<ObjectImpl<Lwm2mSecurity>*>(object(OBJ_ID::LWM2M_SECURITY));
}
# endif
# if OBJ_M_1_LWM2M_SERVER_V11
ObjectImpl<Lwm2mServer> & WppRegistry::lwm2mServer() {
	return *static_cast<ObjectImpl<Lwm2mServer>*>(object(OBJ_ID::LWM2M_SERVER));
}
# endif
/* The end of the prototypes of the mandatory objects. */
/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE! */

/* The start of the prototypes of the optional objects. */
#if OPTIONAL_ACL_OBJ
#endif

#if OPTIONAL_FIRMWARE_UPD_OBJ
#endif
# if OBJ_O_4_CONNECTIVITY_MONITORING_V13
ObjectImpl<ConnectivityMonitoring> & WppRegistry::connectivityMonitoring() {
	return *static_cast<ObjectImpl<ConnectivityMonitoring>*>(object(OBJ_ID::CONNECTIVITY_MONITORING));
}
# endif
/* The end of the prototypes of the optional objects. */
/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE! */


} //wpp
