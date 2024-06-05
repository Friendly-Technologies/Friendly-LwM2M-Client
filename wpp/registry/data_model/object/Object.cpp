#include "Object.h"
#include "WppClient.h"

namespace wpp {

Object::Object(lwm2m_context_t &context) : _context(context) {}

Object::Object(lwm2m_context_t &context, const ObjectInfo &info):  _context(context), _objInfo(info) {
	WPP_LOGD(TAG_WPP_OBJ, "Creating object with ID -> %d", (ID_T)info.objID);

	// Initialising core object representation
	_lwm2m_object.objID 	   = (ID_T)_objInfo.objID;
	_lwm2m_object.instanceList = NULL;
	_lwm2m_object.versionMajor = _objInfo.objVersion.major;
	_lwm2m_object.versionMinor = _objInfo.objVersion.minor;
	_lwm2m_object.userData     = this;

	if (_objInfo.operations.isRead()) _lwm2m_object.readFunc = serverRead_clb;
	else  _lwm2m_object.readFunc = NULL;
	if (_objInfo.operations.isDiscover()) _lwm2m_object.discoverFunc = serverDiscover_clb;
	else  _lwm2m_object.discoverFunc = NULL;
	if (_objInfo.operations.isWrite()) _lwm2m_object.writeFunc = serverWrite_clb;
	else  _lwm2m_object.writeFunc = NULL;
	if (_objInfo.operations.isExecute()) _lwm2m_object.executeFunc = serverExecute_clb;
	else  _lwm2m_object.executeFunc = NULL;
	if (_objInfo.operations.isCreate()) _lwm2m_object.createFunc = serverCreate_clb;
	else  _lwm2m_object.createFunc = NULL;
	if (_objInfo.operations.isDelete()) _lwm2m_object.deleteFunc = serverDelete_clb;
	else  _lwm2m_object.deleteFunc = NULL;
}

Object::~Object() {
	clear();
    _lwm2m_object.userData = NULL;
}

OBJ_ID Object::getObjectID() const { 
    return _objInfo.objID; 
}

lwm2m_object_t& Object::getLwm2mObject() { 
    return _lwm2m_object; 
}

const ObjectInfo& Object::getObjectInfo() const { 
    return _objInfo; 
}

lwm2m_context_t& Object::getContext() { 
	return _context; 
}

WppClient& Object::getClient() {
	return *static_cast<wpp::WppClient *>(getContext().userData);
}

WppRegistry& Object::getRegistry() {
	return static_cast<wpp::WppClient *>(getContext().userData)->registry();
}

void Object::clear() {
	WPP_LOGD(TAG_WPP_OBJ, "Clearing object with ID -> %d", getObjectID());
	// Deleting registered instances from core object
	while (_lwm2m_object.instanceList != NULL) {
		lwm2m_list_t * instance = (lwm2m_list_t *)_lwm2m_object.instanceList;
		ID_T id = instance->id;

		_lwm2m_object.instanceList = _lwm2m_object.instanceList->next;
		delete instance;

		// Update server registration
		lwm2m_update_registration(&getContext(), 0, false, true);

		auto inst = getInstIter(id);
		if (inst == _instances.end()) continue;

		delete *inst;
		_instances.erase(inst);
	}
}

bool Object::remove(ID_T instanceId) {
	// If user want to delete instance with ID that does not exist, then we can not do it
	auto inst = getInstIter(instanceId);
	if (inst == _instances.end()) return false;

	WPP_LOGD(TAG_WPP_OBJ, "Removing instance %d:%d", getObjectID(), instanceId);
	// Deleting registered instance from core object
	 lwm2m_list_t *element = NULL;
	 _lwm2m_object.instanceList = LWM2M_LIST_RM(_lwm2m_object.instanceList, instanceId, (lwm2m_list_t **)&element);
	 if (NULL != element) delete element;

	delete *inst;
	_instances.erase(inst);

	// Update server registration
	lwm2m_update_registration(&getContext(), 0, false, true);
	
	return true;
}

Instance* Object::instance(ID_T instanceID) {
	// If user want to access instance with ID that does not exist, then we can not do that
	auto inst = (instanceID != ID_T_MAX_VAL)? getInstIter(instanceID) : _instances.begin();
	return inst != _instances.end()? *inst : NULL;
}

const std::vector<Instance*> & Object::instances() {
	return _instances;
}

size_t Object::instanceCnt() {
	return _instances.size();
}

bool Object::isExist(ID_T instanceID) {
	return getInstIter(instanceID) != _instances.end();
}

std::vector<Instance*>::iterator Object::getInstIter(ID_T instanceID) {
	auto finder = [instanceID](const Instance *inst) -> bool { return inst->getInstanceID() == instanceID; };
	return std::find_if(_instances.begin(), _instances.end(), finder);
}

ID_T Object::getFirstAvailableInstanceID() {
	// Usually, each subsequent free index will be equal to the number of created objects
	if (!isExist(_instances.size())) return _instances.size();
	// If there are no free indexes, we will search for the first free index
	ID_T id = 0;
	while (isExist(id) && id != ID_T_MAX_VAL) id++;
	return id;
}

/* ------------- Lwm2m core callback ------------- */
uint8_t Object::serverRead_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP) {
    Object *obj = static_cast<Object *>(objectP->userData);
	WPP_LOGD(TAG_WPP_OBJ, "wakaama read %d:%d", obj->getObjectID(), instanceId);
	if (!obj->isExist(instanceId)) return COAP_404_NOT_FOUND;
	return obj->instance(instanceId)->readAsServer(server, numDataP, dataArrayP);
}

uint8_t Object::serverWrite_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP, lwm2m_write_type_t writeType) {
	Object *obj = static_cast<Object *>(objectP->userData);
    WPP_LOGD(TAG_WPP_OBJ, "wakaama write %d:%d", obj->getObjectID(), instanceId);
	if (!obj->isExist(instanceId)) return COAP_404_NOT_FOUND;
	return obj->instance(instanceId)->writeAsServer(server, numData, dataArray, writeType);
}

uint8_t Object::serverExecute_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, ID_T resId, uint8_t * buffer, int length, lwm2m_object_t * objectP) {
	Object *obj = static_cast<Object *>(objectP->userData);
    WPP_LOGD(TAG_WPP_OBJ, "wakaama execute %d:%d", obj->getObjectID(), instanceId);
	if (!obj->isExist(instanceId)) return COAP_404_NOT_FOUND;
	return obj->instance(instanceId)->executeAsServer(server, resId, buffer, length);
}

uint8_t Object::serverDiscover_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP) {
	Object *obj = static_cast<Object *>(objectP->userData);
    WPP_LOGD(TAG_WPP_OBJ, "wakaama discover %d:%d", obj->getObjectID(), instanceId);
	if (!obj->isExist(instanceId)) return COAP_404_NOT_FOUND;
	return obj->instance(instanceId)->discoverAsServer(server, numDataP, dataArrayP);
}

uint8_t Object::serverCreate_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {
	Object *obj = static_cast<Object *>(objectP->userData);
    WPP_LOGD(TAG_WPP_OBJ, "wakaama create %d:%d", obj->getObjectID(), instanceId);
	if (!obj->createInstance(instanceId)) return COAP_500_INTERNAL_SERVER_ERROR;
	// Notify user about creating instance
	obj->operationNotify(*obj, instanceId, ItemOp::CREATE);

	uint8_t result = serverWrite_clb(contextP, server, instanceId, numData, dataArray, objectP, LWM2M_WRITE_REPLACE_RESOURCES);
	if (result != COAP_204_CHANGED) {
		serverDelete_clb(contextP, server, instanceId, objectP);
		return result;
	}

	return COAP_201_CREATED;
}

uint8_t Object::serverDelete_clb(lwm2m_context_t * contextP, lwm2m_server_t *server, ID_T instanceId, lwm2m_object_t * objectP) {
	Object *obj = static_cast<Object *>(objectP->userData);
    WPP_LOGD(TAG_WPP_OBJ, "wakaama delete %d:%d", obj->getObjectID(), instanceId);
	if (!obj->isExist(instanceId)) return COAP_404_NOT_FOUND;
	// Notify user about deleting instance
	obj->operationNotify(*obj, instanceId, ItemOp::DELETE);

	return obj->remove(instanceId)? COAP_202_DELETED : COAP_404_NOT_FOUND;
}

} // namespace wpp