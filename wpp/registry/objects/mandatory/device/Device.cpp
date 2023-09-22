/*
 * Device.cpp
 *
 *  Created on: 24 Jul 2023
 *      Author: valentin
 */

#include <mandatory/device/Device.h>

#include <unordered_map>
#include <iostream>

#include "Resource.h"
#include "WppOperation.h"
#include "types.h"
#include "WppLogs.h"

#define TAG "Device"

namespace wpp {

Device::Device(WppClient &client, const InstanceID &id): IWppInstance(client, id) {
	resourcesInit();
}

/* ---------------IWppInstance implementation part --------------- */
WppResource * Device::getResource(ID_T id) {
	return NULL;
}

std::vector<WppResource *> Device::getResourcesList() {
	std::vector<WppResource *> list;
	return list;
}

std::vector<WppResource *> Device::getResourcesList(const WppOperation& filter) {
	std::vector<WppResource *> list;
	return list;
}

std::vector<WppResource *> Device::getInstantiatedResourcesList() {
	std::vector<WppResource *> list;
	return list;
}

std::vector<WppResource *> Device::getInstantiatedResourcesList(const WppOperation& filter) {
	std::vector<WppResource *> list;
	return list;
}

void Device::serverOperationNotifier(WppOperation::TYPE type, const ResourceID &resId) {
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

void Device::userOperationNotifier(WppOperation::TYPE type, const ResourceID &resId) {
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
void Device::resourcesInit() {
	_resources[REBOOT].set((STRING_T)"");
	_resources[ERROR_CODE].set((INT_T)0);
	_resources[SUPPORTED_BINDINGS].set((STRING_T)"U");
}

} /* namespace wpp */
