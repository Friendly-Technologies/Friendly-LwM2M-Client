#ifndef RESOURCE_H
#define RESOURCE_H

#include <unordered_map>
#include <mutex>
#include <type_traits>
#include <variant>

#include "ObjectInfo.h"
#include "Operation.h"
#include "types.h"

namespace wpp {

/*
 * TODO: For now resource took to many memory, the main consumers are string and unordered_map
 */
class Resource {
public: /* ---------- Public subtypes ----------*/
	enum class DATA_TYPE: uint8_t {
		BOOL,           // bool
		INT,            // int64_t
		UINT,           // uint64_t
		FLOAT,          // double
		OBJ_LINK,       // {object ID, instance ID}
		TIME,         	// Derived from INT
		OPAQUE,    		// vector<uint8_t>
		STRING,    		// string
		CORE_LINK, 		// Derived from STRING
		EXECUTE,		// Type of executable resources
		UNDEFINED     	// Undefined type
	};

	/*
	 * Universal type for data
	 */
	using DATA_T = std::variant<BOOL_T, INT_T, UINT_T, FLOAT_T, OPAQUE_T, OBJ_LINK_T, STRING_T, EXECUTE_T>;

	/*
	 * Universal type for data validation functions
	 */
	using DATA_VERIFIER_T = std::variant<VERIFY_INT_T, VERIFY_UINT_T, VERIFY_FLOAT_T, VERIFY_OPAQUE_T, VERIFY_BOOL_T,
										  VERIFY_OBJ_LINK_T, VERIFY_STRING_T, VERIFY_EXECUTE_T/*, VERIFY_CORE_LINK_T*/>; // VERIFY_CORE_LINK_T the same as VERIFY_STRING_T therefore we use only VERIFY_STRING_T

public: /* ---------- Public methods for common usage ----------*/
    Resource();
    Resource(ID_T id, const Operation &operation, IS_SINGLE isSingle, IS_MANDATORY isMandatory, DATA_TYPE dataType);
    Resource(const Resource& resource);
    Resource(Resource&& resource);

	/* ---------- Methods for get resource metadata ----------*/
    ID_T getID() const;
    DATA_TYPE getDataType() const;
    const Operation& getOperation() const;
    bool isMandatory() const;
    bool isOptional() const;
    bool isSingle() const;
    bool isMultiple() const;

    /* ---------- Helpful methods for check resource parameters ----------*/
	template<typename T>
	bool isDataTypeValid() const;
	template<typename T>
	bool isDataValueValid(const T &data) const;
	bool isDataVerifierValid(const DATA_VERIFIER_T &verifier) const;
	bool isOperationValid(Operation::TYPE type) const;
	bool isInstanceIdPossible(ID_T resourceInstanceId) const;
	bool isInstanceExist(ID_T resourceInstanceId) const;
	bool isDataTypeCompatible(DATA_TYPE type) const;
	bool isEmpty() const;
	size_t instanceCnt() const;

	/* ---------- Extended abilities for access directly to resource data for avoid coping ----------*/
	/*
	 * Before call of this method must be called takeOwnership()
	 * that must return true for guaranty unique property rights
	 * to the resource.
	 * After using of resource must be called giveOwnership() for
	 * get it available for other.
	 */
	std::unordered_map<ID_T, DATA_T>& getInstances();
	bool takeOwnership();
	void giveOwnership();
	std::mutex& getGuard();

	/* ---------- Methods for manage resource data ----------*/
    bool set(const BOOL_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID);
    bool set(const INT_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID);
    bool set(const UINT_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID);
    bool set(const FLOAT_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID);
    bool set(const OPAQUE_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID);
    bool set(const OBJ_LINK_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID);
    bool set(const STRING_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID);
	bool set(const EXECUTE_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID);

    bool get(BOOL_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const;
	bool get(INT_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const;
	bool get(UINT_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const;
	bool get(FLOAT_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const;
	bool get(OPAQUE_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const;
	bool get(OBJ_LINK_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const;
	bool get(STRING_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const;
	bool get(EXECUTE_T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const;

    /*
     * Disabling implicit conversions
     */
    template<typename T>
    bool set(const T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) = delete;
	template<typename T>
	bool get(T &value, ID_T resourceInstanceId = SINGLE_INSTANCE_ID) const  = delete;

	/*
	 * Remove resource instance if resource is multiple and instance exists,
	 * if the resource is SINGLE or it has the last instance remove is not
	 * possible. Because instantiated resources must have at least one instance.
	 */
	bool remove(ID_T resourceInstanceId);

	/*
	 * Remove all instances.
	 */
	bool clear();

	/* ---------- Methods for manage verifier ----------*/
	bool setDataVerifier(const DATA_VERIFIER_T &verifier);

private:
    template<typename T>
	DATA_TYPE _getDataTypeID() const;

    template<typename T>
	bool _set(const T &value, ID_T resourceInstanceId);

	template<typename T>
	bool _get(T &value, ID_T resourceInstanceId) const;

private: /* ---------- Private properties ----------*/
    ID_T _id;
    Operation _operation;
    IS_SINGLE _isSingle;
    IS_MANDATORY _isMandatory;
    DATA_TYPE _dataType;
    mutable std::unordered_map<ID_T, DATA_T> _instances;
    mutable std::mutex _resourceGuard;
    DATA_VERIFIER_T _dataVerifier;
};


/* ---------- Implementation of template methods ----------*/
template<typename T>
bool Resource::isDataTypeValid() const {
	DATA_TYPE typeID = _getDataTypeID<T>();
	return typeID != DATA_TYPE::UNDEFINED && isDataTypeCompatible(typeID);
}

#ifdef __cpp_concepts
template<Resource::ResourceDataType T>
bool set(const T &value, ID_T resourceInstanceId = 0) { return _set(id, value); }
template<Resource::ResourceDataType T>
bool get(T &value, ID_T resourceInstanceId = 0) { return _get(id, value); }
#endif // __cpp_concepts

template<typename T>
Resource::DATA_TYPE Resource::_getDataTypeID() const {
	DATA_TYPE typeID = DATA_TYPE::UNDEFINED;
	if constexpr (std::is_same<T, BOOL_T>::value) typeID = DATA_TYPE::BOOL;
	else if constexpr (std::is_same<T, INT_T>::value) typeID = DATA_TYPE::INT;
	else if constexpr (std::is_same<T, UINT_T>::value) typeID = DATA_TYPE::UINT;
	else if constexpr (std::is_same<T, FLOAT_T>::value) typeID = DATA_TYPE::FLOAT;
	else if constexpr (std::is_same<T, OPAQUE_T>::value) typeID = DATA_TYPE::OPAQUE;
	else if constexpr (std::is_same<T, OBJ_LINK_T>::value) typeID = DATA_TYPE::OBJ_LINK;
	else if constexpr (std::is_same<T, STRING_T>::value) typeID = DATA_TYPE::STRING;
	else if constexpr (std::is_same<T, EXECUTE_T>::value) typeID = DATA_TYPE::EXECUTE;

	return typeID;
}

template<typename T>
bool Resource::_set(const T &value, ID_T resourceInstanceId) {
	std::lock_guard<std::mutex> guard(_resourceGuard);

	if (!isInstanceIdPossible(resourceInstanceId)) return false;
	if (!isDataValueValid(value)) return false;

	_instances[resourceInstanceId] = value;

	return true;
}

template<typename T>
bool Resource::_get(T &value, ID_T resourceInstanceId) const {
	std::lock_guard<std::mutex> guard(_resourceGuard);

	if (!isDataTypeValid<T>()) return false;
	if (!isInstanceExist(resourceInstanceId)) return false;

	value = std::get<T>(_instances[resourceInstanceId]);

	return true;
}

template<typename T>
bool Resource::isDataValueValid(const T &data) const {
	if (!isDataTypeValid<T>()) return false;
	if (!isDataVerifierValid(_dataVerifier)) return true;

	if constexpr (std::is_same<T, BOOL_T>::value) {
		return std::get<VERIFY_BOOL_T>(_dataVerifier)(data);
	} else if constexpr (std::is_same<T, INT_T>::value) {
		return std::get<VERIFY_INT_T>(_dataVerifier)(data);
	} else if constexpr (std::is_same<T, UINT_T>::value) {
		return std::get<VERIFY_UINT_T>(_dataVerifier)(data);
	} else if constexpr (std::is_same<T, FLOAT_T>::value) {
		return std::get<VERIFY_FLOAT_T>(_dataVerifier)(data);
	} else if constexpr (std::is_same<T, OPAQUE_T>::value) {
		return std::get<VERIFY_OPAQUE_T>(_dataVerifier)(data);
	} else if constexpr (std::is_same<T, OBJ_LINK_T>::value) {
		return std::get<VERIFY_OBJ_LINK_T>(_dataVerifier)(data);
	} else if constexpr (std::is_same<T, STRING_T>::value) {
		return std::get<VERIFY_STRING_T>(_dataVerifier)(data);
	} else if constexpr (std::is_same<T, EXECUTE_T>::value) {
		return std::get<VERIFY_EXECUTE_T>(_dataVerifier)(data);
	} else {
		return false;
	}
}

} // namespace wpp

#endif //RESOURCE_H
