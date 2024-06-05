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
	/**
	 * @brief Check if the data type and value are valid
	 * @param data The data value to check
	 * @note Type of data verifies according to the type of the resource,
	 * 		 value verifies according to the verifier of the resource that
	 *  	 can be set through the setDataVerifier method
	 * @return True if the data type and value are valid, false otherwise
	 */
	template<typename T>
	bool isDataValueValid(const T &data) const;

	/**
	 * @brief Check if the instance ID is exist
	 * @param resInstId The instance ID to check
	 * @note If the resource is SINGLE, the instance ID always SINGLE_INSTANCE_ID
	 * @return True if the instance ID is exist, false otherwise
	 */
	bool isExist(ID_T resInstId) const;

	/**
	 * @brief Get the number of resource instances
	 * @note If the resource is SINGLE, the number of instances is always 1
	 * @return The number of resource instances
	 */
	size_t instCount() const;

	/**
 	 * @brief Returns vector with available ids of resource instances
	 * @note If the resource is SINGLE, the vector will be contain only one
	 * 		 element with value SINGLE_INSTANCE_ID
	 */
	std::vector<ID_T> instIds() const;

	/**
	 * @brief Find first available instance ID that is not used
	 * @note If the resource is SINGLE, the method will return ID_T_MAX_VAL
	 * @return The first available instance ID else ID_T_MAX_VAL
	 */
	ID_T newInstId() const;

	/* ---------- Methods for manage verifier ----------*/
	/**
	 * @brief Set data verifier for the resource
	 * @param verifier Function that verifies the data value it will be
	 * 				   called each time when the method set is called
	 * @return True if the verifier is valid and set, false otherwise
	 */
	bool setDataVerifier(const DATA_VERIFIER_T &verifier);

	/* ---------- Methods for manage resource data ----------*/
	/**
	 * @brief Set data value by copy for the resource (instance)
	 * @param value The data value to set 
	 * @param resInstId The instance ID to set the value, used only for multiple resources
	 * @note If set the verifier then the value will be checked, also before set new value 
	 * 		 its type will be checked. If resource is SINGLE then resInstId should be set
	 * 		 to SINGLE_INSTANCE_ID.
	 * @return True if the value is set, false otherwise
	 */
    template<typename T>
    bool set(const T &value, ID_T resInstId = SINGLE_INSTANCE_ID);

	/**
	 * @brief Set data value by move for the resource (instance)
	 * @param value The data value to set 
	 * @param resInstId The instance ID to set the value, used only for multiple resources
	 * @note If set the verifier then the value will be checked, also before set new value 
	 * 		 its type will be checked. If resource is SINGLE then resInstId should be set
	 * 		 to SINGLE_INSTANCE_ID.
	 * @return True if the value is set, false otherwise
	 */
	template<typename T>
	bool set(T &&value, ID_T resInstId = SINGLE_INSTANCE_ID);

	/**
	 * @brief Get data value by reference for the resource (instance)
	 * @param resInstId The instance ID to get the value
	 * @example For example, if the resource is of type INT_T and SINGLE, the value can be retrieved as follows:
	 * @code INT_T value = resource.get<INT_T>(); @endcode
	 * @example For example, if the resource is of type STRING_T and MULTIPLE, the value can be retrieved as follows:
	 * @code STRING_T value = resource.get<STRING_T>(2); @endcode
	 * @note If resource is SINGLE then resInstId should be set to SINGLE_INSTANCE_ID.
	 * 		 If the data type is not valid or the instance does not exist,
	 *  	 the method will return an empty stub value. So before using this
	 * 		 method, you should check if the data type is valid and the
	 * 		 instance exists.
	 * @return The data value
	 */
	template<typename T>
	const T& get(ID_T resInstId = SINGLE_INSTANCE_ID);

	/**
	 * @brief Add new instance with data value by copy for the MULTIPLE resource
	 * @param value The data value to add
	 * @note If the resource is SINGLE, the method will return false.
	 * 		 If the verifier is set, the value will be checked.
	 * 		 If the data type is not valid, the method will return false.
	 * 		 Instance ID will be generated automatically.The ID is determined 
	 * 		 according to the following algorithm: if the ID is equal to the
	 * 		 number of free instances, then we return it, otherwise, starting
	 * 		 with ID 0, we search for the first free index, if no free indexes
	 * 		 are found, then method returns false.
	 * @return True if the instance is added, false otherwise
	 */
	template<typename T>
	bool add(const T &value);

	/**
	 * @brief Add new instance with data value by move for the MULTIPLE resource
	 * @param value The data value to add
	 * @note If the resource is SINGLE, the method will return false.
	 * 		 If the verifier is set, the value will be checked.
	 * 		 If the data type is not valid, the method will return false.
	 * 		 Instance ID will be generated automatically.The ID is determined 
	 * 		 according to the following algorithm: if the ID is equal to the
	 * 		 number of free instances, then we return it, otherwise, starting
	 * 		 with ID 0, we search for the first free index, if no free indexes
	 * 		 are found, then method returns false.
	 * @return True if the instance is added, false otherwise
	 */
	template<typename T>
	bool add(T &&value);

	/**
 	 * @brief Remove resource instance if resource is MULTIPLE and instance exists,
	 * 		  if the resource is SINGLE remove is not possible.
	 * @param resInstId The instance ID to remove
	 * @return True if the instance is removed, false otherwise.
	 */
	bool remove(ID_T resInstId);

	/**
 	 * @brief Remove all instances.
	 * @note If the resource is SINGLE, the method will return false.
	 * @return True if all instances are removed, false otherwise.
	 */
	bool clear();

private:
	template<typename T>
	bool isDataTypeValid() const;
	bool isDataVerifierValid(const DATA_VERIFIER_T &verifier) const;
	bool isInstanceIdPossible(ID_T resInstId) const;
	bool isTypeIdCompatible(TYPE_ID type) const;
	std::vector<ResInst>::iterator getInstIter(ID_T resInstId) const;

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
	if(isSingle()) resInstId = SINGLE_INSTANCE_ID;
	if (!isInstanceIdPossible(resInstId) || !isDataValueValid(value)) {
		WPP_LOGW(TAG_WPP_RES, "Resource[%d], invalid data value or instance id is not possible", _id);
		return false;
	}

	if (isExist(resInstId)) {
		auto instIter = getInstIter(resInstId);
		instIter->data = value;
	} else {
		_instances.push_back({resInstId, value});
	}

	return true;
}

template<typename T>
bool Resource::set(T &&value, ID_T resInstId) {
	if(isSingle()) resInstId = SINGLE_INSTANCE_ID;
	if (!isInstanceIdPossible(resInstId) || !isDataValueValid(value)) {
		WPP_LOGW(TAG_WPP_RES, "Resource[%d], invalid data value or instance id is not possible", _id);
		return false;
	}

	if (isExist(resInstId)) {
		auto instIter = getInstIter(resInstId);
		instIter->data = std::move(value);
	} else {
		ResInst newInst = {resInstId, std::move(value)};
		_instances.push_back(std::move(newInst));
	}

	return true;
}

template<typename T>
const T& Resource::get(ID_T resInstId) {
	if(isSingle()) resInstId = SINGLE_INSTANCE_ID;
	if (!isDataTypeValid<T>() || !isExist(resInstId)) {
		WPP_LOGE(TAG_WPP_RES, "Resource[%d], invalid data type or instance does not exist", _id);
		// TODO: It is workaround for the case when resource is not found
		// This behavior is better than returning NULL, but it is not the best solution
		// Return empty value if the data type is not valid or the instance does not exist
		static T empty;
		return empty;
	}

	auto instIter = getInstIter(resInstId);
	return std::get<T>(instIter->data);
}

template<typename T>
bool Resource::add(const T &value) {
	if (isSingle()) {
		WPP_LOGW(TAG_WPP_RES, "Resource[%d] is SINGLE", _id);
		return false;
	}
	if (!isDataValueValid(value)) {
		WPP_LOGW(TAG_WPP_RES, "Resource[%d] invalid data value", _id);
		return false;
	}

	ID_T resInstId = newInstId();
	if (resInstId == ID_T_MAX_VAL) {
		WPP_LOGE(TAG_WPP_RES, "Resource[%d], no available instance ID", _id);
		return false;
	}
	_instances.push_back({resInstId, value});
	
	return true;
}

template<typename T>
bool Resource::add(T &&value) {
	if (isSingle()) {
		WPP_LOGW(TAG_WPP_RES, "Resource[%d] is SINGLE", _id);
		return false;
	}
	if (!isDataValueValid(value)) {
		WPP_LOGW(TAG_WPP_RES, "Resource[%d] invalid data value", _id);
		return false;
	}

	ID_T resInstId = newInstId();
	if (resInstId == ID_T_MAX_VAL) {
		WPP_LOGE(TAG_WPP_RES, "Resource[%d], no available instance ID", _id);
		return false;
	}
	ResInst newInst = {resInstId, std::move(value)};
	_instances.push_back(std::move(newInst));
	
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
