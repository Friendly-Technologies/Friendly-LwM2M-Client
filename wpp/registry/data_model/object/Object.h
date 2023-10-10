/*
 * Object.h
 *
 *  Created on: 10 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_OBJECT_H_
#define WPP_OBJECT_H_

#include <unordered_map>
#include <variant>

#include "Lwm2mObject.h"
#include "ObjObserver.h"
#include "ObjSubject.h"
#include "Instance.h"
#include "types.h"
#include "WppLogs.h"

namespace wpp {

class WppClient;

/*
 * Object<T> is class that implements manipulation with Instance interface class and his inheritors.
 * The main target of this class is to encapsulate operations like instance create and delete, binding
 * instance callbacks to core interface, for avoid multiple definition of this mechanism in instance
 * implementation classes.
 */
template<typename T>
class Object : public Lwm2mObject, public ObjSubject<T> {
private:
	Object(lwm2m_context_t &context, const ObjectInfo &info);

	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

public:
	~Object();

/* ------------- Object management ------------- */
	static bool create(lwm2m_context_t &context, const ObjectInfo &info);
	static bool isCreated();
	static Object<T>* object();

/* ------------- Object instance management ------------- */
	T* createInstance(ID_T instanceID = ID_T_MAX_VAL);
	bool removeInstance(ID_T instanceID);
	void clear() override;
	void restore() override;

	T* instance(ID_T instanceID = 0);
	const std::unordered_map<ID_T, T*> & getInstances();
	size_t instanceCnt() override;
	bool isInstanceExist(ID_T instanceID) override;

private:
	ID_T getFirstAvailableInstanceID();

/* ------------- Lwm2m core callback ------------- */
	static uint8_t read_clb(lwm2m_context_t * contextP, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);
	static uint8_t write_clb(lwm2m_context_t * contextP, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP, lwm2m_write_type_t writeType);
	static uint8_t execute_clb(lwm2m_context_t * contextP, ID_T instanceId, ID_T resId, uint8_t * buffer, int length, lwm2m_object_t * objectP);
	static uint8_t discover_clb(lwm2m_context_t * contextP, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);
	static uint8_t create_clb(lwm2m_context_t * contextP, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
	static uint8_t delete_clb(lwm2m_context_t * contextP, ID_T instanceId, lwm2m_object_t * objectP);
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	// TODO Implement this functionality
	static uint8_t block1Ccreate_clb(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_media_type_t format, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
	static uint8_t block1Write_clb(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_media_type_t format, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
	static uint8_t block1Execute_clb(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
#endif
private:
	static Object<T> *_object;
	
	lwm2m_context_t &_context;
	std::unordered_map<ID_T, Instance*> _instances; // TODO: maybe here is better to use share_ptr instead simple Instance*
};


/* ---------- Implementation of methods ----------*/
template<typename T>
Object<T> *Object<T>::_object = NULL;

template<typename T>
Object<T>::Object(lwm2m_context_t &context, const ObjectInfo &info): Lwm2mObject(info), _context(context) {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "Creating object with ID -> %d", (ID_T)info.objID);

	// Initialising core object representation
	_lwm2m_object.objID 	   = (ID_T)_objInfo.objID;
	_lwm2m_object.instanceList = NULL;
	_lwm2m_object.versionMajor = _objInfo.objVersion.major;
	_lwm2m_object.versionMinor = _objInfo.objVersion.minor;
	_lwm2m_object.userData = NULL;

	if (_objInfo.resOperation.isRead()) _lwm2m_object.readFunc = read_clb;
	else  _lwm2m_object.readFunc = NULL;
	if (_objInfo.resOperation.isDiscover()) _lwm2m_object.discoverFunc = discover_clb;
	else  _lwm2m_object.discoverFunc = NULL;
	if (_objInfo.resOperation.isWrite()) _lwm2m_object.writeFunc = write_clb;
	else  _lwm2m_object.writeFunc = NULL;
	if (_objInfo.resOperation.isExecute()) _lwm2m_object.executeFunc = execute_clb;
	else  _lwm2m_object.executeFunc = NULL;
	if (_objInfo.instOperation.isCreate()) _lwm2m_object.createFunc = create_clb;
	else  _lwm2m_object.createFunc = NULL;
	if (_objInfo.instOperation.isDelete()) _lwm2m_object.deleteFunc = delete_clb;
	else  _lwm2m_object.deleteFunc = NULL;
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	if (_objInfo.instOperation.isBlock1Create()) _lwm2m_object.rawBlock1CreateFunc = block1Ccreate_clb;
	else  _lwm2m_object.rawBlock1CreateFunc = NULL;
	if (_objInfo.resOperation.isBlock1Write()) _lwm2m_object.rawBlock1WriteFunc = block1Write_clb;
	else  _lwm2m_object.rawBlock1WriteFunc = NULL;
	if (_objInfo.resOperation.isBlock1Execute()) _lwm2m_object.rawBlock1ExecuteFunc = block1Execute_clb;
	else  _lwm2m_object.rawBlock1ExecuteFunc = NULL;
#endif
}

template<typename T>
Object<T>::~Object() {
	clear();
}

/* ------------- Object management ------------- */
template<typename T>
bool Object<T>::create(lwm2m_context_t &context, const ObjectInfo &info) {
	if (isCreated()) return true;
	static Object<T> obj(context, info);
	_object = &obj;
	return true;
}

template<typename T>
bool Object<T>::isCreated() {
	return _object != NULL;
}

template<typename T>
Object<T>* Object<T>::object() {
	return _object;
}

/* ------------- Object instance management ------------- */
template<typename T>
T* Object<T>::createInstance(ID_T instanceId) {
	// If object is single and instance has already exist, then we can not create new one and return NULL
	if (_objInfo.isSingle == IS_SINGLE::SINGLE && _instances.size() != 0) {
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

	// TODO: Use lwm2m_object_t.instanceList and its elements lwm2m_list_t * for save instances instead std:: conatainer
	// TODO: Add check to each new or malloc operation
	// Creating new instance
	_instances[instanceId] = new T(_context, {(ID_T)_objInfo.objID, instanceId});
	return static_cast<T*>(_instances[instanceId]);
}

template<typename T>
bool Object<T>::removeInstance(ID_T instanceId) {
	// If user want to delete instance with ID that does not exist, then we can not do it
	if (!isInstanceExist(instanceId)) return false;

	WPP_LOGD_ARG(TAG_WPP_OBJ, "Removing instance %d:%d", getObjectID(), instanceId);
	// Deleting registered instance from core object
	 lwm2m_list_t *element = NULL;
	 _lwm2m_object.instanceList = LWM2M_LIST_RM(_lwm2m_object.instanceList, instanceId, (lwm2m_list_t **)&element);
	 if (NULL != element) delete element;

	delete _instances[instanceId];
	_instances.erase(instanceId);
	return true;
}

template<typename T>
void Object<T>::clear() {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "Clearing object with ID -> %d", getObjectID());
	// Deleting registered instances from core object
	while (_lwm2m_object.instanceList != NULL) {
		lwm2m_list_t * instance = (lwm2m_list_t *)_lwm2m_object.instanceList;
		ID_T id = instance->id;

		_lwm2m_object.instanceList = _lwm2m_object.instanceList->next;
		delete instance;

		delete _instances[id];
		_instances.erase(id);
	}
}

template<typename T>
void Object<T>::restore() {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "Restoring object with ID -> %d", getObjectID());
    this->observerDoAction(*this, ObjSubject<T>::Action::RESTORE);
}

template<typename T>
T* Object<T>::instance(ID_T instanceID) {
	// If user want to access instance with ID that does not exist, then we can not do it
	if (!isInstanceExist(instanceID)) return NULL;
	return static_cast<T*>(_instances[instanceID]);
}

template<typename T>
const std::unordered_map<ID_T, T*> & Object<T>::getInstances() {
	return _instances;
}

template<typename T>
size_t Object<T>::instanceCnt() {
	return _instances.size();
}

template<typename T>
bool  Object<T>::isInstanceExist(ID_T instanceID) {
	return _instances.find(instanceID) != _instances.end();
}

template<typename T>
ID_T Object<T>::getFirstAvailableInstanceID() {
	// Usually, each subsequent free index will be equal to the number of created objects
	ID_T id = _instances.size();
	if (id == ID_T_MAX_VAL) return id;
	// But it won't always be like that
	while (isInstanceExist(id) && id != ID_T_MAX_VAL) id++;
	// It is also possible that all indexes after the current size are occupied
	if (id == ID_T_MAX_VAL) {
		id = 0;
		// In this case, we need to check the indexes that are before the current size
		while (isInstanceExist(id) && id < _instances.size()) id++;
	}

	return id;
}

/* ------------- Lwm2m core callback ------------- */
template<typename T>
uint8_t Object<T>::read_clb(lwm2m_context_t * contextP, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP) {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "wakaama read %d:%d", object()->getObjectID(), instanceId);
	if (!object()->isInstanceExist(instanceId)) return COAP_404_NOT_FOUND;
	return object()->_instances[instanceId]->read(numDataP, dataArrayP);
}

template<typename T>
uint8_t Object<T>::write_clb(lwm2m_context_t * contextP, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP, lwm2m_write_type_t writeType) {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "wakaama write %d:%d", object()->getObjectID(), instanceId);
	if (!object()->isInstanceExist(instanceId)) return COAP_404_NOT_FOUND;
	return object()->_instances[instanceId]->write(numData, dataArray, writeType);
}

template<typename T>
uint8_t Object<T>::execute_clb(lwm2m_context_t * contextP, ID_T instanceId, ID_T resId, uint8_t * buffer, int length, lwm2m_object_t * objectP) {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "wakaama execute %d:%d", object()->getObjectID(), instanceId);
	if (!object()->isInstanceExist(instanceId)) return COAP_404_NOT_FOUND;
	return object()->_instances[instanceId]->execute(resId, buffer, length);
}

template<typename T>
uint8_t Object<T>::discover_clb(lwm2m_context_t * contextP, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP) {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "wakaama discover %d:%d", object()->getObjectID(), instanceId);
	if (!object()->isInstanceExist(instanceId)) return COAP_404_NOT_FOUND;
	return object()->_instances[instanceId]->discover(numDataP, dataArrayP);
}

template<typename T>
uint8_t Object<T>::create_clb(lwm2m_context_t * contextP, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "wakaama create %d:%d", object()->getObjectID(), instanceId);
	if (!object()->createInstance(instanceId)) return COAP_500_INTERNAL_SERVER_ERROR;
	// Notify user about creating instance
	object()->observerNotify(*object(), instanceId, InstOp::CREATE);

	uint8_t result = write_clb(contextP, instanceId, numData, dataArray, objectP, LWM2M_WRITE_REPLACE_RESOURCES);
	if (result != COAP_204_CHANGED) {
		delete_clb(contextP, instanceId, objectP);
		return result;
	}

	return COAP_201_CREATED;
}

template<typename T>
uint8_t Object<T>::delete_clb(lwm2m_context_t * contextP, ID_T instanceId, lwm2m_object_t * objectP) {
	WPP_LOGD_ARG(TAG_WPP_OBJ, "wakaama delete %d:%d", object()->getObjectID(), instanceId);
	if (!object()->isInstanceExist(instanceId)) return COAP_404_NOT_FOUND;
	// Notify user about deleting instance
	object()->observerNotify(*object(), instanceId, InstOp::DELETE);

	return object()->removeInstance(instanceId)? COAP_202_DELETED : COAP_404_NOT_FOUND;
}

} // namespace wpp
#endif /* WPP_OBJECT_H_ */
