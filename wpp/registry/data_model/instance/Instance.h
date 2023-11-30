/*
 *Instance.h
 *
 *  Created on: 6 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_INSTANCE_H_
#define WPP_INSTANCE_H_

#include <vector>

#include "liblwm2m.h"
#include "Resource.h"
#include "ResOp.h"
#include "types.h"
#include "InstSubject.h"

namespace wpp {

/**
 * @brief Instance is interface class that implements manipulation with derived class resources.
 * The main target of this class is to encapsulate operations like resource write and read by core, for avoid
 * multiple definition of this mechanism in instance implementation classes. And give generalised
 * interface to resources for core(server).
 *
 * Note: It should be noted that the interface of the instance responsible for communication
 * with the client is specified through get()/set() methods, but the developer can define his
 * interface at the Instance implementation level, whether it will be setters and getters or
 * more abstract methods with algorithm encapsulation, it is not important. It is important
 * that the developer is responsible for notifying the core about resource changes, if the value
 * of any of the resources changes bypassing the Instance::get()/set() methods, then the developer
 * must immediately call the method WppClient::notifyServerResChanged() or the one that encapsulates
 * this call. It is necessary to notify about the change for all resources except those marked as EXECUTE.
 * 
 * @note From server side, empty resource == undefined resource.
 */

class Instance: public InstSubject {
public:
	Instance(lwm2m_context_t &context, const OBJ_LINK_T &id): _context(context), _id(id) {}
	virtual ~Instance() {}

	Instance(const Instance&) = delete;
	Instance(Instance&&) = delete;
	Instance& operator=(const Instance&) = delete;
	Instance& operator=(Instance&&) = delete;

	/* ------------- User operation methods ------------- */
	OBJ_LINK_T getLink() const { return _id; }
	OBJ_ID getObjectID() const { return (OBJ_ID)_id.objId; }
	ID_T getInstanceID() const { return _id.objInstId; }
	/**
 	 * @brief Sets resource value
	 */
	template<typename T>
	bool set(ID_T resId, const T &value);
	template<typename T>
	bool set(const ResLink &resId, const T &value);
	/**
 	 * @brief Sets resource value by moving user data to resource to avoid extra copy
	 */
	template<typename T>
	bool setMove(ID_T resId, const T &value);
	template<typename T>
	bool setMove(const ResLink &resId, const T &value);
	/**
 	 * @brief Returns copy of resource value
	 */
	template<typename T>
	bool get(ID_T resId, T &value);
	template<typename T>
	bool get(const ResLink &resId, T &value);
	/**
 	 * @brief Returns const ptr to resource data for avoid extra copy
	 */
	template<typename T>
	bool getPtr(ID_T resId, const T **value);
	template<typename T>
	bool getPtr(const ResLink &resId, const T **value);
	/**
 	 * @brief It is quite dangerous to leave a resource without instances,
	 * because when the server tries to read its value, the server
	 * will not receive anything if the resource is SINGLE, but if
	 * the resource is MANDATORY, then it will generally lead to
	 * reading error. After the cleanup operation, the user must
	 * set the value of the resource.
	 */
	bool clear(ID_T resId);
	/**
 	 * @brief Remove resource instance if resource is multiple and instance exists,
	 * if the resource is SINGLE or it has the last instance remove is not
	 * possible. Because instantiated resources must have at least one instance.
	 */
	bool remove(const ResLink &resId);

	/* ------------- Server operation methods ------------- */
	/**
 	 * @brief This methods is called by the core when the server wants to read,
	 * write, discover, execute the value of the resource. User should not
	 * call this methods directly.
	 */
	uint8_t readAsServer(int * numDataP, lwm2m_data_t ** dataArray);
	uint8_t writeAsServer(int numData, lwm2m_data_t * dataArray, lwm2m_write_type_t writeType);
	uint8_t executeAsServer(ID_T resId, uint8_t * buffer, int length);
	uint8_t discoverAsServer(int * numDataP, lwm2m_data_t ** dataArray);
	#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	uint8_t blockWriteAsServer(lwm2m_uri_t * uri, lwm2m_media_type_t format, uint8_t * buffer, int length, uint32_t blockNum, uint8_t blockMore);
	uint8_t blockExecuteAsServer(lwm2m_uri_t * uri, uint8_t * buffer, int length, uint32_t blockNum, uint8_t blockMore);
	#endif

protected: /* Interface that can be used by derived class */
	/**
 	 * @brief Notify server about resource value change.
	 */
	void notifyServerResChanged(const ResLink &resId);
	/**
 	 * @brief This method return list with resources that has been instantiated.
	 * If resources does not exist then return empty list.
	 */
	std::vector<Resource *> getInstantiatedResList();
	std::vector<Resource *> getInstantiatedResList(const ResOp& filter);
	/**
 	 * @brief This method return iterator for resource if it exists.
	 * If resources does not exist then return empty list.
	 */
	std::vector<Resource>::iterator resource(ID_T resId);

protected: /* Interface that must be implemented by derived class */
	/**
 	 * @brief This method must be implemented by derived class.
	 * Reset all resources values and internal state to default.
	 */
	virtual void setDefaultState() = 0;
	/**
 	 * @brief This method must be implemented by the derived class, and handle
	 * information about resource operation (READ, WRITE_UPD, WRITE_REPLACE_INST,
	 * WRITE_REPLACE_RES, EXECUTE, DISCOVER). Called by Instance after 
	 * resource operation performed by SERVER if the operation is  
	 * READ/WRITE_UPD/WRITE_REPLACE_INST/WRITE_REPLACE_RES/DISCOVER, 
	 * if the operation is EXECUTE then called before this operation.
	 */
	virtual void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) = 0;
	/**
 	 * @brief This method must be implemented by the derived class, and handle
     * information about resource operation (READ, WRITE_UPD, DELETE).
	 * Called by Instance after resource operation performed by the USER.
	 */
	virtual void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) = 0;
	#ifdef LWM2M_RAW_BLOCK1_REQUESTS
	/**
 	 * @brief This method must be implemented by the derived class, and handle
	 * information about resource block operation (BLOCK_WRITE, BLOCK_EXECUTE).
	 * During block operation resource value is not changed, instead user
	 * dirrectly handle block data. Also, the EXECUTE_T resource is not
	 * called, all information and data about the block operation is
	 * transferred through this method to the final implementation of the
	 * Instance class, which decides on the necessary actions. This is done 
	 * to minimize memory usage.
	 */
	virtual void serverBlockOperationNotifier(ResOp::TYPE type, const ResLink &resId, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock);
	#endif

private: /* Interface used by Object or Instance class */
	/* ------------- Compatibility with core data structure ------------- */
	/**
 	 * @brief This methods can be used for convert resource to lwm2m_data_t
	 * structure representation, or fill resource with lwm2m_data_t data.
	 */
	bool resourceToLwm2mData(Resource &res, ID_T instanceId, lwm2m_data_t &data);
	bool lwm2mDataToResource(const lwm2m_data_t &data, Resource &res, ID_T instanceId);
	/* ------------- Helpful methods for server callbacks ------------- */
	Resource* getValidatedResForWrite(const lwm2m_data_t &data, lwm2m_write_type_t writeType, uint8_t &errCode);
	uint8_t resourceWrite(Resource &res, const lwm2m_data_t &data, lwm2m_write_type_t writeType);
	Resource* getValidatedResForRead(const lwm2m_data_t &data, uint8_t &errCode);
	uint8_t resourceRead(lwm2m_data_t &data, Resource &res);
	Resource* getValidatedResForExecute(ID_T resId, uint8_t &errCode);
	uint8_t createEmptyLwm2mDataArray(std::vector<Resource*> resources, lwm2m_data_t **dataArray, int *numData);

protected:
	lwm2m_context_t &_context;
	OBJ_LINK_T _id;

	std::vector<Resource> _resources;
};

/* ---------- Implementation of template methods ----------*/
/**
 * @brief Sets resource value
 */
template<typename T>
bool Instance::set(ID_T resId, const T &value) { 
	return set({resId, SINGLE_INSTANCE_ID}, value); 
}

template<typename T>
bool Instance::set(const ResLink &resId, const T &value)  {
	auto res = resource(resId.resId);
	if (res == _resources.end()) return false;

	if (!res->set(value, resId.resInstId)) return false;

	const ResLink &link = res->isMultiple()? resId : ResLink {resId.resId,};
	userOperationNotifier(ResOp::WRITE_UPD, link);
	notifyServerResChanged(link);

	return true;
}

/**
 * @brief Sets resource value by moving user data to resource to avoid extra copy
 */
template<typename T>
bool Instance::setMove(ID_T resId, const T &value) {
	return setMove({resId, SINGLE_INSTANCE_ID}, value);
}

template<typename T>
bool Instance::setMove(const ResLink &resId, const T &value) {
	auto res = resource(resId.resId);
	if (res == _resources.end()) return false;
	if (!res->setMove(value, resId.resInstId)) return false;

	const ResLink &link = res->isMultiple()? resId : ResLink {resId.resId,};
	userOperationNotifier(ResOp::WRITE_UPD, link);
	notifyServerResChanged(link);

	return true;
}

/**
 * @brief Returns copy of resource value
 */
template<typename T>
bool Instance::get(ID_T resId, T &value) {
	return get({resId, SINGLE_INSTANCE_ID}, value);
}

template<typename T>
bool Instance::get(const ResLink &resId, T &value) {
	auto res = resource(resId.resId);
	if (res == _resources.end()) return false;

	if (!res->get(value, resId.resInstId)) return false;
	
	if (res->isMultiple()) userOperationNotifier(ResOp::READ, resId);
	else userOperationNotifier(ResOp::READ, {resId,});

	return true;
}

/**
 * @brief Returns const ptr to resource data for avoid extra copy
 */
template<typename T>
bool Instance::getPtr(ID_T resId, const T **value) {
	return getPtr({resId, SINGLE_INSTANCE_ID}, value);
}

template<typename T>
bool Instance::getPtr(const ResLink &resId, const T **value) {
	auto res = resource(resId.resId);
	if (res == _resources.end()) return false;

	T *tmpValue = NULL;
	if (!res->ptr(&tmpValue, resId.resInstId) || !tmpValue) return false;
	*value = tmpValue;

	if (res->isMultiple()) userOperationNotifier(ResOp::READ, resId);
	else userOperationNotifier(ResOp::READ, {resId,});

	return true;
}

} /* namespace wpp */

#endif /* WPP_INSTANCE_H_ */
