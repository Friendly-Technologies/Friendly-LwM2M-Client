/*
 * Resource.cpp
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#include "Resource.h"

#define RES_METHODS_IMPL_SET_FOR(_TYPE_)									\
bool Resource::set(const _TYPE_ &value, ID_T resInstId) {					\
	return _set(value, resInstId);											\
}																			\
bool Resource::setMove(_TYPE_ &value, ID_T resInstId) {				\
	return _setMove(value, resInstId);										\
}																			\
bool Resource::get(_TYPE_ &value, ID_T resInstId) const {					\
	return _get(value, resInstId);											\
}																			\
bool Resource::ptr(_TYPE_ **value, ID_T resInstId) {						\
	return _ptr(value, resInstId);											\
}																			\

namespace wpp {

/* ---------- Public methods for common usage ----------*/

Resource::Resource():
	_id(ID_T_MAX_VAL), _operation(), _isSingle(IS_SINGLE::MULTIPLE), _isMandatory(IS_MANDATORY::OPTIONAL), _typeID(TYPE_ID::UNDEFINED) {
}

Resource::Resource(ID_T id, const ResOp &operation, IS_SINGLE isSingle, IS_MANDATORY isMandatory, TYPE_ID dataType):
	_id(id), _operation(operation), _isSingle(isSingle), _isMandatory(isMandatory), _typeID(dataType) {
}

Resource::Resource(const Resource& resource) {
	_id = resource._id;
	_operation = resource._operation;
	_isSingle = resource._isSingle;
	_isMandatory = resource._isMandatory;
	_typeID = resource._typeID;
	_instances = resource._instances;
	_dataVerifier = resource._dataVerifier;
}

Resource::Resource(Resource&& resource) {
	_id = resource._id;
	_operation = resource._operation;
	_isSingle = resource._isSingle;
	_isMandatory = resource._isMandatory;
	_typeID = resource._typeID;
	_instances = std::move(resource._instances);
	_dataVerifier = resource._dataVerifier;
}

Resource& Resource::operator=(const Resource& resource) {
    if (this == &resource) return *this; 

    _id = resource._id;
    _operation = resource._operation;
    _isSingle = resource._isSingle;
    _isMandatory = resource._isMandatory;
    _typeID = resource._typeID;
    _instances = resource._instances;
    _dataVerifier = resource._dataVerifier;

    return *this;
}

Resource& Resource::operator=(Resource&& resource) {
    if (this == &resource) return *this;

    _id = resource._id;
    _operation = resource._operation;
    _isSingle = resource._isSingle;
    _isMandatory = resource._isMandatory;
    _typeID = resource._typeID;
    _instances.clear();
	_instances = std::move(resource._instances);
    _dataVerifier = resource._dataVerifier;
	resource.clear();

    return *this;
}

ID_T Resource::getId() const {
	return _id;
}

TYPE_ID Resource::getTypeId() const {
	return _typeID;
}

const ResOp& Resource::getOperation() const {
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

bool Resource::isOperationValid(ResOp::TYPE type) const {
	return _operation.isSupported(type);
}

bool Resource::isInstanceIdPossible(ID_T resInstId) const {
	return isMultiple() || resInstId == SINGLE_INSTANCE_ID;
}

bool Resource::isInstanceExist(ID_T resInstId) const {
	return getResInstIter(resInstId) != _instances.end();
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

const std::vector<ID_T> Resource::getInstIds() const {
	std::vector<ID_T> ids;
	ids.reserve(_instances.size());
	std::transform(_instances.begin(), _instances.end(), std::back_inserter(ids), [](const auto& inst) { return inst.id; });
	return ids;
}

/* ---------- Methods for get and set resource value ----------*/
RES_METHODS_IMPL_SET_FOR(BOOL_T);
RES_METHODS_IMPL_SET_FOR(INT_T);
RES_METHODS_IMPL_SET_FOR(UINT_T);
RES_METHODS_IMPL_SET_FOR(FLOAT_T);
RES_METHODS_IMPL_SET_FOR(OPAQUE_T);
RES_METHODS_IMPL_SET_FOR(OBJ_LINK_T);
RES_METHODS_IMPL_SET_FOR(STRING_T);
RES_METHODS_IMPL_SET_FOR(EXECUTE_T);

bool Resource::remove(ID_T resInstId) {
	if (!isInstanceExist(resInstId) || isSingle() || instanceCnt() == 1) return false;
	auto instForRemove = getResInstIter(resInstId);
	_instances.erase(instForRemove);

	return true;
}

bool Resource::clear() {
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
	// VERIFY_CORE_LINK_T the same as VERIFY_STRING_T therefore we use only VERIFY_STRING_T
	else if (std::holds_alternative<VERIFY_STRING_T>(verifier) && std::get<VERIFY_STRING_T>(verifier)) return _typeID == TYPE_ID::STRING || _typeID == TYPE_ID::CORE_LINK;
	else if (std::holds_alternative<VERIFY_EXECUTE_T>(verifier) && std::get<VERIFY_EXECUTE_T>(verifier)) return _typeID == TYPE_ID::EXECUTE;
	else return false;
}

std::vector<Resource::ResInst>::iterator Resource::getResInstIter(ID_T resInstId) const {
	auto finder = [&resInstId](const ResInst &inst) -> bool { return inst.id == resInstId; };
	return std::find_if(_instances.begin(), _instances.end(), finder);
}

} //namespace wpp


