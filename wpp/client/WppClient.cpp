/*
 * WppManager.cpp
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#include "WppClient.h"
#include "WppRegistry.h"
#include "WppConnection.h"
#include "WppLogs.h"
#include "WppTaskQueue.h"

namespace wpp {

WppGuard WppClient::_clientGuard;
WppClient *WppClient::_client = NULL;

WppClient::WppClient(WppConnection &connection, WppErrHandler errHandler): _connection(connection), _errHandler(errHandler) {
	lwm2mContextOpen();
	_registry = new WppRegistry(getContext());
}

WppClient::~WppClient() {
	WPP_LOGI(TAG_WPP_CLIENT, "Destroying WppClient");
	WPP_LOGI(TAG_WPP_CLIENT, "Destroying WppRegistry");
	delete _registry;
	_registry = NULL;
	WPP_LOGI(TAG_WPP_CLIENT, "Clearing wpp tasks");
	WppTaskQueue::hardReset();
	WPP_LOGI(TAG_WPP_CLIENT, "Clearing packet queue");
	connection().clearPacketQueue();
	WPP_LOGI(TAG_WPP_CLIENT, "Closing lwm2m context");
	lwm2mContextClose();
	_client = NULL;
}

/* ------------- WppClient management ------------- */
bool WppClient::create(const ClientInfo &info, WppConnection &connection, WppErrHandler errHandler) {
	if (isCreated()) return true;
	
	WPP_LOGD(TAG_WPP_CLIENT, "Creating WppClient instance with info: endpoint->%s, msisdn->%s, altPath->%s", info.endpointName.c_str(), info.msisdn.c_str(), info.altPath.c_str());
	_client = new WppClient(connection, errHandler);
	bool result = _client->lwm2mConfigure(info.endpointName, info.msisdn, info.altPath);
	if (!result) {
		WPP_LOGE(TAG_WPP_CLIENT, "Error during client configuration");
		delete _client;
		_client = NULL;
	}

	return result;
}

void WppClient::remove() {
	if (!isCreated()) return;
	WPP_LOGD(TAG_WPP_CLIENT, "Removing WppClient instance");
	_client->giveOwnership();
	delete _client;
	_client = NULL;
}

bool WppClient::isCreated() {
	return _client != NULL;
}

WppClient* WppClient::takeOwnership() {
	// WPP_LOGD(TAG_WPP_CLIENT, "Taking ownership of client instance");
    if (!isCreated() || !_clientGuard.try_lock()) return NULL;
	// WPP_LOGD(TAG_WPP_CLIENT, "Lock acquired, transferring ownership");
    return _client;
}

WppClient* WppClient::takeOwnershipBlocking() {
    if (!isCreated()) return NULL;
	// WPP_LOGD(TAG_WPP_CLIENT, "Taking ownership of client instance");
	_clientGuard.lock();
	// WPP_LOGD(TAG_WPP_CLIENT, "Lock acquired, transferring ownership");
    return _client;
}

void WppClient::giveOwnership() {
	// WPP_LOGD(TAG_WPP_CLIENT, "Giving ownership of client instance");
	_clientGuard.try_lock();
	_clientGuard.unlock();
}


/* ------------- WppClient components ------------- */
WppConnection & WppClient::connection() {
	return _connection;
}

WppRegistry & WppClient::registry() {
	return *_registry;
}

/* ------------- Wakaama core state processing ------------- */
lwm2m_client_state_t WppClient::getState() {
	return _lwm2m_context->state;
}

lwm2m_context_t & WppClient::getContext() {
	return *_lwm2m_context;
}

time_t WppClient::loop() {
	// Max sleep time
	time_t sleepTimeSec = WPP_CLIENT_MAX_SLEEP_TIME_S;

	WPP_LOGD(TAG_WPP_CLIENT, "Handling server packets if they exists");
	// Handles packets retreived from server
	if (connection().getPacketQueueSize()) {
		connection().handlePacketsInQueue(*this);
		// Handle observe step after handling packets for avoid missing notifications
		lwm2m_observe_step(_lwm2m_context);
	}

	WPP_LOGD(TAG_WPP_CLIENT, "Handling wpp tasks if they exists");
	// Handles Wpp tasks in the WppClient context and return next call interval
	if (WppTaskQueue::getTaskCnt()) {
		time_t nextTaskCallIntervalSec = WppTaskQueue::handleEachTask(*this);
		if (nextTaskCallIntervalSec < sleepTimeSec) sleepTimeSec = nextTaskCallIntervalSec;
	}

	// Handles wakaama core state
	int result = lwm2m_step(_lwm2m_context, &sleepTimeSec);
	WPP_LOGD(TAG_WPP_CLIENT, "Handling lwm2m internal state: result -> %d, state -> %d", result, getState());
	if (result) {
		WPP_LOGW(TAG_WPP_CLIENT, "LWM2M core step failed, error code: %d", result);
		if (_errHandler) _errHandler(*this, result);
		_lwm2m_context->state = STATE_INITIAL;
		sleepTimeSec = 0;
	}

	return sleepTimeSec;
}

/* ------------- WppClient server operations ------------- */
void WppClient::deregister() {
	WPP_LOGI(TAG_WPP_CLIENT, "Unregister with each server");
	lwm2m_deregister(_lwm2m_context);
}

#if defined(LWM2M_SUPPORT_SENML_JSON) && RES_1_23
bool WppClient::send(const DataLink &link) {
	WPP_LOGD(TAG_WPP_CLIENT, "Send data to servers: object ID -> %d, instance ID -> %d, resource ID -> %d, resource instance ID -> %d",
				link.instance.objId, link.instance.objInstId, link.resource.resId, link.resource.resInstId);
	lwm2m_uri_t uri = {link.instance.objId, link.instance.objInstId, link.resource.resId, link.resource.resInstId};
	return !lwm2m_send_operation(_lwm2m_context, &uri);
}
#endif

/* ------------- Wakaama client initialisation ------------- */
bool WppClient::lwm2mContextOpen() {
	_lwm2m_context = lwm2m_init(this);
	return _lwm2m_context != NULL;
}

void WppClient::lwm2mContextClose() {
	lwm2m_close(_lwm2m_context);
	_lwm2m_context = NULL;
}

bool WppClient::lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath) {
	lwm2m_object_t *lwm2m_major_objects[] = {&Lwm2mSecurity::object(*this).getLwm2mObject(),
											 &Lwm2mServer::object(*this).getLwm2mObject(),
										     &Device::object(*this).getLwm2mObject()};
	uint16_t objectsCnt = sizeof(lwm2m_major_objects) / sizeof(lwm2m_object_t *);
	const char *msisdn_c = msisdn.empty()? NULL : msisdn.c_str();
	const char *altPath_c = altPath.empty()? NULL : altPath.c_str();
	return !lwm2m_configure(_lwm2m_context, endpointName.c_str(), msisdn_c, altPath_c, objectsCnt, lwm2m_major_objects);
}

} /* namespace wpp */
