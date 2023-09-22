/*
 * Server.cpp
 *
 *  Created on: 7 Jul 2023
 *      Author: valentin
 */

#include "mandatory/server/Server.h"

#include <unordered_map>
#include <iostream>

#include "Resource.h"
#include "WppOperation.h"
#include "types.h"
#include "WppLogs.h"

#define TAG "Server"

namespace wpp {

Server::Server(WppClient &client, const InstanceID &id): IWppInstance(client, id) {
	resourcesInit();
}

/* ---------------IWppInstance implementation part --------------- */
WppResource * Server::getResource(ID_T id) {
	// Check if resource ID is valid
	if (_resources.find(id) == _resources.end()) return NULL;
	return &_resources[id];
}

std::vector<WppResource *> Server::getResourcesList() {
	std::vector<WppResource *> list;
	for (auto &pair : _resources) list.push_back(&pair.second);
	return list;
}

std::vector<WppResource *> Server::getResourcesList(const WppOperation& filter) {
	std::vector<WppResource *> list;
	for (auto &pair : _resources) {
		if (filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);
	}
	return list;
}

std::vector<WppResource *> Server::getInstantiatedResourcesList() {
	std::vector<WppResource *> list;
	for (auto &pair : _resources) if (!pair.second.isEmpty()) list.push_back(&pair.second);
	return list;
}

std::vector<WppResource *> Server::getInstantiatedResourcesList(const WppOperation& filter) {
	std::vector<WppResource *> list;
	for (auto &pair : _resources) {
		if (!pair.second.isEmpty() && filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);
	}
	return list;
}

void Server::serverOperationNotifier(WppOperation::TYPE type, const ResourceID &resId) {
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

void Server::userOperationNotifier(WppOperation::TYPE type, const ResourceID &resId) {
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
void Server::resourcesInit() {
	_resources[SHORT_SERV_ID].setDataVerifier((VERIFY_INT_T)([](const INT_T& value) { return 1 <= value && value <= 65534; }));
	_resources[SHORT_SERV_ID].set((INT_T)1);

	_resources[LIFETIME].set((INT_T)1);

	_resources[DISABLE].set((EXECUTE_T)([](ID_T id, const OPAQUE_T& data) { std::cout << "Server execute DISABLE!" << std::endl; }));

	_resources[NOTIFICATION_STORING].set(false);

	_resources[BUINDING].setDataVerifier((VERIFY_STRING_T)([](const STRING_T& value) { return value == std::string("U"); }));
	_resources[BUINDING].set((STRING_T)"U");

	_resources[REGISTRATION_UPDATE_TRIGGER].set((EXECUTE_T)([](ID_T id, const OPAQUE_T& data) { std::cout << "Server execute REGISTRATION_UPDATE_TRIGGER!" << std::endl; }));

	_resources[TRIGGER].setDataVerifier((VERIFY_BOOL_T)([](const BOOL_T& value) { return value == false; }));
	_resources[TRIGGER].set(false);
}

} /* namespace wpp */
