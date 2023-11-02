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

template<typename T>
class ObjectImpl : public Object {
public:
    ObjectImpl(lwm2m_context_t &context, const ObjectInfo &info): Object(context, info) {}
	~ObjectImpl() {}

/* ------------- ObjectImpl instance management ------------- */
	Instance* createInstance(ID_T instanceID = ID_T_MAX_VAL) override;
    T* createInstanceImpl(ID_T instanceID = ID_T_MAX_VAL);
    T* instanceImpl(ID_T instanceID = 0);
};


/* ---------- Implementation of methods ----------*/
template<typename T>
Instance* ObjectImpl<T>::createInstance(ID_T instanceId) {
	// If object is single and instance has already exist, then we can not create new one and return NULL
	if (getObjectInfo().isSingle == IS_SINGLE::SINGLE && instanceCnt() != 0) {
		WPP_LOGW_ARG(TAG_WPP_OBJ, "Not possible to create instance %d:%d, object is single", getObjectID(), instanceId);
		return NULL;
	}
	// If instanceId == ID_T_MAX_VAL, it is mean that user do not want to set its own identifier, so we will choose ours
	if (instanceId == ID_T_MAX_VAL) instanceId = getFirstAvailableInstanceID();
	// If ID has been already occupied, then we can not create new instance with such ID and returns NULL
	if (instanceId == ID_T_MAX_VAL || isInstanceExist(instanceId)) {
		WPP_LOGW_ARG(TAG_WPP_OBJ, "Not possible to create instance %d:%d, ID has been already occupied", getObjectID(), instanceId);
		return NULL;
	}

	WPP_LOGD_ARG(TAG_WPP_OBJ, "Creating instance %d:%d", getObjectID(), instanceId);
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
	return inst;
}

template<typename T>
T* ObjectImpl<T>::createInstanceImpl(ID_T instanceID) {
    Instance *inst = createInstance(instanceID);
    return inst? static_cast<T*>(inst) : NULL;
}

template<typename T>
T* ObjectImpl<T>::instanceImpl(ID_T instanceID) {
    Instance *inst = instance(instanceID);
    return inst? static_cast<T*>(inst) : NULL;
}

} // namespace wpp
#endif /* WPP_OBJECT_IMPL_H_ */
