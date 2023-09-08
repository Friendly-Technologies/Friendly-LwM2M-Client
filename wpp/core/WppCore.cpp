/*
 * WppManager.cpp
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#include "WppCore.h"

//TODO: #include "liblwm2m.h"

namespace wpp {

WppCore *WppCore::_manager = NULL;

WppCore::WppCore(const std::string &endpointName, const std::string &msisdn, const std::string &altPath, const OBJ_RESTORE_T &restoreFunc): _restoreFunc(restoreFunc) {
	lwm2mContextOpen();
	lwm2mConfigure(endpointName, msisdn, altPath);
}

WppCore::~WppCore() {
	lwm2mContextClose();
}

/* ------------- WppCore management ------------- */
bool WppCore::create(const std::string &endpointName, const std::string &msisdn, const std::string &altPath, const OBJ_RESTORE_T &restoreFunc) {
	_manager = new WppCore(endpointName, msisdn, altPath, restoreFunc);
	return true;
}

bool WppCore::isCreated() {
	return _manager != NULL;
}

WppCore* WppCore::instance() {
	return _manager;
}

/* ------------- Wakaama core state processing ------------- */
lwm2m_context_t * WppCore::getContext() {
	return _lwm2m_context;
}

lwm2m_client_state_t WppCore::getState() {
	return _lwm2m_context->state;
}

void WppCore::loop(time_t &sleepTime) {
	int result = 0;// TODO: lwm2m_step(_lwm2m_context, availableTime);
	if (!result && getState() == STATE_BOOTSTRAPPING) {
		restoreObject(WppRegistry::instance().security());
		restoreObject(WppRegistry::instance().server());
	}
}

bool WppCore::updateRegistration(Server &server, bool withObjects) {
	INT_T serverId;
	if (!server.get(serverId, Server::SHORT_SERV_ID)) return false;
	return true;// TODO: !lwm2m_update_registration(_lwm2m_context, serverId, withObjects);
}

bool WppCore::updateRegistration(bool withObjects) {
	return true;// TODO: !lwm2m_update_registration(_lwm2m_context, 0, withObjects);
}

void WppCore::deregister() {
	// TODO: lwm2m_deregister(_lwm2m_context);
}


/* ------------- Wakaama core object managing ------------- */
bool WppCore::registerObject(Lwm2mObject &object) {
	return true;// TODO: !lwm2m_add_object(_lwm2m_context, &object.getLwm2mObject());
}

bool WppCore::deregisterObject(Lwm2mObject &object) {
	return true;// TODO: !lwm2m_remove_object(_lwm2m_context, &object.getLwm2mObject().objID);
}

bool WppCore::isObjectRegistered(Lwm2mObject &object) {
	lwm2m_object_t * lwm2m_object = NULL; // TODO: (lwm2m_object_t *)LWM2M_LIST_FIND(_lwm2m_context->objectList, object.getLwm2mObject().objID);
	return lwm2m_object != NULL;
}

void WppCore::restoreObject(Lwm2mObject &object) {
	if (_restoreFunc) _restoreFunc(object);
	else object.clear(); // TODO: Validate this behaviour
}


/* ------------- Wakaama core observer notify ------------- */
void WppCore::notifyValueChanged(const DataID &data) {
	lwm2m_uri_t uri = {data.instance.objectId, data.instance.instanceId, data.resource.resourceId, data.resource.resourceInstanceId};
	// TODO: lwm2m_resource_value_changed(_lwm2m_context, &uri);
}


/* ------------- Wakaama core initialisation ------------- */
bool WppCore::lwm2mContextOpen() {
	_lwm2m_context = new lwm2m_context_t; // TODO: lwm2m_init(this);
	return _lwm2m_context != NULL;
}

void WppCore::lwm2mContextClose() {
	// TODO: lwm2m_close(_lwm2m_context);
	_lwm2m_context = NULL;
}

bool WppCore::lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath) {
//TODO:	lwm2m_object_t lwm2m_major_objects[] = {&WppRegistry::instance().security().getLwm2mObject(),
//										  	    &WppRegistry::instance().server().getLwm2mObject(),
//										        &WppRegistry::instance().device().getLwm2mObject()};
//	uint16_t objectsCnt = sizeof(lwm2m_major_objects)/sizeof(lwm2m_object_t);
//	char *msisdn_c = msisdn.empty()? NULL : msisdn.c_str();
//	char *altPath_c = altPath.empty()? NULL : altPath.c_str();
//	return !lwm2m_configure(_lwm2m_context, endpointName.c_str(), msisdn_c, altPath_c, objectsCnt, lwm2m_major_objects);
	return true;
}

} /* namespace wpp */
