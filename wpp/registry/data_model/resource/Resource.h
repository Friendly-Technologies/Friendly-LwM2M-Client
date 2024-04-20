#ifndef WPP_RESOURCE_H
#define WPP_RESOURCE_H

#include <unordered_map>
#include <type_traits>
#include <variant>

#include "ObjectInfo.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"

namespace wpp {

/**
 * @brief The Resource class in the wpp namespace is a comprehensive and flexible
 * class designed to handle different types of data within a resource-oriented
 * architecture. It offers a range of functionalities including storing, retrieving,
 * and validating various data types, as well as managing multiple instances of a resource. 
 * 
 * This class provides methods for managing and manipulating a resource, including
 * getting and setting resource metadata, checking resource parameters, managing
 * resource data, and managing verifiers for the resource data.
 * 
 * The Resource class contains public subtypes, such as the DATA_T type for storing
 * data of various types, the ResInst struct for representing a resource instance,
 * and the DATA_VERIFIER_T type for storing data validation functions.
 * 
 * The Resource class also provides public methods for common usage, such as
 * constructors, assignment operators, and methods for retrieving resource metadata.
 * 
 * Additionally, the Resource class provides helpful methods for checking resource
 * parameters, managing resource data, and managing verifiers. These methods include
 * checking the validity of the data type, checking the validity of a data value,
 * checking the validity of a data verifier, checking the validity of an operation,
 * checking the possibility of an instance ID, checking the existence of an instance,
 * checking the compatibility of a type ID, checking if the resource is empty,
 * retrieving the number of resource instances, and getting the available IDs of
 * resource instances.
 * 
 * The Resource class also provides methods for managing resource data, including
 * generating prototypes for getting and setting data of various supported types,
 * disabling implicit conversions, setting data values, getting data values, and
 * getting pointers to data values.
 * 
 * Furthermore, the Resource class provides methods for removing resource instances,
 * clearing all instances, and setting the data verifier for the resource.
 */
class Resource {
public: /* ---------- Public subtypes ----------*/
	/**
 	 * @brief Universal type for data
	 */
	using DATA_T = std::variant<BOOL_T, INT_T, UINT_T, FLOAT_T, OPAQUE_T, OBJ_LINK_T, STRING_T, EXECUTE_T>;

	/**
 	 * @brief Type for resource instance
	 */
	struct ResInst {
		ID_T id;
		DATA_T data;
	};

	/**
 	 * @brief Universal type for data validation functions
	 */
	using DATA_VERIFIER_T = std::variant<VERIFY_INT_T, VERIFY_UINT_T, VERIFY_FLOAT_T, VERIFY_OPAQUE_T, VERIFY_BOOL_T,
										  VERIFY_OBJ_LINK_T, VERIFY_STRING_T, VERIFY_EXECUTE_T/*, VERIFY_CORE_LINK_T*/>; // VERIFY_CORE_LINK_T the same as VERIFY_STRING_T therefore we use only VERIFY_STRING_T

public: /* ---------- Public methods for common usage ----------*/
    Resource();
    Resource(ID_T id, const ItemOp &operation, IS_SINGLE isSingle, IS_MANDATORY isMandatory, TYPE_ID dataType);
    Resource(const Resource& resource);
    Resource(Resource&& resource);
	Resource& operator=(const Resource& other);
	Resource& operator=(Resource&& other);

	/* ---------- Methods for get resource metadata ----------*/
    ID_T getId() const;
    TYPE_ID getTypeId() const;
    const ItemOp& getOperation() const;
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
	bool isOperationValid(ItemOp::TYPE type) const;
	bool isInstanceIdPossible(ID_T resInstId) const;
	bool isInstanceExist(ID_T resInstId) const;
	bool isTypeIdCompatible(TYPE_ID type) const;
	bool isEmpty() const;
	size_t instanceCnt() const;
	/**
 	 * @brief Returns vector with available ids of resource instances 
	 */
	const std::vector<ID_T> getInstIds() const;

	/* ---------- Methods for manage resource data ----------*/
    template<typename T>
    bool set(const T &value, ID_T resInstId = SINGLE_INSTANCE_ID);

	template<typename T>
	bool setMove(T &value, ID_T resInstId = SINGLE_INSTANCE_ID);

	template<typename T>
	const T& get(ID_T resInstId = SINGLE_INSTANCE_ID);

	/**
 	 * @brief Remove resource instance if resource is multiple and instance exists,
	 * if the resource is SINGLE or it has the last instance remove is not
	 * possible. Because instantiated resources must have at least one instance.
	 */
	bool remove(ID_T resInstId);

	/**
 	 * @brief Remove all instances.
	 */
	bool clear();

	/* ---------- Methods for manage verifier ----------*/
	bool setDataVerifier(const DATA_VERIFIER_T &verifier);

private:
	std::vector<ResInst>::iterator getResInstIter(ID_T resInstId) const;

private: /* ---------- Private properties ----------*/
    ID_T _id;
    ItemOp _operation;
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
bool Resource::set(const T &value, ID_T resInstId) {
	if (!isInstanceIdPossible(resInstId) || !isDataValueValid(value)) {
		WPP_LOGE(TAG_WPP_RES, "Invalid data value or instance id is not possible");
		return false;
	}

	if (isInstanceExist(resInstId)) {
		auto instIter = getResInstIter(resInstId);
		instIter->data = value;
	} else {
		_instances.push_back({resInstId, value});
	}

	return true;
}

template<typename T>
bool Resource::setMove(T &value, ID_T resInstId) {
	if (!isInstanceIdPossible(resInstId) || !isDataValueValid(value)) {
		WPP_LOGE(TAG_WPP_RES, "Invalid data value or instance id is not possible");
		return false;
	}

	if (isInstanceExist(resInstId)) {
		auto instIter = getResInstIter(resInstId);
		instIter->data = std::move(value);
	} else {
		ResInst newInst = {resInstId, std::move(value)};
		_instances.push_back(std::move(newInst));
	}

	return true;
}

template<typename T>
const T& Resource::get(ID_T resInstId) {
	if (!isDataTypeValid<T>() || !isInstanceExist(resInstId)) {
		// Return empty value if the data type is not valid or the instance does not exist
		static T empty;
		empty = T();
		WPP_LOGE(TAG_WPP_RES, "Invalid data type or instance does not exist");
		return empty;
	}

	auto instIter = getResInstIter(resInstId);
	return std::get<T>(instIter->data);
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
