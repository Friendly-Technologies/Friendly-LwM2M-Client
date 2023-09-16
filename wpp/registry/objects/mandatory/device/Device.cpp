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

namespace wpp {

Device::Device(OBJ_ID objID, ID_T instanceID): IInstance(objID, instanceID) {
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

void Device::serverOperationNotifier(Operation::TYPE type, const ResourceID &resourceId) {
	switch (type) {
	case Operation::READ:
		std::cout << "Security READ -> resourceId:" << resourceId.resourceId << ", resourceInstanceId:" << resourceId.resourceInstanceId << std::endl;
		break;
	case Operation::WRITE:
		std::cout << "Security WRITE -> resourceId:" << resourceId.resourceId << ", resourceInstanceId:" << resourceId.resourceInstanceId << std::endl;
		break;
	case Operation::EXECUTE:
		std::cout << "Security EXECUTE -> resourceId:" << resourceId.resourceId << ", resourceInstanceId:" << resourceId.resourceInstanceId << std::endl;
		break;
	case Operation::DISCOVER:
		std::cout << "Security DISCOVER -> resourceId:" << resourceId.resourceId << ", resourceInstanceId:" << resourceId.resourceInstanceId << std::endl;
		break;
	case Operation::DELETE:
		std::cout << "Security DELETE -> resourceId:" << resourceId.resourceId << ", resourceInstanceId:" << resourceId.resourceInstanceId << std::endl;
		break;
	default: break;
	}
}

/* --------------- Class private methods --------------- */
void Device::resourcesInit() {
}

} /* namespace wpp */
