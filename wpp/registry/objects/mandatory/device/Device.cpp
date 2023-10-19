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
#include "Operation.h"
#include "types.h"
#include "WppLogs.h"

#define TAG "Device"

namespace wpp {

Device::Device(WppClient &client, const InstanceID &id): IInstance(client, id) {
	resourcesInit();
}

/* ---------------IInstance implementation part --------------- */
Resource * Device::getResource(ID_T id) {
	return NULL;
}

std::vector<Resource *> Device::getResourcesList() {
	std::vector<Resource *> list;
	return list;
}

std::vector<Resource *> Device::getResourcesList(const Operation& filter) {
	std::vector<Resource *> list;
	return list;
}

std::vector<Resource *> Device::getInstantiatedResourcesList() {
	std::vector<Resource *> list;
	return list;
}

std::vector<Resource *> Device::getInstantiatedResourcesList(const Operation& filter) {
	std::vector<Resource *> list;
	return list;
}

void Device::serverOperationNotifier(Operation::TYPE type, const ResourceID &resId) {
	observerNotify(*this, resId, type);
	switch (type) {
	case Operation::READ:
		WPP_LOGD_ARG(TAG, "Server READ -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case Operation::WRITE:
		WPP_LOGD_ARG(TAG, "Server WRITE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case Operation::EXECUTE:
		WPP_LOGD_ARG(TAG, "Server EXECUTE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case Operation::DISCOVER:
		WPP_LOGD_ARG(TAG, "Server DISCOVER -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case Operation::DELETE:
		WPP_LOGD_ARG(TAG, "Server DELETE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	default: break;
	}
}

void Device::userOperationNotifier(Operation::TYPE type, const ResourceID &resId) {
	switch (type) {
	case Operation::READ:
		WPP_LOGD_ARG(TAG, "User READ -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case Operation::WRITE:
		WPP_LOGD_ARG(TAG, "User WRITE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	case Operation::DELETE:
		WPP_LOGD_ARG(TAG, "User DELETE -> resId: %d, resInstId: %d\n", resId.resId, resId.resInstId);
		break;
	default: break;
	}
}

/* --------------- Class private methods --------------- */
void Device::resourcesInit() {
}

} /* namespace wpp */