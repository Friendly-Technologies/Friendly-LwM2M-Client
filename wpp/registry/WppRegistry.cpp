#include "WppRegistry.h"
#include "WppLogs.h"
#include "liblwm2m.h"

namespace wpp {

WppRegistry::WppRegistry(lwm2m_context_t &context): _context(context) {
    WPP_LOGD(TAG_WPP_REG, "Creating registry instance");
	
	/* ---------- Mandatory objects init block begin ---------- */
	#ifdef OBJ_M_3_DEVICE
	_objects.push_back(new ObjectImpl<Device>(_context, DEVICE_OBJ_INFO));
	#endif
	#ifdef OBJ_M_1_LWM2M_SERVER
	_objects.push_back(new ObjectImpl<Lwm2mServer>(_context, LWM2M_SERVER_OBJ_INFO));
	#endif
	#ifdef OBJ_M_0_LWM2M_SECURITY
	_objects.push_back(new ObjectImpl<Lwm2mSecurity>(_context, LWM2M_SECURITY_OBJ_INFO));
	#endif
	/* ---------- Mandatory objects init block end ---------- */

	/* ---------- Optional objects init block begin ---------- */
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
	_objects.push_back(new ObjectImpl<ConnectivityMonitoring>(_context, CONNECTIVITY_MONITORING_OBJ_INFO));
	#endif
	#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
	_objects.push_back(new ObjectImpl<Lwm2mAccessControl>(_context, LWM2M_ACCESS_CONTROL_OBJ_INFO));
	#endif
	#if OBJ_O_3339_AUDIO_CLIP
	_objects.push_back(new ObjectImpl<AudioClip>(_context, AUDIO_CLIP_OBJ_INFO));
	#endif
	#if OBJ_O_5_FIRMWARE_UPDATE
	_objects.push_back(new ObjectImpl<FirmwareUpdate>(_context, FIRMWARE_UPDATE_OBJ_INFO));
	#endif
	#if OBJ_O_3201_DIGITAL_OUTPUT
	_objects.push_back(new ObjectImpl<DigitalOutput>(_context, DIGITAL_OUTPUT_OBJ_INFO));
	#endif
	#if OBJ_O_3347_PUSH_BUTTON
	_objects.push_back(new ObjectImpl<PushButton>(_context, PUSH_BUTTON_OBJ_INFO));
	#endif
	#if OBJ_O_3303_TEMPERATURE
	_objects.push_back(new ObjectImpl<Temperature>(_context, TEMPERATURE_OBJ_INFO));
	#endif
	#if OBJ_O_3313_ACCELEROMETER
	_objects.push_back(new ObjectImpl<Accelerometer>(_context, ACCELEROMETER_OBJ_INFO));
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

bool WppRegistry::isExist(OBJ_ID objId) {
	return object(objId) != NULL;
}

Object * WppRegistry::object(OBJ_ID objId) {
	auto finder = [objId](const Object *obj) -> bool { return obj->getObjectID() == objId; };
	auto objIter = std::find_if(_objects.begin(), _objects.end(), finder);
	return objIter != _objects.end()? *objIter : NULL;
}

std::vector<Object *> & WppRegistry::objects() {
	return _objects;
}

/* ---------- Mandatory objects method block begin ---------- */
#ifdef OBJ_M_3_DEVICE
Object & WppRegistry::device() {
	return *object(OBJ_ID::DEVICE);
}
#endif
#ifdef OBJ_M_1_LWM2M_SERVER
Object & WppRegistry::lwm2mServer() {
	return *object(OBJ_ID::LWM2M_SERVER);
}
#endif
#ifdef OBJ_M_0_LWM2M_SECURITY
Object & WppRegistry::lwm2mSecurity() {
	return *object(OBJ_ID::LWM2M_SECURITY);
}
#endif
/* ---------- Mandatory objects method block end ---------- */

/* ---------- Optional objects method block begin ---------- */
#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
Object & WppRegistry::connectivityMonitoring() {
	return *object(OBJ_ID::CONNECTIVITY_MONITORING);
}
#endif
#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
Object & WppRegistry::lwm2mAccessControl() {
	return *object(OBJ_ID::LWM2M_ACCESS_CONTROL);
}
#endif
#if OBJ_O_3339_AUDIO_CLIP
Object & WppRegistry::audioClip() {
	return *object(OBJ_ID::AUDIO_CLIP);
}
#endif
#if OBJ_O_5_FIRMWARE_UPDATE
Object & WppRegistry::firmwareUpdate() {
	return *object(OBJ_ID::FIRMWARE_UPDATE);
}
#endif
#if OBJ_O_3201_DIGITAL_OUTPUT
Object & WppRegistry::digitalOutput() {
	return *object(OBJ_ID::DIGITAL_OUTPUT);
}
#endif
#if OBJ_O_3347_PUSH_BUTTON
Object & WppRegistry::pushButton() {
	return *object(OBJ_ID::PUSH_BUTTON);
}
#endif
#if OBJ_O_3303_TEMPERATURE
Object & WppRegistry::temperature() {
	return *object(OBJ_ID::TEMPERATURE);
}
#endif
#if OBJ_O_3313_ACCELEROMETER
Object & WppRegistry::accelerometer() {
	return *object(OBJ_ID::ACCELEROMETER);
}
#endif
/* ---------- Optional objects method block end ---------- */

} //wpp
