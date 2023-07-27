/*
 * Resource.cpp
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#include "Resource.h"

namespace wpp {

/* ---------- Public methods for common usage ----------*/

Resource::Resource():
	_id(ID_T_MAX_VAL), _operation(), _isSingle(IS_SINGLE::MULTIPLE), _isMandatory(IS_MANDATORY::OPTIONAL), _dataType(DATA_TYPE::UNDEFINED) {
	_resourceGuard.unlock();
}

Resource::Resource(ID_T id, const Operation &operation, IS_SINGLE isSingle, IS_MANDATORY isMandatory, DATA_TYPE dataType):
	_id(id), _operation(operation), _isSingle(isSingle), _isMandatory(isMandatory), _dataType(dataType) {
	_resourceGuard.unlock();
}

Resource::Resource(const Resource& resource) {
	_id = resource._id;
	_operation = resource._operation;
	_isSingle = resource._isSingle;
	_isMandatory = resource._isMandatory;
	_dataType = resource._dataType;
	_instances = resource._instances;
	_dataVerifier = resource._dataVerifier;
	// The guard not inherit original object guard state
	_resourceGuard.unlock();
}

Resource::Resource(Resource&& resource) {
	_id = resource._id;
	_operation = resource._operation;
	_isSingle = resource._isSingle;
	_isMandatory = resource._isMandatory;
	_dataType = resource._dataType;
	_instances.insert(std::make_move_iterator(resource._instances.begin()), std::make_move_iterator(resource._instances.end()));
	_dataVerifier = resource._dataVerifier; // TODO: Check whether copied validator still valid after deleting resource
	// The guard not inherit original object guard state
	_resourceGuard.unlock();
}

ID_T Resource::getID() const {
	return _id;
}

Resource::DATA_TYPE Resource::getDataType() const {
	return _dataType;
}

const Operation& Resource::getOperation() const {
	return _operation;
}

bool Resource::isMandatory() const {
	return _isMandatory == IS_MANDATORY::MANDATORY;
}

bool Resource::isOptional() const {
	return _isMandatory == IS_MANDATORY::OPTIONAL;
}

bool Resource::isSingle() const {
	return _isSingle == IS_SINGLE::SINGLE;
}

bool Resource::isMultiple() const {
	return _isSingle == IS_SINGLE::MULTIPLE;
}

bool Resource::isOperationValid(Operation::TYPE type) const {
	return _operation.isSupported(type);
}

bool Resource::isInstanceIdPossible(ID_T resourceInstanceId) const {
	return isMultiple() || resourceInstanceId == SINGLE_INSTANCE_ID;
}

bool Resource::isInstanceExist(ID_T resourceInstanceId) const {
	return _instances.find(resourceInstanceId) != _instances.end();
}

bool Resource::isDataTypeCompatible(DATA_TYPE type) const {
	switch (type) {
	case DATA_TYPE::INT:
	case DATA_TYPE::TIME:
		return _dataType == DATA_TYPE::INT || _dataType == DATA_TYPE::TIME;

	case DATA_TYPE::STRING:
	case DATA_TYPE::CORE_LINK:
		return _dataType == DATA_TYPE::STRING || _dataType == DATA_TYPE::CORE_LINK;
	default: break;
	}

	return _dataType == type;
}

bool Resource::isEmpty() const {
	return _instances.size() == 0;
}

size_t Resource::instanceCnt() const {
	return _instances.size();
}


/* ---------- Extended abilities for access directly to resource data for avoid coping ----------*/
std::unordered_map<ID_T,Resource::DATA_T>& Resource::getInstances() {
	return _instances;
}

bool Resource::takeOwnership() {
	return _resourceGuard.try_lock();
}

void Resource::giveOwnership() {
	_resourceGuard.unlock();
}

std::mutex& Resource::getGuard() {
	return _resourceGuard;
}


/* ---------- Methods for get and set resource value ----------*/
#ifndef __cpp_concepts
bool Resource::set(const BOOL_T &value, ID_T resourceInstanceId) {
	return _set(value, resourceInstanceId);
}

bool Resource::set(const INT_T &value, ID_T resourceInstanceId) {
	return _set(value, resourceInstanceId);
}

bool Resource::set(const UINT_T &value, ID_T resourceInstanceId) {
	return _set(value, resourceInstanceId);
}

bool Resource::set(const FLOAT_T &value, ID_T resourceInstanceId) {
	return _set(value, resourceInstanceId);
}

bool Resource::set(const OPAQUE_T &value, ID_T resourceInstanceId) {
	return _set(value, resourceInstanceId);
}

bool Resource::set(const OBJ_LINK_T &value, ID_T resourceInstanceId) {
	return _set(value, resourceInstanceId);
}

bool Resource::set(const STRING_T &value, ID_T resourceInstanceId) {
	return _set(value, resourceInstanceId);
}

bool Resource::set(const EXECUTE_T &value, ID_T resourceInstanceId) {
	return _set(value, resourceInstanceId);
}

bool Resource::get(BOOL_T &value, ID_T resourceInstanceId) const {
	return _get(value, resourceInstanceId);
}

bool Resource::get(INT_T &value, ID_T resourceInstanceId) const {
	return _get(value, resourceInstanceId);
}

bool Resource::get(UINT_T &value, ID_T resourceInstanceId) const {
	return _get(value, resourceInstanceId);
}

bool Resource::get(FLOAT_T &value, ID_T resourceInstanceId) const {
	return _get(value, resourceInstanceId);
}

bool Resource::get(OPAQUE_T &value, ID_T resourceInstanceId) const {
	return _get(value, resourceInstanceId);
}

bool Resource::get(OBJ_LINK_T &value, ID_T resourceInstanceId) const {
	return _get(value, resourceInstanceId);
}

bool Resource::get(STRING_T &value, ID_T resourceInstanceId) const {
	return _get(value, resourceInstanceId);
}

bool Resource::get(EXECUTE_T &value, ID_T resourceInstanceId) const {
	return _get(value, resourceInstanceId);
}
#endif // __cpp_concepts

bool Resource::remove(ID_T resourceInstanceId) {
	std::lock_guard<std::mutex> guard(_resourceGuard);

	if (!isInstanceExist(resourceInstanceId) || isSingle()) return false;
	_instances.erase(resourceInstanceId);

	return true;
}

bool Resource::clear() {
	std::lock_guard<std::mutex> guard(_resourceGuard);

	_instances.clear();

	return true;
}

bool Resource::setDataVerifier(const DATA_VERIFIER_T &verifier) {
	if (!isDataVerifierValid(verifier)) return false;
	_dataVerifier = verifier;
	return true;
}

bool Resource::isDataVerifierValid(const DATA_VERIFIER_T &verifier) const {
	if (std::holds_alternative<VERIFY_BOOL_T>(verifier) && std::get<VERIFY_BOOL_T>(verifier)) return _dataType == DATA_TYPE::BOOL;
	else if (std::holds_alternative<VERIFY_INT_T>(verifier) && std::get<VERIFY_INT_T>(verifier)) return _dataType == DATA_TYPE::INT;
	else if (std::holds_alternative<VERIFY_UINT_T>(verifier) && std::get<VERIFY_UINT_T>(verifier)) return _dataType == DATA_TYPE::UINT;
	else if (std::holds_alternative<VERIFY_FLOAT_T>(verifier) && std::get<VERIFY_FLOAT_T>(verifier)) return _dataType == DATA_TYPE::FLOAT;
	else if (std::holds_alternative<VERIFY_OPAQUE_T>(verifier) && std::get<VERIFY_OPAQUE_T>(verifier)) return _dataType == DATA_TYPE::OPAQUE;
	else if (std::holds_alternative<VERIFY_OBJ_LINK_T>(verifier) && std::get<VERIFY_OBJ_LINK_T>(verifier)) return _dataType == DATA_TYPE::OBJ_LINK;
	else if (std::holds_alternative<VERIFY_STRING_T>(verifier) && std::get<VERIFY_STRING_T>(verifier)) return _dataType == DATA_TYPE::STRING;
	else if (std::holds_alternative<VERIFY_EXECUTE_T>(verifier) && std::get<VERIFY_EXECUTE_T>(verifier)) return _dataType == DATA_TYPE::EXECUTE;
	else return false;
}

} //namespace wpp


