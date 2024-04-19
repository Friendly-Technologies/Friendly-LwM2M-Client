/*
 * ObjectSpec.h
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_OBJECT_SPEC_H_
#define WPP_OBJECT_SPEC_H_

#include "Object.h"

namespace wpp {

/**
 * @class ObjectSpec
 * @brief The ObjectSpec class is a template class that represents an implementation of an object in the Wakaama library.
 * @tparam T The type of the object.
 *
 * The ObjectSpec class provides functionality for creating and managing instances of the object.
 * It inherits from the Object class and overrides its methods to provide the implementation specific to the object type.
 * The ObjectSpec class is intended to be used as a base class for implementing custom objects in the Wakaama library.
 */
template<typename T>
class ObjectSpec : public Object {
public:
	/**
	 * @brief Constructs an ObjectSpec object.
	 * @param context The lwm2m_context_t object.
	 * @param info The ObjectInfo object.
	 */
	ObjectSpec(lwm2m_context_t &context, const ObjectInfo &info): Object(context, info) {}
	~ObjectSpec() {};

	/* ------------- ObjectSpec instance management ------------- */

	/**
	 * @brief Creates a new instance of the object.
	 * @param instanceID The ID of the instance to create. Defaults to ID_T_MAX_VAL.
	 * @return A pointer to the created instance of general type, or nullptr if the instance could not be created.
	 */
	Instance* createInstance(ID_T instanceID = ID_T_MAX_VAL) override;

	/**
	 * @brief Creates a new instance of the object.
	 * @param instanceID The ID of the instance to create. Defaults to ID_T_MAX_VAL.
	 * @return A pointer to the created instance of specialized type, or nullptr if the instance could not be created.
	 */
	T* createInstanceSpec(ID_T instanceID = ID_T_MAX_VAL);

	/**
	 * @brief Returns the instance of the object with the specified ID.
	 * @param instanceID The ID of the instance to retrieve. Defaults to 0.
	 * @return A pointer to the instance specialized type, or nullptr if the instance does not exist.
	 */
	T* instanceSpec(ID_T instanceID = ID_T_MAX_VAL);

	/**
	 * @brief Gets all instances of the object.
	 * 
	 * @return A vector of pointers to the Instance objects.
	 */
	const std::vector<T*>& getSpecInstances();
};

/* ---------- Implementation of methods ----------*/
template<typename T>
Instance* ObjectSpec<T>::createInstance(ID_T instanceId) {
	// If object is single and instance has already exist, then we can not create new one and return NULL
	if (getObjectInfo().isSingle == IS_SINGLE::SINGLE && instanceCnt() != 0) {
		WPP_LOGW(TAG_WPP_OBJ, "Not possible to create instance %d:%d, object is single", getObjectID(), instanceId);
		return NULL;
	}
	// If instanceId == ID_T_MAX_VAL, it is mean that user do not want to set its own identifier, so we will choose ours
	if (instanceId == ID_T_MAX_VAL) instanceId = getFirstAvailableInstanceID();
	// If ID has been already occupied, then we can not create new instance with such ID and returns NULL
	if (instanceId == ID_T_MAX_VAL || isInstanceExist(instanceId)) {
		WPP_LOGW(TAG_WPP_OBJ, "Not possible to create instance %d:%d, ID has been already occupied", getObjectID(), instanceId);
		return NULL;
	}

	WPP_LOGD(TAG_WPP_OBJ, "Creating instance %d:%d", getObjectID(), instanceId);
	// Creation and registration new instance in core object
	 lwm2m_list_t *element = new lwm2m_list_t;
	 if (NULL == element) return NULL;
	 element->next = NULL;
	 element->id = instanceId;
	 _lwm2m_object.instanceList = LWM2M_LIST_ADD(_lwm2m_object.instanceList, element);

	// TODO: Add check to each new or malloc operation
	// Creating new instance
	T *inst = new T(_context, {(ID_T)_objInfo.objID, instanceId});
	_instances.push_back(inst);

	// Update server registration
	if (getContext().state > STATE_BOOTSTRAPPING) lwm2m_update_registration(&getContext(), 0, false, true);

	return inst;
}

template<typename T>
T* ObjectSpec<T>::createInstanceSpec(ID_T instanceID) {
    Instance *inst = createInstance(instanceID);
    return inst? static_cast<T*>(inst) : NULL;
}

template<typename T>
T* ObjectSpec<T>::instanceSpec(ID_T instanceID) {
    Instance *inst = instance(instanceID);
    return inst? static_cast<T*>(inst) : NULL;
}

template<typename T>
const std::vector<T*>& ObjectSpec<T>::getSpecInstances() {
	std::vector<T*> specInstances;
	for (auto inst : _instances) {
		specInstances.push_back(static_cast<T*>(inst));
	}
	return specInstances;
}

} // namespace wpp
#endif /* WPP_OBJECT_SPEC_H_ */
