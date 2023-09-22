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
#include "WppOperation.h"
#include "types.h"
#include "WppLogs.h"

#define TAG "Security"

namespace wpp {

Security::Security(WppClient &client, const InstanceID &id): IWppInstance(client, id) {
	resourcesInit();
}

/* ---------------IWppInstance implementation part --------------- */
WppResource * Security::getResource(ID_T id) {
	return NULL;
}

std::vector<WppResource *> Security::getResourcesList() {
	std::vector<WppResource *> list;
	return list;
}

std::vector<WppResource *> Security::getResourcesList(const WppOperation& filter) {
	std::vector<WppResource *> list;
	return list;
}

std::vector<WppResource *> Security::getInstantiatedResourcesList() {
	std::vector<WppResource *> list;
	return list;
}

std::vector<WppResource *> Security::getInstantiatedResourcesList(const WppOperation& filter) {
	std::vector<WppResource *> list;
	return list;
}

void Security::serverOperationNotifier(WppOperation::TYPE type, const ResourceID &resId) {
	observerNotify(*this, resId, type);
	switch (type) {
	case WppOperation::READ:
		WPP_LOGD_ARG(TAG, "Server READ -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case WppOperation::WRITE:
		WPP_LOGD_ARG(TAG, "Server WRITE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case WppOperation::EXECUTE:
		WPP_LOGD_ARG(TAG, "Server EXECUTE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case WppOperation::DISCOVER:
		WPP_LOGD_ARG(TAG, "Server DISCOVER -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case WppOperation::DELETE:
		WPP_LOGD_ARG(TAG, "Server DELETE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	default: break;
	}
}

void Security::userOperationNotifier(WppOperation::TYPE type, const ResourceID &resId) {
	switch (type) {
	case WppOperation::READ:
		WPP_LOGD_ARG(TAG, "User READ -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case WppOperation::WRITE:
		WPP_LOGD_ARG(TAG, "User WRITE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case WppOperation::DELETE:
		WPP_LOGD_ARG(TAG, "User DELETE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
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
}

} /* namespace wpp */
