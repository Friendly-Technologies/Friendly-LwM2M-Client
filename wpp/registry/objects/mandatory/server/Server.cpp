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
#include "ResOp.h"
#include "types.h"
#include "WppLogs.h"

#define TAG "Server"

namespace wpp {

Server::Server(WppClient &client, const OBJ_LINK_T &id): Instance(client, id) {
	resourcesInit();
}

/* ---------------Instance implementation part --------------- */
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

std::vector<Resource *> Server::getResourcesList(const ResOp& filter) {
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

std::vector<Resource *> Server::getInstantiatedResourcesList(const ResOp& filter) {
	std::vector<Resource *> list;
	for (auto &pair : _resources) {
		if (!pair.second.isEmpty() && filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);
	}
	return list;
}

void Server::setDefaultState() {
	for (auto &pair : _resources) pair.second.clear();
	resourcesInit();
}

void Server::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	observerNotify(*this, resId, type);
	switch (type) {
	case ResOp::READ:
		WPP_LOGD_ARG(TAG, "Server READ -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE_REPLACE_INST:
		WPP_LOGD_ARG(TAG, "Server WRITE_REPLACE_INST -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE_REPLACE_RES:
		WPP_LOGD_ARG(TAG, "Server WRITE_REPLACE_RES -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE_UPD:
		WPP_LOGD_ARG(TAG, "Server WRITE_UPD -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::EXECUTE:
		WPP_LOGD_ARG(TAG, "Server EXECUTE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::DISCOVER:
		WPP_LOGD_ARG(TAG, "Server DISCOVER -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	default: break;
	}
}

void Server::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	switch (type) {
	case ResOp::READ:
		WPP_LOGD_ARG(TAG, "User READ -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE:
		WPP_LOGD_ARG(TAG, "User WRITE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::DELETE:
		WPP_LOGD_ARG(TAG, "User DELETE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
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
