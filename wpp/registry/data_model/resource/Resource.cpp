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
	_id(ID_T_MAX_VAL), _operation(), _isSingle(IS_SINGLE::MULTIPLE), _isMandatory(IS_MANDATORY::OPTIONAL), _typeID(TYPE_ID::UNDEFINED) {
	_resourceGuard.unlock();
}

Resource::Resource(ID_T id, const Operation &operation, IS_SINGLE isSingle, IS_MANDATORY isMandatory, TYPE_ID dataType):
	_id(id), _operation(operation), _isSingle(isSingle), _isMandatory(isMandatory), _typeID(dataType) {
	_resourceGuard.unlock();
}

Resource::Resource(const Resource& resource) {
	_id = resource._id;
	_operation = resource._operation;
	_isSingle = resource._isSingle;
	_isMandatory = resource._isMandatory;
	_typeID = resource._typeID;
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
	_typeID = resource._typeID;
	_instances.insert(std::make_move_iterator(resource._instances.begin()), std::make_move_iterator(resource._instances.end()));
	_dataVerifier = resource._dataVerifier;
	// The guard not inherit original object guard state
	_resourceGuard.unlock();
}

ID_T Resource::getID() const {
	return _id;
}

TYPE_ID Resource::getTypeId() const {
	return _typeID;
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

bool Resource::isInstanceIdPossible(ID_T resInstId) const {
	return isMultiple() || resInstId == SINGLE_INSTANCE_ID;
}

bool Resource::isInstanceExist(ID_T resInstId) const {
	return _instances.find(resInstId) != _instances.end();
}

bool Resource::isTypeIdCompatible(TYPE_ID type) const {
	switch (type) {
	case TYPE_ID::INT:
	case TYPE_ID::TIME:
		return _typeID == TYPE_ID::INT || _typeID == TYPE_ID::TIME;

	case TYPE_ID::STRING:
	case TYPE_ID::CORE_LINK:
		return _typeID == TYPE_ID::STRING || _typeID == TYPE_ID::CORE_LINK;
	default: break;
	}

	return _typeID == type;
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
bool Resource::set(const BOOL_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool Resource::set(const INT_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool Resource::set(const UINT_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool Resource::set(const FLOAT_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool Resource::set(const OPAQUE_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool Resource::set(const OBJ_LINK_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool Resource::set(const STRING_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool Resource::set(const EXECUTE_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool Resource::get(BOOL_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool Resource::get(INT_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool Resource::get(UINT_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool Resource::get(FLOAT_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool Resource::get(OPAQUE_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool Resource::get(OBJ_LINK_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool Resource::get(STRING_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool Resource::get(EXECUTE_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool Resource::remove(ID_T resInstId) {
	std::lock_guard<std::mutex> guard(_resourceGuard);

	if (!isInstanceExist(resInstId) || isSingle()) return false;
	_instances.erase(resInstId);

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
	if (std::holds_alternative<VERIFY_BOOL_T>(verifier) && std::get<VERIFY_BOOL_T>(verifier)) return _typeID == TYPE_ID::BOOL;
	else if (std::holds_alternative<VERIFY_INT_T>(verifier) && std::get<VERIFY_INT_T>(verifier)) return _typeID == TYPE_ID::INT;
	else if (std::holds_alternative<VERIFY_UINT_T>(verifier) && std::get<VERIFY_UINT_T>(verifier)) return _typeID == TYPE_ID::UINT;
	else if (std::holds_alternative<VERIFY_FLOAT_T>(verifier) && std::get<VERIFY_FLOAT_T>(verifier)) return _typeID == TYPE_ID::FLOAT;
	else if (std::holds_alternative<VERIFY_OPAQUE_T>(verifier) && std::get<VERIFY_OPAQUE_T>(verifier)) return _typeID == TYPE_ID::OPAQUE;
	else if (std::holds_alternative<VERIFY_OBJ_LINK_T>(verifier) && std::get<VERIFY_OBJ_LINK_T>(verifier)) return _typeID == TYPE_ID::OBJ_LINK;
	else if (std::holds_alternative<VERIFY_STRING_T>(verifier) && std::get<VERIFY_STRING_T>(verifier)) return _typeID == TYPE_ID::STRING;
	else if (std::holds_alternative<VERIFY_EXECUTE_T>(verifier) && std::get<VERIFY_EXECUTE_T>(verifier)) return _typeID == TYPE_ID::EXECUTE;
	else return false;
}

} //namespace wpp

