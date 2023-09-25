/*
 * WppManager.cpp
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#include "WppClient.h"
#include "WppRegistry.h"
#include "IWppConnection.h"
#include "WppLogs.h"

namespace wpp {

WppClient *WppClient::_client = NULL;
std::mutex WppClient::_clientGuard;

WppClient::WppClient(IWppConnection &connection): _connection(connection) {
	 _registry = new WppRegistry(*this);
	lwm2mContextOpen();
}

WppClient::~WppClient() {
	delete _registry;
	lwm2mContextClose();
}

/* ------------- WppClient management ------------- */
bool WppClient::create(const ClientInfo &info, IWppConnection &connection) {
	if (isCreated()) return true;
	
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Creating WppClient instance with info: endpoint->%s, msisdn->%s, altPath->%s", info.endpointName.c_str(), info.msisdn.c_str(), info.altPath.c_str());
	_client = new WppClient(connection);
	
	// Try to configure client with user data
	bool result = _client->lwm2mConfigure(info.endpointName, info.msisdn, info.altPath);
	if (!result) {
		WPP_LOGE(TAG_WPP_CLIENT, "Error during client configuration");
		delete _client;
		_client = NULL;
	}

	return result;
}

bool WppClient::isCreated() {
	return _client != NULL;
}

WppClient* WppClient::takeOwnership() {
	// WPP_LOGD(TAG_WPP_CLIENT, "Taking ownership of client instance");
    if (!_clientGuard.try_lock()) return NULL;
	// WPP_LOGD(TAG_WPP_CLIENT, "Lock acquired, transferring ownership");
    return _client;
}

void WppClient::giveOwnership() {
	// WPP_LOGD(TAG_WPP_CLIENT, "Giving ownership of client instance");
    _clientGuard.unlock();
}


/* ------------- WppClient components ------------- */
IWppConnection & WppClient::connection() {
	return _connection;
}

WppRegistry & WppClient::registry() {
	return *_registry;
}

/* ------------- Wakaama core state processing ------------- */
lwm2m_client_state_t WppClient::getState() {
	return _lwm2m_context->state;
}

time_t WppClient::loop() {
	static time_t sleepTime = WppPlatform::getTime();

	WPP_LOGD(TAG_WPP_CLIENT, "Handling server packets if they exists");
	// Handle packets retreived from server
	if (connection().getPacketQueueSize()) connection().handlePacketsInQueue(getContext());

	// Handle wakaama core state
	int result = lwm2m_step(_lwm2m_context, &sleepTime);
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Processing internal state: result -> %d, state -> %d", result, getState());
	if (!result && getState() == STATE_BOOTSTRAPPING) {
		WPP_LOGW(TAG_WPP_CLIENT, "Trying to restore security and server objects");
		registry().security().restore();
		registry().server().restore();
	}

	return sleepTime;
}

bool WppClient::updateServerRegistration(INT_T serverId, bool withObjects) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Update registration to server: ID -> %d, withObjects -> %d", serverId, withObjects);
	return !lwm2m_update_registration(_lwm2m_context, serverId, withObjects);
}

bool WppClient::updateServerRegistration(bool withObjects) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Update registration to each server: withObjects -> %d", withObjects);
	return !lwm2m_update_registration(_lwm2m_context, 0, withObjects);
}

void WppClient::deregister() {
	WPP_LOGI(TAG_WPP_CLIENT, "Unregister with each server");
	lwm2m_deregister(_lwm2m_context);
}


/* ------------- Wakaama core object managing ------------- */
bool WppClient::registerObject(Lwm2mObject &object) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Register object with id: %d", object.getObjectID());
	return !lwm2m_add_object(_lwm2m_context, &object.getLwm2mObject());
}

bool WppClient::deregisterObject(Lwm2mObject &object) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Deregister object with id: %d", object.getObjectID());
	return !lwm2m_remove_object(_lwm2m_context, object.getLwm2mObject().objID);
}

bool WppClient::isObjectRegistered(Lwm2mObject &object) {
	lwm2m_object_t * lwm2m_object = (lwm2m_object_t *)LWM2M_LIST_FIND(_lwm2m_context->objectList, object.getLwm2mObject().objID);
	return lwm2m_object != NULL;
}


/* ------------- Wakaama core observer notify ------------- */
void WppClient::notifyValueChanged(const DataID &data) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Notify value changed: objID=%d, instID=%d, resID=%d, resInstID=%d", data.instance.objId, data.instance.objInstId, data.resource.resId, data.resource.resInstId);	
	lwm2m_uri_t uri = {data.instance.objId, data.instance.objInstId, data.resource.resId, data.resource.resInstId};
	lwm2m_resource_value_changed(_lwm2m_context, &uri);
}


/* ------------- Wakaama client initialisation ------------- */
bool WppClient::lwm2mContextOpen() {
	_lwm2m_context = lwm2m_init(this);
	return _lwm2m_context != NULL;
}

void WppClient::lwm2mContextClose() {
	lwm2m_close(_lwm2m_context);
	_lwm2m_context = NULL;
}

lwm2m_context_t * WppClient::getContext() {
	return _lwm2m_context;
}

bool WppClient::lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath) {
	lwm2m_object_t *lwm2m_major_objects[] = {&registry().security().getLwm2mObject(),
											&registry().server().getLwm2mObject(),
										    &registry().device().getLwm2mObject()};
	uint16_t objectsCnt = sizeof(lwm2m_major_objects) / sizeof(lwm2m_object_t *);
	const char *msisdn_c = msisdn.empty()? NULL : msisdn.c_str();
	const char *altPath_c = altPath.empty()? NULL : altPath.c_str();
	return !lwm2m_configure(_lwm2m_context, endpointName.c_str(), msisdn_c, altPath_c, objectsCnt, lwm2m_major_objects);
}

} /* namespace wpp */
