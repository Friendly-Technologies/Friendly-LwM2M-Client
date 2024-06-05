/*
 * ObjectImpl.h
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_OBJECT_IMPL_H_
#define WPP_OBJECT_IMPL_H_

#include "Object.h"

namespace wpp {

/**
 * @class ObjectImpl
 * @brief The ObjectImpl class is a template class that represents an implementation of an object in the Wakaama library.
 * @tparam T The type of the object.
 *
 * The ObjectImpl class provides functionality for creating and managing instances of the object.
 * It inherits from the Object class and overrides its methods to provide the implementation specific to the object type.
 * The ObjectImpl class is intended to be used as a base class for implementing custom objects in the Wakaama library.
 */
template<typename T>
class ObjectImpl : public Object {
public:
	/**
	 * Default constructor.
	 */
	ObjectImpl(lwm2m_context_t &context): Object(context) {}
	/**
	 * @brief Constructs an ObjectImpl object.
	 * @param context The lwm2m_context_t object.
	 * @param info The ObjectInfo object.
	 */
	ObjectImpl(lwm2m_context_t &context, const ObjectInfo &info): Object(context, info) {}
	~ObjectImpl() {};

	/* ------------- ObjectImpl instance management ------------- */

	/**
	 * @brief Creates a new instance of the object.
	 * @param instanceID The ID of the instance to create. Defaults to ID_T_MAX_VAL.
	 * @return A pointer to the created instance of general type, or nullptr if the instance could not be created.
	 */
	Instance* createInstance(ID_T instanceID = ID_T_MAX_VAL) override;
};

/* ---------- Implementation of methods ----------*/
template<typename T>
Instance* ObjectImpl<T>::createInstance(ID_T instanceId) {
	// If object is single and instance has already exist, then we can not create new one and return NULL
	if (getObjectInfo().isSingle == IS_SINGLE::SINGLE && instanceCnt() != 0) {
		WPP_LOGW(TAG_WPP_OBJ, "Not possible to create instance %d:%d, object is single", getObjectID(), instanceId);
		return NULL;
	}
	// If instanceId == ID_T_MAX_VAL, it is mean that user do not want to set its own identifier, so we will choose ours
	if (instanceId == ID_T_MAX_VAL) instanceId = getFirstAvailableInstanceID();
	// If ID has been already occupied, then we can not create new instance with such ID and returns NULL
	if (instanceId == ID_T_MAX_VAL || isExist(instanceId)) {
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
	lwm2m_update_registration(&getContext(), 0, false, true);

	return inst;
}

} // namespace wpp
#endif /* WPP_OBJECT_IMPL_H_ */
