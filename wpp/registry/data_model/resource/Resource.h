#ifndef WPP_RESOURCE_H
#define WPP_RESOURCE_H

#include <unordered_map>
#include <mutex>
#include <type_traits>
#include <variant>

#include "ObjectInfo.h"
#include "ResOp.h"
#include "types.h"

namespace wpp {

/*
 * TODO: For now resource took to many memory, the main consumers are string and unordered_map
 */
class Resource {
public: /* ---------- Public subtypes ----------*/
	/*
	 * Universal type for data
	 */
	using DATA_T = std::variant<BOOL_T, INT_T, UINT_T, FLOAT_T, OPAQUE_T, OBJ_LINK_T, STRING_T, EXECUTE_T>;

	/*
	 * Type for resource instance
	 */
	struct ResInst {
		ID_T id;
		DATA_T data;
	};

	/*
	 * Universal type for data validation functions
	 */
	using DATA_VERIFIER_T = std::variant<VERIFY_INT_T, VERIFY_UINT_T, VERIFY_FLOAT_T, VERIFY_OPAQUE_T, VERIFY_BOOL_T,
										  VERIFY_OBJ_LINK_T, VERIFY_STRING_T, VERIFY_EXECUTE_T/*, VERIFY_CORE_LINK_T*/>; // VERIFY_CORE_LINK_T the same as VERIFY_STRING_T therefore we use only VERIFY_STRING_T

public: /* ---------- Public methods for common usage ----------*/
    Resource();
    Resource(ID_T id, const ResOp &operation, IS_SINGLE isSingle, IS_MANDATORY isMandatory, TYPE_ID dataType);
    Resource(const Resource& resource);
    Resource(Resource&& resource);
	Resource& operator=(const Resource& other);
	Resource& operator=(Resource&& other);

	/* ---------- Methods for get resource metadata ----------*/
    ID_T getId() const;
    TYPE_ID getTypeId() const;
    const ResOp& getOperation() const;
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
	bool isOperationValid(ResOp::TYPE type) const;
	bool isInstanceIdPossible(ID_T resInstId) const;
	bool isInstanceExist(ID_T resInstId) const;
	bool isTypeIdCompatible(TYPE_ID type) const;
	bool isEmpty() const;
	size_t instanceCnt() const;

	/* ---------- Extended abilities for access directly to resource data for avoid coping ----------*/
	const std::vector<ResInst>& getInstances();

	/* ---------- Methods for manage resource data ----------*/
    bool set(const BOOL_T &value, ID_T resInstId = SINGLE_INSTANCE_ID);
    bool set(const INT_T &value, ID_T resInstId = SINGLE_INSTANCE_ID);
    bool set(const UINT_T &value, ID_T resInstId = SINGLE_INSTANCE_ID);
    bool set(const FLOAT_T &value, ID_T resInstId = SINGLE_INSTANCE_ID);
    bool set(const OPAQUE_T &value, ID_T resInstId = SINGLE_INSTANCE_ID);
    bool set(const OBJ_LINK_T &value, ID_T resInstId = SINGLE_INSTANCE_ID);
    bool set(const STRING_T &value, ID_T resInstId = SINGLE_INSTANCE_ID);
	bool set(const EXECUTE_T &value, ID_T resInstId = SINGLE_INSTANCE_ID);

    bool get(BOOL_T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const;
	bool get(INT_T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const;
	bool get(UINT_T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const;
	bool get(FLOAT_T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const;
	bool get(OPAQUE_T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const;
	bool get(OBJ_LINK_T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const;
	bool get(STRING_T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const;
	bool get(EXECUTE_T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const;

    /*
     * Disabling implicit conversions
     */
    template<typename T>
    bool set(const T &value, ID_T resInstId = SINGLE_INSTANCE_ID) = delete;
	template<typename T>
	bool get(T &value, ID_T resInstId = SINGLE_INSTANCE_ID) const  = delete;

	/*
	 * Remove resource instance if resource is multiple and instance exists,
	 * if the resource is SINGLE or it has the last instance remove is not
	 * possible. Because instantiated resources must have at least one instance.
	 */
	bool remove(ID_T resInstId);

	/*
	 * Remove all instances.
	 */
	bool clear();

	/* ---------- Methods for manage verifier ----------*/
	bool setDataVerifier(const DATA_VERIFIER_T &verifier);

private:
	std::vector<ResInst>::iterator getInstIter(ID_T resInstId) const;

    template<typename T>
	bool _set(const T &value, ID_T resInstId);

	template<typename T>
	bool _get(T &value, ID_T resInstId) const;

private: /* ---------- Private properties ----------*/
    ID_T _id;
    ResOp _operation;
    IS_SINGLE _isSingle;
    IS_MANDATORY _isMandatory;
    TYPE_ID _typeID;
    mutable std::vector<ResInst> _instances;
    DATA_VERIFIER_T _dataVerifier;
};


/* ---------- Implementation of template methods ----------*/
template<typename T>
bool Resource::isDataTypeValid() const {
	TYPE_ID typeID = dataTypeToID<T>();
	return typeID != TYPE_ID::UNDEFINED && isTypeIdCompatible(typeID);
}

template<typename T>
bool Resource::_set(const T &value, ID_T resInstId) {
	if (!isInstanceIdPossible(resInstId)) return false;
	if (!isDataValueValid(value)) return false;

	if (isInstanceExist(resInstId)) {
		auto instIter = getInstIter(resInstId);
		instIter->data = value;
	} else {
		_instances.push_back({resInstId, value});
	}

	return true;
}

template<typename T>
bool Resource::_get(T &value, ID_T resInstId) const {
	if (!isDataTypeValid<T>()) return false;
	if (!isInstanceExist(resInstId)) return false;

	auto instIter = getInstIter(resInstId);
	value = std::get<T>(instIter->data);

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

#endif //WPP_RESOURCE_H
