/*
 * Instnace.cpp
 *
 *  Created on: 17 Jul 2023
 *      Author: valentin
 */

#include "Instance.h"
#include "WppLogs.h"

#define IS_RES_EXISTS(res) (res && !res->isEmpty())

namespace wpp {

bool Instance::clear(ID_T resId) {
	Resource *const resource = getResource(resId);
	if (!resource) return false;

	bool result = resource->clear();
	if (result) {
		notifyValueChanged({_id, {resId,}});
		userOperationNotifier(ResOp::DELETE, {resId,});
	}

	return result;
}

bool Instance::remove(const ResLink &resId) {
	Resource *const resource = getResource(resId.resInstId);
	if (!resource) return false;

	bool result = resource->remove(resId.resInstId);
	if (result) {
		notifyValueChanged({_id, {resId.resId, resId.resInstId}});
		userOperationNotifier(ResOp::DELETE, {resId.resId, resId.resInstId});
	}

	return result;
}

void Instance::notifyValueChanged(const DataLink &data) {
	WPP_LOGD_ARG(TAG_WPP_INST, "Notify value changed: objID=%d, instID=%d, resID=%d, resInstID=%d", data.instance.objId, data.instance.objInstId, data.resource.resId, data.resource.resInstId);	
	lwm2m_uri_t uri = {data.instance.objId, data.instance.objInstId, data.resource.resId, data.resource.resInstId};
	lwm2m_resource_value_changed(&_context, &uri);
}

bool Instance::resourceToLwm2mData(Resource &resource, ID_T instanceId, lwm2m_data_t &data) {
	switch(resource.getTypeId()) {
	case TYPE_ID::BOOL: {
		BOOL_T value;
		if (resource.get(value, instanceId)) lwm2m_data_encode_bool(value, &data);
		break;
	}
	case TYPE_ID::TIME:
	case TYPE_ID::INT: {
		INT_T value;
		if (resource.get(value, instanceId)) lwm2m_data_encode_int(value, &data);
		break;
	}
	case TYPE_ID::UINT: {
		UINT_T value;
		if (resource.get(value, instanceId)) lwm2m_data_encode_uint(value, &data);
		break;
	}
	case TYPE_ID::FLOAT: {
		FLOAT_T value;
		if (resource.get(value, instanceId)) lwm2m_data_encode_float(value, &data);
		break;
	}
	case TYPE_ID::OBJ_LINK: {
		OBJ_LINK_T value;
		if (resource.get(value, instanceId)) lwm2m_data_encode_objlink(value.objId, value.objInstId, &data);
		break;
	}
	case TYPE_ID::OPAQUE: {
		OPAQUE_T value;
		if (resource.get(value, instanceId)) lwm2m_data_encode_opaque(value.data(), value.size(), &data);
		break;
	}
	case TYPE_ID::STRING: {
		STRING_T value;
		if (resource.get(value, instanceId)) lwm2m_data_encode_string(value.c_str(), &data);
		break;
	}
	case TYPE_ID::CORE_LINK: {
		CORE_LINK_T value;
		if (resource.get(value, instanceId)) lwm2m_data_encode_corelink(value.c_str(), &data);
		break;
	}
	default: return false;
	}

	return true;
}

bool Instance::lwm2mDataToResource(const lwm2m_data_t &data, Resource &resource, ID_T instanceId) {
	switch (resource.getTypeId()) {
	case TYPE_ID::BOOL: {
		BOOL_T value;
		if (!lwm2m_data_decode_bool(&data, &value) || !resource.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::TIME:
	case TYPE_ID::INT: {
		INT_T value;
		if (!lwm2m_data_decode_int(&data, &value) || !resource.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::UINT: {
		UINT_T value;
		if (!lwm2m_data_decode_uint(&data, &value) || !resource.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::FLOAT: {
		FLOAT_T value;
		if (!lwm2m_data_decode_float(&data, &value) || !resource.set(value, instanceId)) return false;
		break;
	}
	case TYPE_ID::OBJ_LINK: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		if (data.type != LWM2M_TYPE_OBJECT_LINK) return false;
		if (!resource.set(OBJ_LINK_T{ID_T_MAX_VAL, ID_T_MAX_VAL}, instanceId)) return false;
		break;
	}
	case TYPE_ID::OPAQUE: {
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (!resource.set(OPAQUE_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	case TYPE_ID::STRING: {
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (!resource.set(STRING_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	case TYPE_ID::CORE_LINK: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING && data.type != LWM2M_TYPE_CORE_LINK) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (!resource.set(CORE_LINK_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	default: return false;
	}

	return true;
}

Resource* Instance::getValidatedResForWrite(const lwm2m_data_t &data, lwm2m_write_type_t writeType, uint8_t &errCode) {
	// TODO: In some cases, according to the implementation of the wakaama,
	// the resources marked as R can be written by the server during the
	// instance creation operation (Ex: ACL object resource 0). I did not
	// find the necessary description in the documentation, so this question
	// needs to be investigated in detail.

	Resource *resource = getResource(data.id);
	if (!IS_RES_EXISTS(resource)) {
		WPP_LOGW_ARG(TAG_WPP_INST, "Resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_404_NOT_FOUND;
		return NULL;
	}
	// Check the server operation permission for resource.
	// Bootstrap server can write evan if resource is not writable.
	// Also allowed to write R resources during instance creation.
	if (!resource->getOperation().isWrite() && 
	      (_context.state != STATE_BOOTSTRAPPING) && 
	      (writeType != LWM2M_WRITE_REPLACE_INSTANCE)) {
		WPP_LOGW_ARG(TAG_WPP_INST, "Server does not have permission for write resource: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_405_METHOD_NOT_ALLOWED;
		return NULL;
	}
	// Check resource type (single/multiple) matches with data type
	if ((data.type == LWM2M_TYPE_MULTIPLE_RESOURCE && resource->isSingle()) ||
		  (data.type != LWM2M_TYPE_MULTIPLE_RESOURCE && resource->isMultiple())) {
		WPP_LOGW_ARG(TAG_WPP_INST, "Server can not write multiple resource to single and vise verse: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_405_METHOD_NOT_ALLOWED;
		return NULL;
	}

	return resource;
}

uint8_t Instance::resourceWrite(Resource &res, const lwm2m_data_t &data, lwm2m_write_type_t writeType) {
	bool isReplace = (writeType == LWM2M_WRITE_REPLACE_INSTANCE || writeType == LWM2M_WRITE_REPLACE_RESOURCES);
	Resource resBackup;

	// Clear resource data if we need to replace it
	if (isReplace) {
		WPP_LOGD_ARG(TAG_WPP_INST, "Clear resource before write: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		resBackup = std::move(res);
		res.clear();
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
		WPP_LOGD_ARG(TAG_WPP_INST, "Resource write: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, resInstId);
		if (!lwm2mDataToResource(data_ptr[i], res, resInstId)) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Problem with converting lwm2mData to resource: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, resInstId);
			// Restore resource state
			if (isReplace) res = std::move(resBackup);
			return COAP_400_BAD_REQUEST;
		}
		// Notify implementation about update operation
		if (writeType == LWM2M_WRITE_PARTIAL_UPDATE) serverOperationNotifier(ResOp::WRITE_UPD, {res.getId(), (res.isSingle()? ID_T_MAX_VAL : resInstId)});
	}
	// Notify implementation about replace resource operation
	if (writeType == LWM2M_WRITE_REPLACE_RESOURCES) serverOperationNotifier(ResOp::WRITE_REPLACE_RES, {res.getId(), ID_T_MAX_VAL});

	return COAP_NO_ERROR;
}

Resource* Instance::getValidatedResForRead(const lwm2m_data_t &data, uint8_t &errCode) {
	Resource *resource = getResource(data.id);
	if (!IS_RES_EXISTS(resource)) {
		WPP_LOGW_ARG(TAG_WPP_INST, "Resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_404_NOT_FOUND;
		return NULL;
	}
	// Check the server operation permission for resource
	if (!resource->getOperation().isRead()) {
		WPP_LOGE_ARG(TAG_WPP_INST, "Server does not have permission for read resource: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_405_METHOD_NOT_ALLOWED;
		return NULL;
	}
	// Check resource type (single/multiple) matches with data type
	if (data.type == LWM2M_TYPE_MULTIPLE_RESOURCE && resource->isSingle()) {
		WPP_LOGW_ARG(TAG_WPP_INST, "Server can not read single resource to multiple: %d:%d:%d", _id.objId, _id.objInstId, data.id);
		errCode = COAP_405_METHOD_NOT_ALLOWED;
		return NULL;
	}

	return resource;
}

uint8_t Instance::resourceRead(lwm2m_data_t &data, Resource &res) {
	// if has been received data for multiple resource with not allocated memory
	// then we ourselves allocate memory for instances
	if (res.isMultiple() && data.type != LWM2M_TYPE_MULTIPLE_RESOURCE) {
		lwm2m_data_t *subData = lwm2m_data_new(res.instanceCnt());
		lwm2m_data_t *dataCnt = subData;
		for (const auto& inst : res.getInstances()) (dataCnt++)->id = inst.id;
		lwm2m_data_encode_instances(subData, res.instanceCnt(), &data);
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
		WPP_LOGD_ARG(TAG_WPP_INST, "Resource read: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, resInstId);
		if (!resourceToLwm2mData(res, resInstId, data_ptr[j])) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Problem with converting resource to lwm2mData: %d:%d:%d:%d", _id.objId, _id.objInstId, data.id, resInstId);
			return COAP_400_BAD_REQUEST;
		}
		// Notify implementation about read resource operation
		serverOperationNotifier(ResOp::READ, {res.getId(), (res.isSingle()? ID_T_MAX_VAL : resInstId)});
	}

	return COAP_NO_ERROR;
}

Resource* Instance::getValidatedResForExecute(ID_T resId, uint8_t &errCode) {
	Resource *resource = getResource(resId);
	if (!IS_RES_EXISTS(resource)) {
		WPP_LOGW_ARG(TAG_WPP_INST, "Resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, resId);
		errCode = COAP_404_NOT_FOUND;
		return NULL;
	}
	// Check the server operation permission for resource
	if (!resource->getOperation().isExecute()) {
		WPP_LOGE_ARG(TAG_WPP_INST, "Server does not have permission for execute resource: %d:%d:%d", _id.objId, _id.objInstId, resId);
		errCode = COAP_405_METHOD_NOT_ALLOWED;
		return NULL;
	}
	return resource;
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

uint8_t Instance::read(int *numData, lwm2m_data_t **dataArray) {
	// TODO: Read-Composite Operation for now not supported
	bool readAllAvailableRes = *numData == 0;
	WPP_LOGD_ARG(TAG_WPP_INST, "Read %d:%d, readAllAvailableRes: %d, numData: %d", _id.objId, _id.objInstId, readAllAvailableRes, *numData);
	if (readAllAvailableRes) {
		std::vector<Resource *> readResources = getInstantiatedResourcesList(ResOp(ResOp::READ));
		uint8_t errCode = createEmptyLwm2mDataArray(readResources, dataArray, numData);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Error during creating lwm2m_data_t for read instance %d:%d", _id.objId, _id.objInstId);
			return errCode;
		}
	}

	// TODO: According to the documentation, optional resources can be missing when a write
	// or read attempt is made, allowing us to ignore a request to read/write these 
	// resources, but wakaama is implemented in such a way that the behavior of ignoring
	// optional resources results in an incorrect internal registration system state. 
	// One of the workarounds is to determine the number of resources that are read or 
	// written, wakaama reads resources only one at a time, this allows to determine that
	// the reading or writing is done by wakaama core and set the correct behavior in the
	// absence of a resource.
	bool ignore = *numData > 1;
	
	for (int i = 0; i < *numData; i++) {
		uint8_t errCode = COAP_NO_ERROR;
		lwm2m_data_t *data = (*dataArray) + i;
		Resource *resource = getValidatedResForRead(*data, errCode);
		if (!resource) {
			WPP_LOGW_ARG(TAG_WPP_INST, "Resource %d:%d:%d read not possible, ignore: %d", _id.objId, _id.objInstId, data->id, ignore);
			if (ignore) continue;
			else return errCode;
		}
		
		errCode = resourceRead(*data, *resource);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Resource %d:%d:%d read error: %d", _id.objId, _id.objInstId, data->id, errCode);
			return errCode;
		}
	}

	return COAP_205_CONTENT;
}

uint8_t Instance::write(int numData, lwm2m_data_t *dataArray, lwm2m_write_type_t writeType) {
	// TODO: Write-Composite Operation for now not supported

	// TODO: According to the documentation, optional resources can be missing when a write
	// or read attempt is made, allowing us to ignore a request to read/write these 
	// resources, but wakaama is implemented in such a way that the behavior of ignoring
	// optional resources results in an incorrect internal registration system state. 
	// One of the workarounds is to determine the number of resources that are read or 
	// written, wakaama reads resources only one at a time, this allows to determine that
	// the reading or writing is done by wakaama core and set the correct behavior in the
	// absence of a resource.
	bool ignore = numData > 1;
	WPP_LOGD_ARG(TAG_WPP_INST, "Write %d:%d, ignore: %d, numData: %d, write type %d", _id.objId, _id.objInstId, ignore, numData, writeType);

	// During the replace instance we should reset instance to default
	// state and then write with replace all resources in array. During
	// this operation, we notify the instance only one time after the 
	// writing is completed.
	bool isReplaceInstance = writeType == LWM2M_WRITE_REPLACE_INSTANCE;
	if (isReplaceInstance) setDefaultState();
	
	for (int i = 0; i < numData; i++) {
		uint8_t errCode = COAP_NO_ERROR;
		Resource *resource = getValidatedResForWrite(dataArray[i], writeType, errCode);
		if (!resource) {
			WPP_LOGW_ARG(TAG_WPP_INST, "Resource %d:%d:%d write not possible, ignore: %d", _id.objId, _id.objInstId, dataArray[i].id, ignore);
			if (ignore) continue;
			else return errCode;
		}

		errCode = resourceWrite(*resource, dataArray[i], writeType);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Resource %d:%d:%d write error: %d", _id.objId, _id.objInstId, dataArray[i].id, errCode);
			return errCode;
		}
	}
	// Notify implementation about replace instance operation
	if (isReplaceInstance) serverOperationNotifier(ResOp::WRITE_REPLACE_INST, {ID_T_MAX_VAL, ID_T_MAX_VAL});

	return COAP_204_CHANGED;
}

uint8_t Instance::execute(ID_T resId, uint8_t * buffer, int length) {
	WPP_LOGD_ARG(TAG_WPP_INST, "Execute %d:%d:%d, buffer length: %d", _id.objId, _id.objInstId, resId, length);
	uint8_t errCode = COAP_NO_ERROR;
	Resource *resource = getValidatedResForExecute(resId, errCode);
	if (!resource) {
		WPP_LOGE_ARG(TAG_WPP_INST, "Resource %d:%d:%d execute not possible", _id.objId, _id.objInstId, resId);
		return errCode;
	}
	
	EXECUTE_T execute;
	if (!resource->get(execute) || !execute) {
		WPP_LOGE_ARG(TAG_WPP_INST, "Resource value is not set: %d:%d:%d", _id.objId, _id.objInstId, resId);
		return COAP_404_NOT_FOUND;
	}

	WPP_LOGD_ARG(TAG_WPP_INST, "Resource execute: %d:%d:%d, buffer length: %d", _id.objId, _id.objInstId, resId, length);
	execute(resId, OPAQUE_T(buffer, buffer + length));
	// Notify implementation about execute resource operation
	serverOperationNotifier(ResOp::EXECUTE, {resource->getId(), ID_T_MAX_VAL});

	return COAP_204_CHANGED;
}

uint8_t Instance::discover(int * numData, lwm2m_data_t ** dataArray) {
	bool discoverAllAvailableRes = *numData == 0;
	WPP_LOGD_ARG(TAG_WPP_INST, "Discover %d:%d, discoverAllAvailableRes: %d, numData: %d", _id.objId, _id.objInstId, discoverAllAvailableRes, *numData);
	if (discoverAllAvailableRes) {
		std::vector<Resource *> resources = getInstantiatedResourcesList();
		uint8_t errCode = createEmptyLwm2mDataArray(resources, dataArray, numData);
		if (errCode != COAP_NO_ERROR) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Error during creating lwm2m_data_t for discover instance %d:%d", _id.objId, _id.objInstId);
			return errCode;
		}
	}

	for (int i = 0; i < *numData; i++) {
		lwm2m_data_t *data = (*dataArray) + i;
		Resource *resource = getResource(data->id);
		if (!IS_RES_EXISTS(resource)) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data->id);
			return COAP_404_NOT_FOUND;
		}

		// if has been received data for multiple resource with not allocated memory
		// then we ourselves allocate memory for instances
		if (resource->isMultiple() && data->type != LWM2M_TYPE_MULTIPLE_RESOURCE) {
			lwm2m_data_t *subData = lwm2m_data_new(resource->instanceCnt());
			lwm2m_data_t *dataCnt = subData;
			for (const auto& inst : resource->getInstances()) {
				(dataCnt++)->id = inst.id;
				WPP_LOGD_ARG(TAG_WPP_INST, "Resource instance discover: %d:%d:%d:%d", _id.objId, _id.objInstId, data->id, inst.id);
				// Notify implementation about discover resource instance operation
				serverOperationNotifier(ResOp::DISCOVER, {resource->getId(), inst.id});
			}
			lwm2m_data_encode_instances(subData, resource->instanceCnt(), data);
		} else {
			WPP_LOGD_ARG(TAG_WPP_INST, "Resource discover: %d:%d:%d", _id.objId, _id.objInstId, data->id);
			// Notify implementation about discover resource operation
			serverOperationNotifier(ResOp::DISCOVER, {resource->getId(), ID_T_MAX_VAL});
		}
	}
	return COAP_205_CONTENT;
}

} // namespcae wpp

