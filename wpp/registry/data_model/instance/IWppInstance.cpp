/*
 * Instnace.cpp
 *
 *  Created on: 17 Jul 2023
 *      Author: valentin
 */

#include "IWppInstance.h"
#include "WppLogs.h"

namespace wpp {

bool IWppInstance::clear(ID_T resId) {
	Resource *const resource = getResource(resId);
	if (!resource) return false;

	bool result = resource->clear();
	if (result) {
		client().notifyValueChanged({_id, {resId,}});
		userOperationNotifier(Operation::DELETE, {resId,});
	}

	return resource->clear();
}

bool IWppInstance::remove(const ResourceID &resId) {
	Resource *const resource = getResource(resId.resInstId);
	if (!resource) return false;

	bool result = resource->remove(resId.resInstId);
	if (result) {
		client().notifyValueChanged({_id, {resId.resId, resId.resInstId}});
		userOperationNotifier(Operation::DELETE, {resId.resId, resId.resInstId});
	}

	return result;
}

WppClient& IWppInstance::client() {
	return _client;
}

bool IWppInstance::resourceToLwm2mData(Resource &resource, ID_T instanceId, lwm2m_data_t &data) {
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

bool IWppInstance::lwm2mDataToResource(const lwm2m_data_t &data, Resource &resource, ID_T instanceId) {
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
		if (resource.set(OBJ_LINK_T{ID_T_MAX_VAL, ID_T_MAX_VAL}, instanceId)) return false;
		break;
	}
	case TYPE_ID::OPAQUE: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		if (data.type != LWM2M_TYPE_OPAQUE) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (resource.set(OPAQUE_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	case TYPE_ID::STRING: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (resource.set(STRING_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	case TYPE_ID::CORE_LINK: {
		// TODO: It is necessary to check in practice, it is not completely clear how to convert data
		if (data.type != LWM2M_TYPE_OPAQUE && data.type != LWM2M_TYPE_STRING && data.type != LWM2M_TYPE_CORE_LINK) return false;
		size_t len = data.value.asBuffer.length;
		uint8_t *buffer =  data.value.asBuffer.buffer;
		if (resource.set(CORE_LINK_T(buffer, buffer + len), instanceId)) return false;
		break;
	}
	default: return false;
	}

	return true;
}

uint8_t IWppInstance::resourceRead(ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP) {
	// TODO: Read-Composite Operation for now not supported

	// Requested each resource
	if (!*numDataP) {
		std::vector<Resource *> readResources = getInstantiatedResourcesList(Operation(Operation::READ));

		*dataArrayP = lwm2m_data_new(readResources.size());
		if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
		*numDataP = readResources.size();

		lwm2m_data_t *tmp_data_p = *dataArrayP;
		for (auto &resource : readResources) (tmp_data_p++)->id = resource->getID();
	}

	for (int i = 0; i < *numDataP; i++) {
		lwm2m_data_t *data = (*dataArrayP) + i;

		Resource *resource = getResource(data->id);
		//  Note that availability is not mandatory for optional resources
		if (resource == NULL || resource->isEmpty()) {
			if (resource->isOptional()) {
				WPP_LOGW_ARG(TAG_WPP_INST, "Optional resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data->id);
				continue;
			} else {
				WPP_LOGE_ARG(TAG_WPP_INST, "Mandatory resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data->id);
				return COAP_404_NOT_FOUND;
			}
		}

		// Check the server operation permission for resource
		if (!resource->getOperation().isRead()) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Server does not have permission for read resource: %d:%d:%d", _id.objId, _id.objInstId, data->id);
			return COAP_405_METHOD_NOT_ALLOWED;
		}

		// if has been received data for multiple resource with not allocated memory
		// then we ourselves allocate memory for instances
		if (resource->isMultiple() && data->type != LWM2M_TYPE_MULTIPLE_RESOURCE) {
			lwm2m_data_t *subData = lwm2m_data_new(resource->instanceCnt());
			lwm2m_data_t *dataCnt = subData;
			for (const auto& pair : resource->getInstances()) (dataCnt++)->id = pair.first;
			lwm2m_data_encode_instances(subData, resource->instanceCnt(), data);
		}

		size_t count = 1;
		lwm2m_data_t *data_ptr = data;
		if (data->type == LWM2M_TYPE_MULTIPLE_RESOURCE) {
			count = data->value.asChildren.count;
			data_ptr = data->value.asChildren.array;
		}

		// If resource is single then this loop execute only once
		for (size_t j = 0; j < count; j++) {
			ID_T resInstId = resource->isSingle()? SINGLE_INSTANCE_ID : data_ptr[j].id;
			//  Note that availability is not mandatory for optional resources
			if (!resourceToLwm2mData(*resource, resInstId, data_ptr[j])) {
				if (resource->isOptional()) {
					WPP_LOGW_ARG(TAG_WPP_INST, "Problem with converting optional resource to lwm2mData: %d:%d:%d:%d", _id.objId, _id.objInstId, data->id, resInstId);
					continue;
				} else {
					WPP_LOGE_ARG(TAG_WPP_INST, "Problem with converting mandatory resource to lwm2mData: %d:%d:%d:%d", _id.objId, _id.objInstId, data->id, resInstId);
					return COAP_404_NOT_FOUND;
				}
			}
			// If execution get to this place then operation completed with
			// success and we can notifyIWppInstance implementation about it
			serverOperationNotifier(Operation::READ, {resource->getID(), resInstId});
		}
	}

	return COAP_205_CONTENT;
}

uint8_t IWppInstance::resourceWrite(ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_write_type_t writeType) {
	// Protect access to instance list

	// TODO: In some cases, according to the implementation of the wakaama,
	// the resources marked as R can be written by the server during the
	// instance creation operation (Ex: ACL object resource 0). I did not
	// find the necessary description in the documentation, so this question
	// needs to be investigated in detail.
	// TODO: Write-Composite Operation for now not supported
	
	for (int i = 0; i < numData; i++) {
		Resource *resource = getResource(dataArray[i].id);
		if (resource == NULL) {
			if (resource->isOptional()) {
				WPP_LOGW_ARG(TAG_WPP_INST, "Optional resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, dataArray[i].id);
				continue;
			} else {
				WPP_LOGE_ARG(TAG_WPP_INST, "Mandatory resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, dataArray[i].id);
				return COAP_404_NOT_FOUND;
			}
		}
	
		// Check the server operation permission for resource
		if (!resource->getOperation().isWrite()) {
			WPP_LOGE_ARG(TAG_WPP_INST, "Server does not have permission for write resource: %d:%d:%d", _id.objId, _id.objInstId, dataArray[i].id);
			return COAP_405_METHOD_NOT_ALLOWED;
		}
		if ((dataArray[i].type == LWM2M_TYPE_MULTIPLE_RESOURCE && resource->isSingle()) ||
			(dataArray[i].type != LWM2M_TYPE_MULTIPLE_RESOURCE && resource->isMultiple())) {
				WPP_LOGE_ARG(TAG_WPP_INST, "Server can not write multiple resource to single and vise verse: %d:%d:%d", _id.objId, _id.objInstId, dataArray[i].id);
				return COAP_405_METHOD_NOT_ALLOWED;
		}

		// Clear resource data if we need to replace it
		if (writeType == LWM2M_WRITE_REPLACE_RESOURCES) {
			WPP_LOGD_ARG(TAG_WPP_INST, "Clear resource before write: %d:%d:%d", _id.objId, _id.objInstId, dataArray[i].id);
			resource->clear();
			// Notify IWppInstance implementation about operation
			serverOperationNotifier(Operation::DELETE, {resource->getID(), SINGLE_INSTANCE_ID});
		}

		size_t count = 1;
		const lwm2m_data_t *data_ptr = dataArray + i;
		if (dataArray[i].type == LWM2M_TYPE_MULTIPLE_RESOURCE) {
			count = dataArray[i].value.asChildren.count;
			data_ptr = dataArray[i].value.asChildren.array;
		}

		// If resource is single then this loop execute only once
		for (size_t j = 0; j < count; j++) {
			ID_T resInstId = resource->isSingle()? SINGLE_INSTANCE_ID : data_ptr[j].id;
			//  Note that availability is not mandatory for optional resources
			if (!lwm2mDataToResource(data_ptr[j], *resource, resInstId)) {
				if (resource->isOptional()) {
					WPP_LOGW_ARG(TAG_WPP_INST, "Problem with converting lwm2mData to optional resource: %d:%d:%d:%d", _id.objId, _id.objInstId, dataArray[i].id, resInstId);
					continue;
				} else {
					WPP_LOGE_ARG(TAG_WPP_INST, "Problem with converting lwm2mData to mandatory resource: %d:%d:%d:%d", _id.objId, _id.objInstId, dataArray[i].id, resInstId);
					return COAP_404_NOT_FOUND;
				}
			}
			// If execution get to this place then operation completed with
			// success and we can notify IWppInstance implementation about it
			serverOperationNotifier(Operation::WRITE, {resource->getID(), resInstId});
		}
	}

	return COAP_204_CHANGED;
}

uint8_t IWppInstance::resourceExecute(ID_T instanceId, ID_T resId, uint8_t * buffer, int length) {
	EXECUTE_T execute;
	Resource *resource = getResource(resId);
	if (!resource || !resource->get(execute) || !execute) {
		if (resource->isOptional()) {
			WPP_LOGW_ARG(TAG_WPP_INST, "Optional resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, resId);
			return COAP_204_CHANGED;
		} else {
			WPP_LOGE_ARG(TAG_WPP_INST, "Mandatory resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, resId);
			return COAP_405_METHOD_NOT_ALLOWED;
		}
	}
	// Check the server operation permission for resource
	if (!resource->getOperation().isExecute()) {
		WPP_LOGE_ARG(TAG_WPP_INST, "Server does not have permission for execute resource: %d:%d:%d", _id.objId, _id.objInstId, resId);
		return COAP_405_METHOD_NOT_ALLOWED;
	}

	execute(resId, OPAQUE_T(buffer, buffer + length));

	// If execution get to this place then operation completed with
	// success and we can notify IWppInstance implementation about it
	serverOperationNotifier(Operation::EXECUTE, {resource->getID(), SINGLE_INSTANCE_ID});

	return COAP_204_CHANGED;
}

uint8_t IWppInstance::resourceDiscover(ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP) {
	// Requested each resource
	if (!*numDataP) {
		std::vector<Resource *> resources = getResourcesList();

		*dataArrayP = lwm2m_data_new(resources.size());
		if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
		*numDataP = resources.size();

		lwm2m_data_t *tmp_data_p = *dataArrayP;
		for (auto &resource : resources) {
			(tmp_data_p++)->id = resource->getID();
		}
	}

	for (int i = 0; i < *numDataP; i++) {
		lwm2m_data_t *data = (*dataArrayP) + i;
		Resource *resource = getResource(data->id);
		if (resource == NULL || resource->isEmpty()) {
			if (resource->isOptional()) {
				WPP_LOGW_ARG(TAG_WPP_INST, "Optional resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data->id);
				continue;
			} else {
				WPP_LOGE_ARG(TAG_WPP_INST, "Mandatory resource does not exist: %d:%d:%d", _id.objId, _id.objInstId, data->id);
				return COAP_404_NOT_FOUND;
			}
		}

		// if has been received data for multiple resource with not allocated memory
		// then we ourselves allocate memory for instances
		if (resource->isMultiple() && data->type != LWM2M_TYPE_MULTIPLE_RESOURCE) {
			lwm2m_data_t *subData = lwm2m_data_new(resource->instanceCnt());
			lwm2m_data_t *dataCnt = subData;
			for (const auto& pair : resource->getInstances()) {
				(dataCnt++)->id = pair.first;
				// If execution get to this place then operation completed with
				// success and we can notifyIWppInstance implementation about it
				serverOperationNotifier(Operation::DISCOVER, {resource->getID(), pair.first});
			}
			lwm2m_data_encode_instances(subData, resource->instanceCnt(), data);
		}
	}
	return COAP_205_CONTENT;
}

} // namespcae wpp

