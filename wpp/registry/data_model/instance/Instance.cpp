/*
 * Instnace.cpp
 *
 *  Created on: 17 Jul 2023
 *      Author: valentin
 */

#include "Instance.h"
#include "WppLogs.h"
#include "WppClient.h"

#define IS_PTR_VALID_AND_RES_EXISTS(resPtr) (resPtr && resPtr->instCount())
#define IS_RES_PTR_VALID(resPtr) (resPtr != NULL)

namespace wpp {

void Instance::notifyResChanged(ID_T resId, ID_T resInstId) {
	if (_context.state <= STATE_BOOTSTRAPPING) return;
	WPP_LOGD(TAG_WPP_INST, "Notify value changed: objID=%d, instID=%d, resID=%d, resInstID=%d", getObjectID(), getInstanceID(), resId, resInstId);	
	lwm2m_uri_t uri = {getObjectID(), getInstanceID(), resId, resInstId};
	lwm2m_resource_value_changed(&_context, &uri);
}

std::vector<Resource *> Instance::getInstantiatedResList() {
	std::vector<Resource *> list;
	for (auto &res : resources()) {
		if (res.instCount()) list.push_back(&res);
	}
	return list;
}

std::vector<Resource *> Instance::getInstantiatedResList(const ItemOp& filter) {
	std::vector<Resource *> list;
	for (auto &res : resources()) {
		if (res.instCount() && filter.isCompatible(res.getOperation())) list.push_back(&res);
	}
	return list;
}

std::vector<Resource *> Instance::getResList() {
	std::vector<Resource *> list;
	for (auto &res : resources()) list.push_back(&res);
	return list;
}

void Instance::resourceOperationNotifier(ItemOp::TYPE type, ID_T resId, ID_T resInstId) {
	userOperationNotifier(type, {resId, resInstId});
}

lwm2m_context_t& Instance::getContext() { 
	return _context; 
}

WppClient& Instance::getClient() {
	return *static_cast<wpp::WppClient *>(getContext().userData);
}

WppRegistry& Instance::getRegistry() {
	return static_cast<wpp::WppClient *>(getContext().userData)->registry();
}

Instance* Instance::getSecurityInst(lwm2m_server_t *server) {
	if (!server) return NULL;
	return getRegistry().object(OBJ_ID::LWM2M_SECURITY)->instance(server->secObjInstID);
}

bool Instance::resourceToLwm2mData(Resource &res, ID_T instanceId, lwm2m_data_t &data) {
	if (!res.isExist(instanceId)) {
		WPP_LOGE(TAG_WPP_INST, "Resource instance does not exist: %d:%d:%d:%d", _id.objId, _id.objInstId, res.getId(), instanceId);	
		return false;
	}

	switch(res.getTypeId()) {
	case TYPE_ID::BOOL: {
		BOOL_T value = res.get<BOOL_T>(instanceId);
		lwm2m_data_encode_bool(value, &data);
		break;
	}
	case TYPE_ID::TIME:  {
		TIME_T value = res.get<TIME_T>(instanceId);
		lwm2m_data_encode_time(value, &data);
		break;
	}
	case TYPE_ID::INT: {
		INT_T value = res.get<INT_T>(instanceId);
		lwm2m_data_encode_int(value, &data);
		break;
	}
	case TYPE_ID::UINT: {
		UINT_T value = res.get<UINT_T>(instanceId);
		lwm2m_data_encode_uint(value, &data);
		break;
	}
	case TYPE_ID::FLOAT: {
		FLOAT_T value = res.get<FLOAT_T>(instanceId);
		lwm2m_data_encode_float(value, &data);
		break;
	}
	case TYPE_ID::OBJ_LINK: {
		OBJ_LINK_T value = res.get<OBJ_LINK_T>(instanceId);
		lwm2m_data_encode_objlink(value.objId, value.objInstId, &data);
		break;
	}
	case TYPE_ID::OPAQUE: {
		OPAQUE_T value = res.get<OPAQUE_T>(instanceId);
		lwm2m_data_encode_opaque(value.data(), value.size(), &data);
		break;
	}
	case TYPE_ID::STRING: {
		STRING_T value = res.get<STRING_T>(instanceId);
		lwm2m_data_encode_string(value.c_str(), &data);
		break;
	}
	case TYPE_ID::CORE_LINK: {
		CORE_LINK_T value = res.get<CORE_LINK_T>(instanceId);
		lwm2m_data_encode_corelink(value.c_str(), &data);
		break;
	}
	default: return false;
	}

	return true;
}

bool Instance::lwm2mDataToResource(const lwm2m_data_t &data, Resource &res, ID_T instanceId) {
	switch (res.getTypeId()) {
	case TYPE_ID::BOOL: {
		BOOL_T value;
		if (!lwm2m_data_decode_bool(&data, &value) || !res.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::TIME:{
		TIME_T value;
		if (!lwm2m_data_decode_time(&data, &value) || !res.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::INT: {
		INT_T value;
		if (!lwm2m_data_decode_int(&data, &value) || !res.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::UINT: {
		UINT_T value;
		if (!lwm2m_data_decode_uint(&data, &value) || !res.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::FLOAT: {
		FLOAT_T value;
		if (!lwm2m_data_decode_float(&data, &value) || !res.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::OBJ_LINK: {
		OBJ_LINK_T value;
		if (!lwm2m_data_decode_objlink(&data, &value.objId, &value.objInstId) || !res.set(std::move(value), instanceId)) return false;
		break;
	}
	case TYPE_ID::OPAQUE: {
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING) return false;
		size_t len = 0;
		uint8_t *buffer = NULL;
		if (!lwm2m_data_decode_opaque(&data, &buffer, &len) || !res.set(OPAQUE_T(buffer, buffer+len), instanceId)) {
			if (len) lwm2m_free(buffer);
			return false;
		}
		if (len) lwm2m_free(buffer);
		break;
	}
	case TYPE_ID::STRING: {
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (!res.set(STRING_T(buffer, buffer+len), instanceId)) return false;
		break;
	}
	case TYPE_ID::CORE_LINK: {
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING && data.type != LWM2M_TYPE_CORE_LINK) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (!res.set(CORE_LINK_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	default: return false;
	}

	return true;
}

Resource* Instance::getValidatedResForWrite(const lwm2m_data_t &data, lwm2m_write_type_t writeType, uint8_t &errCode) {
	auto res = resource(data.id);
	if (!IS_RES_PTR_VALID(res)) {
		WPP_LOGW(TAG_WPP_INST, "Resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_404_NOT_FOUND;
		return NULL;
	}
	// Check resource type (single/multiple) matches with data type
	if ((data.type == LWM2M_TYPE_MULTIPLE_RESOURCE && res->isSingle()) ||
		  (data.type != LWM2M_TYPE_MULTIPLE_RESOURCE && res->isMultiple())) {
		WPP_LOGW(TAG_WPP_INST, "Server can not write multiple resource to single and vise verse: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_400_BAD_REQUEST;
		return NULL;
	}
	return &(*res);
}

uint8_t Instance::resourceWrite(lwm2m_server_t *server, Resource &res, const lwm2m_data_t &data, lwm2m_write_type_t writeType) {
	bool isReplace = (writeType == LWM2M_WRITE_REPLACE_INSTANCE || writeType == LWM2M_WRITE_REPLACE_RESOURCES);
	Resource resBackup;

	// Check if data is not empty
	if (data.type == LWM2M_TYPE_MULTIPLE_RESOURCE && data.value.asChildren.count == 0) {
		WPP_LOGW(TAG_WPP_INST, "Resource write: %d:%d:%d, empty data", _id.objId, _id.objInstId, data.id);
		return COAP_400_BAD_REQUEST;
	}

	// Clear resource data if we need to replace it
	if (isReplace) {
		WPP_LOGD(TAG_WPP_INST, "Clear resource before write: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		resBackup = std::move(res);
	}

	size_t count = 1;
	const lwm2m_data_t *data_ptr = &data;
	if (data.type == LWM2M_TYPE_MULTIPLE_RESOURCE) {
		count = data.value.asChildren.count;
		data_ptr = data.value.asChildren.array;
	}

	// If resource is single then this loop execute only once
	for (size_t i = 0; i < count; i++) {
		ID_T resInstId = res.isSingle()? SINGLE_INSTANCE_ID : data_ptr[i].id;
		WPP_LOGD(TAG_WPP_INST, "Resource write: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, resInstId);
		if (!lwm2mDataToResource(data_ptr[i], res, resInstId)) {
			WPP_LOGE(TAG_WPP_INST, "Problem with converting lwm2mData to resource: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, resInstId);
			// Restore resource state
			if (isReplace) res = std::move(resBackup);
			return COAP_400_BAD_REQUEST;
		}
		// Notify implementation about update operation
		if (writeType == LWM2M_WRITE_PARTIAL_UPDATE) serverOperationNotifier(getSecurityInst(server), ItemOp::WRITE, {res.getId(), (res.isSingle()? ID_T_MAX_VAL : resInstId)});
	}
	// Notify implementation about replace resource operation
	if (writeType == LWM2M_WRITE_REPLACE_RESOURCES) serverOperationNotifier(getSecurityInst(server), ItemOp::WRITE, {res.getId(), ID_T_MAX_VAL});

	return COAP_NO_ERROR;
}

Resource* Instance::getValidatedResForRead(const lwm2m_data_t &data, uint8_t &errCode) {
	auto res = resource(data.id);
	if (!IS_PTR_VALID_AND_RES_EXISTS(res)) {
		WPP_LOGW(TAG_WPP_INST, "Resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_404_NOT_FOUND;
		return NULL;
	}
	// Check the server operation permission for resource
	if (!res->getOperation().isRead()) {
		WPP_LOGE(TAG_WPP_INST, "Server does not have permission for read resource: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_400_BAD_REQUEST;
		return NULL;
	}
	// Check resource type (single/multiple) matches with data type
	if (data.type == LWM2M_TYPE_MULTIPLE_RESOURCE && res->isSingle()) {
		WPP_LOGW(TAG_WPP_INST, "Server can not read single resource to multiple: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_400_BAD_REQUEST;
		return NULL;
	}
	// If resource is multile then we should check that all instances are exists
	if (data.type == LWM2M_TYPE_MULTIPLE_RESOURCE) {
		for (size_t i = 0; i < data.value.asChildren.count; i++) {
			if (!res->isExist(data.value.asChildren.array[i].id)) {
				WPP_LOGW(TAG_WPP_INST, "Resource instance does not exist: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, data.value.asChildren.array[i].id);
				errCode = COAP_404_NOT_FOUND;
				return NULL;
			}
		}
	}

	return &(*res);
}

uint8_t Instance::resourceRead(lwm2m_server_t *server, lwm2m_data_t &data, Resource &res) {
	// if has been received data for multiple resource with not allocated memory
	// then we ourselves allocate memory for instances
	if (res.isMultiple() && data.type != LWM2M_TYPE_MULTIPLE_RESOURCE) {
		lwm2m_data_t *subData = lwm2m_data_new(res.instCount());
		lwm2m_data_t *dataCnt = subData;
		for (auto id : res.instIds()) (dataCnt++)->id = id;
		lwm2m_data_encode_instances(subData, res.instCount(), &data);
	}

	size_t count = 1;
	lwm2m_data_t *data_ptr = &data;
	if (data.type == LWM2M_TYPE_MULTIPLE_RESOURCE) {
		count = data.value.asChildren.count;
		data_ptr = data.value.asChildren.array;
	}

	// If resource is single then this loop execute only once
	for (size_t j = 0; j < count; j++) {
		ID_T resInstId = res.isSingle()? SINGLE_INSTANCE_ID : data_ptr[j].id;
		WPP_LOGD(TAG_WPP_INST, "Resource read: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, resInstId);
		if (!resourceToLwm2mData(res, resInstId, data_ptr[j])) {
			WPP_LOGE(TAG_WPP_INST, "Problem with converting resource to lwm2mData: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, resInstId);
			return COAP_400_BAD_REQUEST;
		}
	}

	return COAP_NO_ERROR;
}

Resource* Instance::getValidatedResForExecute(ID_T resId, uint8_t &errCode) {
	auto res = resource(resId);
	if (!IS_RES_PTR_VALID(res)) {
		WPP_LOGW(TAG_WPP_INST, "Resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, resId);
		errCode = COAP_404_NOT_FOUND;
		return NULL;
	}
	// Check the server operation permission for resource
	if (!res->getOperation().isExecute()) {
		WPP_LOGE(TAG_WPP_INST, "Server does not have permission for execute resource: %d:%d:%d", _id.objId, _id.objInstId, resId);
		errCode = COAP_405_METHOD_NOT_ALLOWED;
		return NULL;
	}
	return &(*res);
}

uint8_t Instance::createEmptyLwm2mDataArray(std::vector<Resource*> resources, lwm2m_data_t **dataArray, int *numData) {
	lwm2m_data_t *arr = lwm2m_data_new(resources.size());
	if (!arr) return COAP_500_INTERNAL_SERVER_ERROR;

	lwm2m_data_t *arrCnt = arr;
	for (auto &resource : resources) (arrCnt++)->id = resource->getId();
	*dataArray = arr;
	*numData = resources.size();

	return COAP_NO_ERROR;
}

bool Instance::isAllMandatoryResourcesPresent(int numData, lwm2m_data_t *data){
	// During the replace instance operation, request should contains all
	// mandatory resources for write
	std::vector<Resource *> mandatoryResources;
	for (auto &res : resources()) {
		if (!res.isMandatory() || !res.getOperation().isWrite()) continue;
		bool found = false;
		for (int i = 0; i < numData; i++) {
			if (data[i].id == res.getId()) { 
				found = true;
				break;
			}
		}
		if (!found) {
			WPP_LOGE(TAG_WPP_INST, "Mandatory resource %d:%d:%d does not found", _id.objId, _id.objInstId, res.getId());
			return false;
		}
	}
	return true;
}

uint8_t Instance::replaceInstance(lwm2m_server_t *server, int numData, lwm2m_data_t *dataArray) {
	// Container with updated resources that is used during
	// replace instance operation we do not want to change
	// the original resources to avoid data corruption. After
	// successful writing, we will replace the original resources.
	std::vector<Resource> replaceInstResources;
	// During the replace instance operation, request should contains all
	// mandatory resources for write
	if (_context.state > STATE_BOOTSTRAPPING && !isAllMandatoryResourcesPresent(numData, dataArray)) return COAP_400_BAD_REQUEST;
	
	for (int i = 0; i < numData; i++) {
		uint8_t errCode = COAP_NO_ERROR;
		Resource *res = getValidatedResForWrite(dataArray[i], LWM2M_WRITE_REPLACE_INSTANCE, errCode);
		if (!res) {
			// If resource does not exist that means it is optional resource
			// and optional resources can be missing during write operation
			if (errCode != COAP_404_NOT_FOUND) {
				WPP_LOGW(TAG_WPP_INST, "Resource %d:%d:%d write not possible, error: %d", _id.objId, _id.objInstId, dataArray[i].id, errCode);
				return errCode;
			}
			continue;
		}
		replaceInstResources.push_back(*res);
		errCode = resourceWrite(server, replaceInstResources.back(), dataArray[i], LWM2M_WRITE_REPLACE_INSTANCE);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE(TAG_WPP_INST, "Resource %d:%d:%d write error: %d", _id.objId, _id.objInstId, dataArray[i].id, errCode);
			return errCode;
		}
	}

	// Replace original resources with updated resources
	for (auto &res : replaceInstResources) {
		*resource(res.getId()) = std::move(res);
		serverOperationNotifier(getSecurityInst(server), ItemOp::WRITE, {res.getId(), ID_T_MAX_VAL});
	}

	return COAP_NO_ERROR;
}

uint8_t Instance::replaceResource(lwm2m_server_t *server, int numData, lwm2m_data_t *dataArray, lwm2m_write_type_t writeType) {
	for (int i = 0; i < numData; i++) {
		uint8_t errCode = COAP_NO_ERROR;
		Resource *res = getValidatedResForWrite(dataArray[i], writeType, errCode);
		if (!res) {
			// TODO: According to the documentation, optional resources can be missing when a write
			// or read attempt is made, allowing us to ignore a request to read/write these 
			// resources, but wakaama is implemented in such a way that the behavior of ignoring
			// optional resources results in an incorrect internal registration system state. 
			if (server == NULL || errCode != COAP_404_NOT_FOUND) {
				WPP_LOGW(TAG_WPP_INST, "Resource %d:%d:%d write not possible, error: %d", _id.objId, _id.objInstId, dataArray[i].id, errCode);
				return errCode;
			}
			continue;
		}
		errCode = resourceWrite(server, *res, dataArray[i], writeType);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE(TAG_WPP_INST, "Resource %d:%d:%d write error: %d", _id.objId, _id.objInstId, dataArray[i].id, errCode);
			return errCode;
		}
	}

	return COAP_NO_ERROR;
}

uint8_t Instance::readAsServer(lwm2m_server_t *server, int *numData, lwm2m_data_t **dataArray) {
	// TODO: Read-Composite Operation for now not supported
	if (numData == NULL || dataArray == NULL) return COAP_500_INTERNAL_SERVER_ERROR;

	bool readAllAvailableRes = *numData == 0;
	WPP_LOGD(TAG_WPP_INST, "Read %d:%d, readAllAvailableRes: %d, numData: %d", _id.objId, _id.objInstId, readAllAvailableRes, *numData);
	if (readAllAvailableRes) {
		std::vector<Resource *> readResources = getInstantiatedResList(ItemOp(ItemOp::READ));
		uint8_t errCode = createEmptyLwm2mDataArray(readResources, dataArray, numData);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE(TAG_WPP_INST, "Error during creating lwm2m_data_t for read instance %d:%d", _id.objId, _id.objInstId);
			return errCode;
		}
	}
	
	uint8_t result = COAP_404_NOT_FOUND;
	for (int i = 0; i < *numData; i++) {
		uint8_t errCode = COAP_NO_ERROR;
		lwm2m_data_t *data = (*dataArray) + i;
		Resource *res = getValidatedResForRead(*data, errCode);
		if (!res) {
			// TODO: According to the documentation, optional resources can be missing when a write
			// or read attempt is made, allowing us to ignore a request to read/write these 
			// resources, but wakaama is implemented in such a way that the behavior of ignoring
			// optional resources results in an incorrect internal registration system state. 
			if (server == NULL || errCode != COAP_404_NOT_FOUND) {
				WPP_LOGW(TAG_WPP_INST, "Resource %d:%d:%d read not possible, error: %d", _id.objId, _id.objInstId, data->id, errCode);
				return errCode;
			}
			continue;
		}
		
		errCode = resourceRead(server, *data, *res);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE(TAG_WPP_INST, "Resource %d:%d:%d read error: %d", _id.objId, _id.objInstId, data->id, errCode);
			return errCode;
		}
		// For successful read operation we should read at least one resource
		result = COAP_205_CONTENT;
	}

	return result;
}

uint8_t Instance::writeAsServer(lwm2m_server_t *server, int numData, lwm2m_data_t *dataArray, lwm2m_write_type_t writeType) {
	// TODO: Write-Composite Operation for now not supported
	if (!numData) return COAP_204_CHANGED;
	if (dataArray == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
	WPP_LOGD(TAG_WPP_INST, "Write %d:%d, numData: %d, write type %d", _id.objId, _id.objInstId, numData, writeType);

	// Check the server operation permission for resource.
	// Bootstrap server can write even if resource is not writable.
	for (int i = 0; i < numData; i++) {
		auto res = resource(dataArray[i].id);
		if (!IS_RES_PTR_VALID(res)) continue;
		if (!res->getOperation().isWrite() && (_context.state > STATE_BOOTSTRAPPING) && server != NULL) {
			WPP_LOGE(TAG_WPP_INST, "Trying to write read-only resource %d:%d:%d", _id.objId, _id.objInstId, dataArray[i].id);
			return COAP_405_METHOD_NOT_ALLOWED;
		}
	}

	uint8_t errCode = COAP_NO_ERROR;
	if (writeType == LWM2M_WRITE_REPLACE_INSTANCE) errCode = replaceInstance(server, numData, dataArray);
	else errCode = replaceResource(server, numData, dataArray, writeType);

	return (errCode == COAP_NO_ERROR)? COAP_204_CHANGED : errCode;
}

uint8_t Instance::executeAsServer(lwm2m_server_t *server, ID_T resId, uint8_t * buffer, int length) {
	WPP_LOGD(TAG_WPP_INST, "Execute %d:%d:%d, buffer length: %d", _id.objId, _id.objInstId, resId, length);
	uint8_t errCode = COAP_NO_ERROR;
	Resource *res = getValidatedResForExecute(resId, errCode);
	if (!res) {
		WPP_LOGE(TAG_WPP_INST, "Resource %d:%d:%d execute not possible", _id.objId, _id.objInstId, resId);
		return errCode;
	}
	
	EXECUTE_T execute = res->get<EXECUTE_T>();
	if (!execute) {
		WPP_LOGE(TAG_WPP_INST, "Resource value is not set: %d:%d:%d", _id.objId, _id.objInstId, resId);
		return COAP_404_NOT_FOUND;
	}

	// Notify implementation about execute resource operation
	serverOperationNotifier(getSecurityInst(server), ItemOp::EXECUTE, {res->getId(), ID_T_MAX_VAL});

	WPP_LOGD(TAG_WPP_INST, "Resource execute: %d:%d:%d, buffer length: %d", _id.objId, _id.objInstId, resId, length);
	return execute(*this, resId, OPAQUE_T(buffer, buffer + length))? COAP_204_CHANGED : COAP_405_METHOD_NOT_ALLOWED;
}

uint8_t Instance::discoverAsServer(lwm2m_server_t *server, int * numData, lwm2m_data_t ** dataArray) {
	if (numData == NULL || dataArray == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
	
	bool discoverAllAvailableRes = *numData == 0;
	WPP_LOGD(TAG_WPP_INST, "Discover %d:%d, discoverAllAvailableRes: %d, numData: %d", _id.objId, _id.objInstId, discoverAllAvailableRes, *numData);
	if (discoverAllAvailableRes) {
		std::vector<Resource *> resources = getResList();
		uint8_t errCode = createEmptyLwm2mDataArray(resources, dataArray, numData);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE(TAG_WPP_INST, "Error during creating lwm2m_data_t for discover instance %d:%d", _id.objId, _id.objInstId);
			return errCode;
		}
	}

	for (int i = 0; i < *numData; i++) {
		lwm2m_data_t *data = (*dataArray) + i;
		auto res = resource(data->id);
		if (!IS_RES_PTR_VALID(res)) {
			WPP_LOGE(TAG_WPP_INST, "Resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data->id);
			return COAP_404_NOT_FOUND;
		}

		// if has been received data for multiple resource with not allocated memory
		// then we ourselves allocate memory for instances
		if (res->isMultiple() && res->instCount() && data->type != LWM2M_TYPE_MULTIPLE_RESOURCE) {
			lwm2m_data_t *subData = lwm2m_data_new(res->instCount());
			lwm2m_data_t *dataCnt = subData;
			for (auto id : res->instIds()) {
				(dataCnt++)->id = id;
				WPP_LOGD(TAG_WPP_INST, "Resource instance discover: %d:%d:%d:%d", _id.objId, _id.objInstId, data->id, id);
			}
			lwm2m_data_encode_instances(subData, res->instCount(), data);
		} else {
			WPP_LOGD(TAG_WPP_INST, "Resource discover: %d:%d:%d", _id.objId, _id.objInstId, data->id);
		}
	}
	return COAP_205_CONTENT;
}

} // namespcae wpp

