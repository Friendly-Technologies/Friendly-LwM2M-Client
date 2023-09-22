/*
 * Resource.cpp
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#include "Resource.h"

namespace wpp {

/* ---------- Public methods for common usage ----------*/

WppResource::WppResource():
	_id(ID_T_MAX_VAL), _operation(), _isSingle(IS_SINGLE::MULTIPLE), _isMandatory(IS_MANDATORY::OPTIONAL), _typeID(TYPE_ID::UNDEFINED) {
	_resourceGuard.unlock();
}

WppResource::WppResource(ID_T id, const WppOperation &operation, IS_SINGLE isSingle, IS_MANDATORY isMandatory, TYPE_ID dataType):
	_id(id), _operation(operation), _isSingle(isSingle), _isMandatory(isMandatory), _typeID(dataType) {
	_resourceGuard.unlock();
}

WppResource::WppResource(const WppResource& resource) {
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

WppResource::WppResource(WppResource&& resource) {
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

ID_T WppResource::getID() const {
	return _id;
}

TYPE_ID WppResource::getTypeId() const {
	return _typeID;
}

const WppOperation& WppResource::getOperation() const {
	return _operation;
}

bool WppResource::isMandatory() const {
	return _isMandatory == IS_MANDATORY::MANDATORY;
}

bool WppResource::isOptional() const {
	return _isMandatory == IS_MANDATORY::OPTIONAL;
}

bool WppResource::isSingle() const {
	return _isSingle == IS_SINGLE::SINGLE;
}

bool WppResource::isMultiple() const {
	return _isSingle == IS_SINGLE::MULTIPLE;
}

bool WppResource::isOperationValid(WppOperation::TYPE type) const {
	return _operation.isSupported(type);
}

bool WppResource::isInstanceIdPossible(ID_T resInstId) const {
	return isMultiple() || resInstId == SINGLE_INSTANCE_ID;
}

bool WppResource::isInstanceExist(ID_T resInstId) const {
	return _instances.find(resInstId) != _instances.end();
}

bool WppResource::isTypeIdCompatible(TYPE_ID type) const {
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

bool WppResource::isEmpty() const {
	return _instances.size() == 0;
}

size_t WppResource::instanceCnt() const {
	return _instances.size();
}


/* ---------- Extended abilities for access directly to resource data for avoid coping ----------*/
const std::unordered_map<ID_T, WppResource::DATA_T>& WppResource::getInstances() {
	return _instances;
}

/* ---------- Methods for get and set resource value ----------*/
bool WppResource::set(const BOOL_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool WppResource::set(const INT_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool WppResource::set(const UINT_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool WppResource::set(const FLOAT_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool WppResource::set(const OPAQUE_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool WppResource::set(const OBJ_LINK_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool WppResource::set(const STRING_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool WppResource::set(const EXECUTE_T &value, ID_T resInstId) {
	return _set(value, resInstId);
}

bool WppResource::get(BOOL_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool WppResource::get(INT_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool WppResource::get(UINT_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool WppResource::get(FLOAT_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool WppResource::get(OPAQUE_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool WppResource::get(OBJ_LINK_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool WppResource::get(STRING_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool WppResource::get(EXECUTE_T &value, ID_T resInstId) const {
	return _get(value, resInstId);
}

bool WppResource::remove(ID_T resInstId) {
	std::lock_guard<std::mutex> guard(_resourceGuard);

	if (!isInstanceExist(resInstId) || isSingle()) return false;
	_instances.erase(resInstId);

	return true;
}

bool WppResource::clear() {
	std::lock_guard<std::mutex> guard(_resourceGuard);

	_instances.clear();

	return true;
}

bool WppResource::setDataVerifier(const DATA_VERIFIER_T &verifier) {
	if (!isDataVerifierValid(verifier)) return false;
	_dataVerifier = verifier;
	return true;
}

bool WppResource::isDataVerifierValid(const DATA_VERIFIER_T &verifier) const {
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


