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
#include "Operation.h"
#include "types.h"
#include "WppLogs.h"

#define TAG "Server"

namespace wpp {

Server::Server(OBJ_ID objID, ID_T instanceID): IInstance(objID, instanceID) {
	resourcesInit();
}

/* --------------- User helpful methods for manage resources data --------------- */

bool Server::clear(ID_T resId) {
	Resource *const resource = getResource(resId);
	if (!resource) return false;

	bool result = resource->clear();
	if (result) {
		userPerformedOperation(Operation::DELETE, resId);
	}

	return resource->clear();
}

bool Server::remove(ID_T resId, ID_T resInstId) {
	Resource *const resource = getResource(resId);
	if (!resource) return false;

	bool result = resource->remove(resInstId);
	if (result) {
		userPerformedOperation(Operation::DELETE, resId, resInstId);
	}

	return result;
}

void Server::userPerformedOperation(Operation::TYPE type, ID_T resId, ID_T resInstId) {
	switch (type) {
	case Operation::READ:
		WPP_LOGD_ARG(TAG, "User READ -> resId: %d, resInstId: %d\n", resId, resInstId);
		break;
	case Operation::WRITE:
		WPP_LOGD_ARG(TAG, "User WRITE -> resId: %d, resInstId: %d\n", resId, resInstId);
		break;
	case Operation::DELETE:
		WPP_LOGD_ARG(TAG, "User DELETE -> resId: %d, resInstId: %d\n", resId, resInstId);
		break;
	default: break;
	}
}

/* ---------------IInstance implementation part --------------- */
Resource * Server::getResource(ID_T id) {
	// Check if resource ID is valid
	if (_resources.find(id) == _resources.end()) return NULL;
	return &_resources[id];
}

std::vector<Resource *> Server::getResourcesList() {
	std::vector<Resource *> list;
	for (auto &pair : _resources) list.push_back(&pair.second);
	return list;
}

std::vector<Resource *> Server::getResourcesList(const Operation& filter) {
	std::vector<Resource *> list;
	for (auto &pair : _resources) {
		if (filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);
	}
	return list;
}

std::vector<Resource *> Server::getInstantiatedResourcesList() {
	std::vector<Resource *> list;
	for (auto &pair : _resources) if (!pair.second.isEmpty()) list.push_back(&pair.second);
	return list;
}

std::vector<Resource *> Server::getInstantiatedResourcesList(const Operation& filter) {
	std::vector<Resource *> list;
	for (auto &pair : _resources) {
		if (!pair.second.isEmpty() && filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);
	}
	return list;
}

void Server::serverOperationNotifier(Operation::TYPE type, const ResourceID &resId) {
	observerNotify(*this, resId, type);

	switch (type) {
	case Operation::READ:
		std::cout << "Server READ -> resId:" << resId.resId << ", resInstId:" << resId.resInstId << std::endl;
		break;
	case Operation::WRITE:
		std::cout << "Server WRITE -> resId:" << resId.resId << ", resInstId:" << resId.resInstId << std::endl;
		break;
	case Operation::EXECUTE:
		std::cout << "Server EXECUTE -> resId:" << resId.resId << ", resInstId:" << resId.resInstId << std::endl;
		break;
	case Operation::DISCOVER:
		std::cout << "Server DISCOVER -> resId:" << resId.resId << ", resInstId:" << resId.resInstId << std::endl;
		break;
	case Operation::DELETE:
		std::cout << "Server DELETE -> resId:" << resId.resId << ", resInstId:" << resId.resInstId << std::endl;
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
