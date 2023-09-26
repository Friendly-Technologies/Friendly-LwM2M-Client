/*
 * Security.cpp
 *
 *  Created on: 24 Jul 2023
 *      Author: valentin
 */

#include <mandatory/security/Security.h>

#include <unordered_map>
#include <iostream>

#include "Resource.h"
#include "Operation.h"
#include "types.h"
#include "WppLogs.h"

#define TAG "Security"

namespace wpp {

Security::Security(WppClient &client, const InstanceID &id): Instance(client, id) {
	resourcesInit();
}

/* ---------------Instance implementation part --------------- */
Resource * Security::getResource(ID_T id) {
	// Check if resource ID is valid
	if (_resources.find(id) == _resources.end()) return NULL;
	return &_resources[id];
}

std::vector<Resource *> Security::getResourcesList() {
	std::vector<Resource *> list;
	for (auto &pair : _resources) list.push_back(&pair.second);
	return list;
}

std::vector<Resource *> Security::getResourcesList(const Operation& filter) {
	std::vector<Resource *> list;
	for (auto &pair : _resources) {
		if (filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);
	}
	return list;
}

std::vector<Resource *> Security::getInstantiatedResourcesList() {
	std::vector<Resource *> list;
	for (auto &pair : _resources) if (!pair.second.isEmpty()) list.push_back(&pair.second);
	return list;
}

std::vector<Resource *> Security::getInstantiatedResourcesList(const Operation& filter) {
	std::vector<Resource *> list;
	for (auto &pair : _resources) {
		if (!pair.second.isEmpty() && filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);
	}
	return list;
}

void Security::serverOperationNotifier(Operation::TYPE type, const ResourceID &resId) {
	observerNotify(*this, resId, type);
	switch (type) {
	case Operation::READ:
		WPP_LOGD_ARG(TAG, "Server READ -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case Operation::WRITE:
		WPP_LOGD_ARG(TAG, "Server WRITE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case Operation::EXECUTE:
		WPP_LOGD_ARG(TAG, "Server EXECUTE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case Operation::DISCOVER:
		WPP_LOGD_ARG(TAG, "Server DISCOVER -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case Operation::DELETE:
		WPP_LOGD_ARG(TAG, "Server DELETE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	default: break;
	}
}

void Security::userOperationNotifier(Operation::TYPE type, const ResourceID &resId) {
	switch (type) {
	case Operation::READ:
		WPP_LOGD_ARG(TAG, "User READ -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case Operation::WRITE:
		WPP_LOGD_ARG(TAG, "User WRITE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case Operation::DELETE:
		WPP_LOGD_ARG(TAG, "User DELETE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	default: break;
	}
}

/* --------------- Class private methods --------------- */
void Security::resourcesInit() {
	_resources[SERVER_URI].set((STRING_T)"");
	_resources[BOOTSTRAP_SERVER].set(false);
	_resources[SECURITY_MODE].set((INT_T)LWM2M_SECURITY_MODE_NONE);
	_resources[PUBLIC_KEY].set(OPAQUE_T());
	_resources[SERVER_PUBLIC_KEY].set(OPAQUE_T());
	_resources[SECRET_KEY].set(OPAQUE_T());
	_resources[SERVER_ID].set(INT_T(0));
}

} /* namespace wpp */
