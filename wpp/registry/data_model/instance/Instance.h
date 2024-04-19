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
#include "ItemOp.h"
#include "WppTypes.h"
#include "InstSubject.h"

namespace wpp {

class WppRegistry;
class WppClient;

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
 * TODO: Avoid of using clear and remove methods, because it is not clear what to do with resources.
 * Instead, replace the approach to act with MULTIPLE resources. Add the ability to set/get the whole
 * resource at one time.
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
	 * @brief Return context that can be used by derived class.
	 */
	lwm2m_context_t& getContext();

	/**
 	 * @brief Helpfull methods to get client instances. 
	 */
	WppClient& getClient();

	/**
	 * @brief Helpfull methods to get registry instances. 
	 */
	WppRegistry& getRegistry();

	/**
 	 * @brief Sets resource value.
	 * This version of the method is used with SINGLE resources.
	 */
	template<typename T>
	bool set(ID_T resId, const T &value);

	/**
 	 * @brief Sets resource value, this version of the method is used with MULTIPLE resources.
	 */
	template<typename T>
	bool set(ID_T resId, ID_T resInstId, const T &value);

	/**
 	 * @brief Sets resource value by moving user data to resource to avoid extra copy.
	 * This version of the method is used with SINGLE resources.
	 */
	template<typename T>
	bool setMove(ID_T resId, T &value);

	/**
 	 * @brief Sets resource value by moving user data to resource to avoid extra copy.
	 * This version of the method is used with MULTIPLE resources.
	 */
	template<typename T>
	bool setMove(ID_T resId, ID_T resInstId, T &value);

	/**
 	 * @brief Returns copy of resource value.
	 * This version of the method is used with SINGLE resources.
	 */
	template<typename T>
	T &get(ID_T resId);

	/**
 	 * @brief Returns copy of resource value.
	 * This version of the method is used with MULTIPLE resources.
	 */
	template<typename T>
	T &get(ID_T resId, ID_T resInstId);

	bool isExist(ID_T resId);
	bool isExist(ID_T resId, ID_T resInstId);

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
	bool remove(ID_T resId, ID_T resInstId);

	/* ------------- Server operation methods ------------- */
	/**
 	 * @brief This methods is called by the core when the server wants to read,
	 * write, discover, execute the value of the resource. User should not
	 * call this methods directly.
	 * @param securityInst - Contains security instance ID when request retrieved
	 * 						 from server or ID_T_MAX_VAL if request initiated by core.
	 */
	uint8_t readAsServer(lwm2m_server_t *server, int *numDataP, lwm2m_data_t **dataArray);
	uint8_t writeAsServer(lwm2m_server_t *server, int numData, lwm2m_data_t *dataArray, lwm2m_write_type_t writeType);
	uint8_t executeAsServer(lwm2m_server_t *server, ID_T resId, uint8_t *buffer, int length);
	uint8_t discoverAsServer(lwm2m_server_t *server, int * numDataP, lwm2m_data_t **dataArray);

protected: /* Interface that can be used by derived class */
	/**
 	 * @brief Notify server about resource value change.
	 */
	void notifyServerResChanged(ID_T resId, ID_T resInstId = ID_T_MAX_VAL);

	/**
 	 * @brief This method return list with resources that has been instantiated.
	 * If resources does not exist then return empty list.
	 */
	std::vector<Resource *> getInstantiatedResList();
	std::vector<Resource *> getInstantiatedResList(const ItemOp& filter);

	/**
 	 * @brief This method return iterator for resource if it exists.
	 * If resources does not exist then return empty list.
	 */
	std::vector<Resource>::iterator resource(ID_T resId);

protected: /* Interface that must be implemented by derived class */
	/**
 	 * @brief This method must be implemented by the derived class, and handle
	 * information about resource operation (READ, WRITE, EXECUTE). 
	 * Called by Instance after resource operation performed by SERVER if the operation is  
	 * READ/WRITE, if the operation is EXECUTE then called before this operation.
	 * When the EXECUTE operation, the handler that was set before the serverOperationNotifier()
	 * call is used.
	 * @param securityInst - Contains security instance when the request received
	 * 						 from the server or NULL if the request is initiated by the core.
	 */
	virtual void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) = 0;

	/**
 	 * @brief This method must be implemented by the derived class, and handle
     * information about resource operation (READ, WRITE, DELETE).
	 * Called by Instance after resource operation performed by the USER.
	 */
	virtual void userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) = 0;

private: /* Interface used by Object or Instance class */
	Instance *getSecurityInst(lwm2m_server_t *server);

	/* ------------- Compatibility with core data structure ------------- */
	/**
 	 * @brief This methods can be used for convert resource to lwm2m_data_t
	 * structure representation, or fill resource with lwm2m_data_t data.
	 */
	bool resourceToLwm2mData(Resource &res, ID_T instanceId, lwm2m_data_t &data);
	bool lwm2mDataToResource(const lwm2m_data_t &data, Resource &res, ID_T instanceId);

	/* ------------- Helpful methods for server callbacks ------------- */
	Resource* getValidatedResForWrite(const lwm2m_data_t &data, lwm2m_write_type_t writeType, uint8_t &errCode);
	uint8_t resourceWrite(lwm2m_server_t *server, Resource &res, const lwm2m_data_t &data, lwm2m_write_type_t writeType);
	Resource* getValidatedResForRead(const lwm2m_data_t &data, uint8_t &errCode);
	uint8_t resourceRead(lwm2m_server_t *server, lwm2m_data_t &data, Resource &res);
	Resource* getValidatedResForExecute(ID_T resId, uint8_t &errCode);
	uint8_t createEmptyLwm2mDataArray(std::vector<Resource*> resources, lwm2m_data_t **dataArray, int *numData);
	bool isAllMandatoryResourcesPresent(int numData, lwm2m_data_t *data);
	uint8_t replaceInstance(lwm2m_server_t *server, int numData, lwm2m_data_t *dataArray);
	uint8_t replaceResource(lwm2m_server_t *server, int numData, lwm2m_data_t *dataArray, lwm2m_write_type_t writeType);

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
	auto res = resource(resId);
	if (res == _resources.end() || res->isMultiple()) {
		WPP_LOGE(TAG_WPP_INST, "Invalid resource id or resource is multiple");
		return false;
	}
	if (!res->set<T>(value)) return false;

	userOperationNotifier(ItemOp::WRITE, {resId, ID_T_MAX_VAL});
	notifyServerResChanged(resId);

	return true; 
}

template<typename T>
bool Instance::set(ID_T resId, ID_T resInstId, const T &value)  {
	auto res = resource(resId);
	if (res == _resources.end() || res->isSingle()) {
		WPP_LOGE(TAG_WPP_INST, "Invalid resource id or resource is sinagle");
		return false;
	}
	if (!res->set<T>(value, resInstId)) return false;

	userOperationNotifier(ItemOp::WRITE, {resId, resInstId});
	notifyServerResChanged(resId, resInstId);

	return true;
}

/**
 * @brief Sets resource value by moving user data to resource to avoid extra copy
 */
template<typename T>
bool Instance::setMove(ID_T resId, T &value) {
	auto res = resource(resId);
	if (res == _resources.end() || res->isMultiple())  {
		WPP_LOGE(TAG_WPP_INST, "Invalid resource id or resource is multiple");
		return false;
	}
	if (!res->setMove<T>(value)) return false;

	userOperationNotifier(ItemOp::WRITE, {resId, ID_T_MAX_VAL});
	notifyServerResChanged(resId);
	return true;
}

template<typename T>
bool Instance::setMove(ID_T resId, ID_T resInstId, T &value) {
	auto res = resource(resId);
	if (res == _resources.end() || res->isSingle()) {
		WPP_LOGE(TAG_WPP_INST, "Invalid resource id or resource is sinagle");
		return false;
	}
	if (!res->setMove<T>(value, resInstId)) return false;

	userOperationNotifier(ItemOp::WRITE, {resId, resInstId});
	notifyServerResChanged(resId, resInstId);

	return true;
}

/**
 * @brief Returns reference to resource value
 */
template<typename T>
T &Instance::get(ID_T resId) {
	auto res = resource(resId);
	if (res == _resources.end() || !res->isDataTypeValid<T>() || res->isMultiple()) {
		// Return empty value if the data type is not valid or the instance does not exist
		static T empty;
		empty = T();
		WPP_LOGE(TAG_WPP_INST, "Invalid resource id or data type");
		return empty;
	}

	userOperationNotifier(ItemOp::READ, {resId, ID_T_MAX_VAL});
	return res->get<T>();
}

template<typename T>
T &Instance::get(ID_T resId, ID_T resInstId) {
	auto res = resource(resId);
	if (res == _resources.end() || !res->isInstanceExist(resInstId) || !res->isDataTypeValid<T>() || res->isSingle()) {
		// Return empty value if the data type is not valid or the instance does not exist
		static T empty;
		empty = T();
		WPP_LOGE(TAG_WPP_RES, "Invalid resource id or data type");
		return empty;
	}

	userOperationNotifier(ItemOp::READ, {resId, resInstId});
	return res->get<T>(resInstId);
}

} /* namespace wpp */

#endif /* WPP_INSTANCE_H_ */
