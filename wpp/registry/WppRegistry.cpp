#include "WppRegistry.h"
#include "WppLogs.h"
#include "liblwm2m.h"

namespace wpp {

WppRegistry::WppRegistry(lwm2m_context_t &context): _context(context) {
    WPP_LOGD(TAG_WPP_REG, "Creating registry instance");
	
	/* ---------- Mandatory objects init block begin ---------- */
	#ifdef OBJ_M_3_DEVICE_V12
	_objects.push_back(new ObjectSpec<Device>(_context, DEVICE_OBJ_INFO));
	#endif
	#ifdef OBJ_M_1_LWM2M_SERVER_V11
	_objects.push_back(new ObjectSpec<Lwm2mServer>(_context, LWM2M_SERVER_OBJ_INFO));
	#endif
	#ifdef OBJ_M_0_LWM2M_SECURITY_V11
	_objects.push_back(new ObjectSpec<Lwm2mSecurity>(_context, LWM2M_SECURITY_OBJ_INFO));
	#endif
	/* ---------- Mandatory objects init block end ---------- */

	/* ---------- Optional objects init block begin ---------- */
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING_V13
	_objects.push_back(new ObjectSpec<ConnectivityMonitoring>(_context, CONNECTIVITY_MONITORING_OBJ_INFO));
	#endif
	#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
	_objects.push_back(new ObjectSpec<Lwm2mAccessControl>(_context, LWM2M_ACCESS_CONTROL_OBJ_INFO));
	#endif
	#if OBJ_O_3339_AUDIO_CLIP_V10
	_objects.push_back(new ObjectSpec<AudioClip>(_context, AUDIO_CLIP_OBJ_INFO));
	#endif
	#if OBJ_O_5_FIRMWARE_UPDATE_V10
	_objects.push_back(new ObjectSpec<FirmwareUpdate>(_context, FIRMWARE_UPDATE_OBJ_INFO));
	#endif
	/* ---------- Optional objects init block end ---------- */
}

WppRegistry::~WppRegistry() {
	for (auto obj : _objects) {
		delete obj;
	}
	_objects.clear();
}

bool WppRegistry::registerObj(Object &object) {
	WPP_LOGD(TAG_WPP_CLIENT, "Register object with id: %d", object.getObjectID());
	return !lwm2m_add_object(&_context, &object.getLwm2mObject());
}

bool WppRegistry::deregisterObj(Object &object) {
	WPP_LOGD(TAG_WPP_CLIENT, "Deregister object with id: %d", object.getObjectID());
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

/* ---------- Mandatory objects method block begin ---------- */
#ifdef OBJ_M_3_DEVICE_V12
ObjectSpec<Device> & WppRegistry::device() {
	return *static_cast<ObjectSpec<Device>*>(object(OBJ_ID::DEVICE));
}
#endif
#ifdef OBJ_M_1_LWM2M_SERVER_V11
ObjectSpec<Lwm2mServer> & WppRegistry::lwm2mServer() {
	return *static_cast<ObjectSpec<Lwm2mServer>*>(object(OBJ_ID::LWM2M_SERVER));
}
#endif
#ifdef OBJ_M_0_LWM2M_SECURITY_V11
ObjectSpec<Lwm2mSecurity> & WppRegistry::lwm2mSecurity() {
	return *static_cast<ObjectSpec<Lwm2mSecurity>*>(object(OBJ_ID::LWM2M_SECURITY));
}
#endif
/* ---------- Mandatory objects method block end ---------- */

/* ---------- Optional objects method block begin ---------- */
#ifdef OBJ_O_4_CONNECTIVITY_MONITORING_V13
ObjectSpec<ConnectivityMonitoring> & WppRegistry::connectivityMonitoring() {
	return *static_cast<ObjectSpec<ConnectivityMonitoring>*>(object(OBJ_ID::CONNECTIVITY_MONITORING));
}
#endif
#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
ObjectSpec<Lwm2mAccessControl> & WppRegistry::lwm2mAccessControl() {
	return *static_cast<ObjectSpec<Lwm2mAccessControl>*>(object(OBJ_ID::LWM2M_ACCESS_CONTROL));
}
#endif
#if OBJ_O_3339_AUDIO_CLIP_V10
ObjectSpec<AudioClip> & WppRegistry::audioClip() {
	return *static_cast<ObjectSpec<AudioClip>*>(object(OBJ_ID::AUDIO_CLIP));
}
#endif
#if OBJ_O_5_FIRMWARE_UPDATE_V10
ObjectSpec<FirmwareUpdate> & WppRegistry::firmwareUpdate() {
	return *static_cast<ObjectSpec<FirmwareUpdate>*>(object(OBJ_ID::FIRMWARE_UPDATE));
}
#endif
/* ---------- Optional objects method block end ---------- */


} //wpp
