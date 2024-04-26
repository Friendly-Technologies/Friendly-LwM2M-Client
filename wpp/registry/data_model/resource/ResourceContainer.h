#ifndef WPP_RESOURCE_CONTAINER_H
#define WPP_RESOURCE_CONTAINER_H

#include <vector>

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"

namespace wpp {

class ResourceContainer {
public:
    ResourceContainer(std::vector<Resource> resources);
    ResourceContainer(std::vector<Resource> &&resources);
    virtual ~ResourceContainer() {}

	/**
	 * @brief Check if the resource is SINGLE
	 * @param resId The resource ID
	 */
	bool isSingle(ID_T resId) const;

	/**
	 * @brief Check if the resource is MULTIPLE
	 * @param resId The resource ID
	 */
    bool isMultiple(ID_T resId) const;

	/**
	 * @brief Check if the instance ID is exist
	 * @param resId The resource ID
	 * @return True if the instance ID is exist, false otherwise
	 */
	bool isExist(ID_T resId) const;

	/**
	 * @brief Check if the instance ID is exist
	 * @param resId The resource ID
	 * @param resInstId The instance ID to check
	 * @note If the resource is SINGLE, the instance ID always SINGLE_INSTANCE_ID
	 * @return True if the instance ID is exist, false otherwise
	 */
	bool isExist(ID_T resId, ID_T resInstId) const;

	/**
	 * @brief Get the number of resource instances
	 * @param resId The resource ID
	 * @note If the resource is SINGLE, the number of instances is always 1
	 * @return The number of resource instances
	 */
	size_t instCount(ID_T resId) const;

	/**
 	 * @brief Returns vector with available ids of resource instances
	 * @param resId The resource ID
	 * @note If the resource is SINGLE, the vector will be contain only one
	 * 		 element with value SINGLE_INSTANCE_ID
	 */
	std::vector<ID_T> instIds(ID_T resId) const;

	/**
	 * @brief Find first available instance ID that is not used
	 * @param resId The resource ID
	 * @note If the resource is SINGLE, the method will return ID_T_MAX_VAL
	 * @return The first available instance ID else ID_T_MAX_VAL
	 */
	ID_T newInstId(ID_T resId) const;

	/**
	 * @brief Set data value by copy for the resource (instance)
	 * @param value The data value to set 
	 * @param resId The resource ID
	 * @param resInstId The instance ID to set the value, used only for MULTIPLE resources
	 * @note Before set new value its type will be checked. If resource is SINGLE then
	 * 		 resInstId not used.
	 * @return True if the value is set, false otherwise
	 */
	template<typename T>
    bool set(const T &value, ID_T resId, ID_T resInstId = SINGLE_INSTANCE_ID);

	/**
	 * @brief Set data value by move for the resource (instance)
	 * @param value The data value to set 
	 * @param resId The resource ID
	 * @param resInstId The instance ID to set the value, used only for multiple resources
	 * @note Before set new value its type will be checked. If resource is SINGLE then
	 * 		 resInstId not used.
	 * @return True if the value is set, false otherwise
	 */
	template<typename T>
	bool set(T &&value, ID_T resId, ID_T resInstId = SINGLE_INSTANCE_ID);

	/**
	 * @brief Get data value by reference for the resource (instance)
	 * @param resId The resource ID
	 * @param resInstId The instance ID to get the value
	 * @example For example, if the resource is of type INT_T and SINGLE, the value can be retrieved as follows:
	 * @code INT_T value = resources.get<INT_T>(1); @endcode
	 * @example For example, if the resource is of type STRING_T and MULTIPLE, the value can be retrieved as follows:
	 * @code STRING_T value = resources.get<STRING_T>(2, 1); @endcode
	 * @note If resource is SINGLE then resInstId not used.
	 * 		 If the data type is not valid or the instance does not exist,
	 *  	 the method will return an empty stub value. So before using this
	 * 		 method, you should check if the data type is valid and the
	 * 		 instance exists.
	 * @return The data value
	 */
	template<typename T>
	const T& get(ID_T resId, ID_T resInstId = SINGLE_INSTANCE_ID);

	/**
	 * @brief Add new instance with data value by copy for the MULTIPLE resource
	 * @param value The data value to add
	 * @param resId The resource ID
	 * @note If the resource is SINGLE, the method will return false.
	 * 		 If the data type is not valid, the method will return false.
	 * 		 Instance ID will be generated automatically.
	 * @return True if the instance is added, false otherwise
	 */
	template<typename T>
	bool add(const T &value, ID_T resId);

	/**
	 * @brief Add new instance with data value by move for the MULTIPLE resource
	 * @param value The data value to add
	 * @param resId The resource ID
	 * @note If the resource is SINGLE, the method will return false.
	 * 		 If the data type is not valid, the method will return false.
	 * 		 Instance ID will be generated automatically.
	 * @return True if the instance is added, false otherwise
	 */
	template<typename T>
	bool add(T &&value, ID_T resId);

	/**
 	 * @brief Remove resource instance if resource is MULTIPLE and instance exists,
	 * 		  if the resource is SINGLE remove is not possible.
	 * @param resId The resource ID
	 * @param resInstId The instance ID to remove
	 * @return True if the instance is removed, false otherwise.
	 */
	bool remove(ID_T resId, ID_T resInstId);

	/**
 	 * @brief Remove all instances.
	 * @note If the resource is SINGLE, the method will return false.
	 * @return True if all instances are removed, false otherwise.
	 */
	bool clear(ID_T resId);

protected:
	/**
 	 * @brief This method return resource ptr if it exists.
	 * 		  If resources does not exist then return NULL.
	 * @param resId - Resource ID.
	 * @return Resource pointer or NULL.
	 */
    Resource * resource(ID_T resId);

	/**
	 * @brief This method return list with all resources that has been defined.
	 */
    std::vector<Resource> & resources();

	/**
 	 * @brief This method must be implemented by the derived class, and handle
     * information about resource operation (WRITE, DELETE).
	 * Called by ResourceContainer after resource operation performed.
	 */
    virtual void resourceOperationNotifier(ItemOp::TYPE type, ID_T resId, ID_T resInstId) = 0;

private:
    std::vector<Resource> _resources;
};

/* ---------- Implementation of template methods ----------*/
template<typename T>
bool ResourceContainer::set(const T &value, ID_T resId, ID_T resInstId) {
	auto res = resource(resId);
	if (res == NULL) {
		WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
		return false;
	}
	if (!res->set(value, resInstId)) return false;
	operationNotifier(ItemOp::WRITE, resId, resInstId);
	return true;
}

template<typename T>
bool ResourceContainer::set(T &&value, ID_T resId, ID_T resInstId) {
	auto res = resource(resId);
	if (res == NULL) {
		WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
		return false;
	}
	if (!res->set(std::move(value), resInstId)) return false;
	operationNotifier(ItemOp::WRITE, resId, resInstId);
	return true;
}

template<typename T>
const T& ResourceContainer::get(ID_T resId, ID_T resInstId) {
	auto res = resource(resId);
	if (res == NULL) {
		WPP_LOGE(TAG_WPP_RES, "Resource does not exist");
		// TODO: It is workaround for the case when resource is not found
		// This behavior is better than returning NULL, but it is not the best solution
		// Return empty value if the data type is not valid or the instance does not exist
		static T empty;
		return empty;
	}
	return res->get<T>(resInstId);
}

template<typename T>
bool ResourceContainer::add(const T &value, ID_T resId) {
	auto res = resource(resId);
	if (res == NULL) {
		WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
		return false;
	}
	ID_T newId = res->newInstId();
	if (!res->set(value, newId)) return false;
	operationNotifier(ItemOp::WRITE, resId, newId);
	return true;
}

template<typename T>
bool ResourceContainer::add(T &&value, ID_T resId) {
	auto res = resource(resId);
	if (res == NULL) {
		WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
		return false;
	}
	ID_T newId = res->newInstId();
	if (!res->set(std::move(value), newId)) return false;
	operationNotifier(ItemOp::WRITE, resId, newId);
	return true;
}

} /* namespace wpp */

#endif // WPP_RESOURCE_CONTAINER_H