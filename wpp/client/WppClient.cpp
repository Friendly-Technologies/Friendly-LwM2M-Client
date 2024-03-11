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

namespace wpp {

WppClient *WppClient::_client = NULL;
std::mutex WppClient::_clientGuard;

WppClient::WppClient(WppConnection &connection, time_t maxSleepTime): _connection(connection), _maxSleepTime(maxSleepTime) {
	lwm2mContextOpen();
	_registry = new WppRegistry(getContext());
}

WppClient::~WppClient() {
	WPP_LOGE(TAG_WPP_CLIENT, "Destroying WppClient");
	lwm2mContextClose();
	delete _registry;
	_client = NULL;
}

/* ------------- WppClient management ------------- */
bool WppClient::create(const ClientInfo &info, WppConnection &connection, time_t maxSleepTime) {
	if (isCreated()) return true;
	
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Creating WppClient instance with info: endpoint->%s, msisdn->%s, altPath->%s", info.endpointName.c_str(), info.msisdn.c_str(), info.altPath.c_str());
	_client = new WppClient(connection, maxSleepTime);
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

void WppClient::giveOwnership() {
	// WPP_LOGD(TAG_WPP_CLIENT, "Giving ownership of client instance");
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

time_t WppClient::loop() {
	// Max sleep time
	time_t sleepTime = _maxSleepTime;

	WPP_LOGD(TAG_WPP_CLIENT, "Handling server packets if they exists");
	// Handle packets retreived from server
	if (connection().getPacketQueueSize()) connection().handlePacketsInQueue(getContext());

	// Handle wakaama core state
	int result = lwm2m_step(_lwm2m_context, &sleepTime);
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Processing internal state: result -> %d, state -> %d", result, getState());
	if (result) {
		WPP_LOGW_ARG(TAG_WPP_CLIENT, "LWM2M core step failed, error code: %d", result);
		if (getState() == STATE_BOOTSTRAPPING || getState() == STATE_BOOTSTRAP_REQUIRED) {
			WPP_LOGW(TAG_WPP_CLIENT, "Trying to restore security and server objects");
			registry().lwm2mSecurity().restore();
			registry().lwm2mServer().restore();
		}
		_lwm2m_context->state = STATE_INITIAL;
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


/* ------------- Wakaama client initialisation ------------- */
bool WppClient::lwm2mContextOpen() {
	_lwm2m_context = lwm2m_init(this);
	return _lwm2m_context != NULL;
}

void WppClient::lwm2mContextClose() {
	lwm2m_close(_lwm2m_context);
	_lwm2m_context = NULL;
}

lwm2m_context_t & WppClient::getContext() {
	return *_lwm2m_context;
}

bool WppClient::lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath) {
	lwm2m_object_t *lwm2m_major_objects[] = {&registry().lwm2mSecurity().getLwm2mObject(),
											&registry().lwm2mServer().getLwm2mObject(),
										    &registry().device().getLwm2mObject()};
	uint16_t objectsCnt = sizeof(lwm2m_major_objects) / sizeof(lwm2m_object_t *);
	const char *msisdn_c = msisdn.empty()? NULL : msisdn.c_str();
	const char *altPath_c = altPath.empty()? NULL : altPath.c_str();
	return !lwm2m_configure(_lwm2m_context, endpointName.c_str(), msisdn_c, altPath_c, objectsCnt, lwm2m_major_objects);
}

} /* namespace wpp */