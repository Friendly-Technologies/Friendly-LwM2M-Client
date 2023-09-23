/*
 *IWppInstance.h
 *
 *  Created on: 6 Jul 2023
 *      Author: valentin
 */

#ifndef I_WPP_INSTANCE_H_
#define I_WPP_INSTANCE_H_

#include <vector>

#include "WppClient.h"
#include "Resource.h"
#include "Operation.h"
#include "types.h"

namespace wpp {

class WppClient;

/*
 * IWppInstance is interface class that implements manipulation with derived class resources.
 * The main target of this class is to encapsulate operations like resource write and read by core, for avoid
 * multiple definition of this mechanism in instance implementation classes. And give generalised
 * interface to resources for core(server).
 *
 * Note: It should be noted that the interface of the instance responsible for communication
 * with the client is specified through get()/set() methods, but the developer can define his
 * interface at the IWppInstance implementation level, whether it will be setters and getters or
 * more abstract methods with algorithm encapsulation, it is not important. It is important
 * that the developer is responsible for notifying the core about resource changes, if the value
 * of any of the resources changes bypassing the IWppInstance::get()/set() methods, then the developer
 * must immediately call the method WppClient::notifyValueChanged() or the one that encapsulates
 * this call. It is necessary to notify about the change for all resources except those marked as EXECUTE.
 */
class IWppInstance {
	template<typename T>
	friend class WppObject;

public: /* Interface that can be used by user */
	IWppInstance(WppClient &client, const InstanceID &id): _client(client), _id(id) {}
	virtual ~IWppInstance() {}

	IWppInstance(const IWppInstance&) = delete;
	IWppInstance(IWppInstance&&) = delete;
	IWppInstance& operator=(const IWppInstance&) = delete;
	IWppInstance& operator=(IWppInstance&&) = delete;

	OBJ_ID getObjectID() { return (OBJ_ID)_id.objId;}
	ID_T getInstanceID() { return _id.objInstId;}

	/*
	 * Sets resource value
	 */
	template<typename T>
	bool set(ID_T resId, const T &value) { return userSet({resId, SINGLE_INSTANCE_ID}, value); }
	template<typename T>
	bool set(const ResourceID &resId, const T &value)  { return userSet({resId.resId, resId.resInstId}, value); }
	/*
	 * Returns copy of resource value
	 */
	template<typename T>
	bool get(ID_T resId, T &value) { return userGet({resId, SINGLE_INSTANCE_ID}, value); }
	template<typename T>
	bool get(const ResourceID &resId, T &value) { return userGet({resId.resId, resId.resInstId}, value); }
	/*
	 * It is quite dangerous to leave a resource without instances,
	 * because when the server tries to read its value, the server
	 * will not receive anything if the resource is SINGLE, but if
	 * the resource is MANDATORY, then it will generally lead to
	 * reading error. After the cleanup operation, the user must
	 * set the value of the resource.
	 */
	bool clear(ID_T resId);
	/*
	 * Remove resource instance if resource is multiple and instance exists,
	 * if the resource is SINGLE or it has the last instance remove is not
	 * possible. Because instantiated resources must have at least one instance.
	 */
	bool remove(const ResourceID &resId);

protected: /* Interface that can be used by derived class */
	WppClient& client();

protected: /* Interface implemented by IWppInstance derived class */
	/*
	 * This method must be implemented by derived class,
	 * and return resource if it is exists.
	 * If resource does not exist then return NULL.
	 */
	virtual Resource * getResource(ID_T resId) = 0;
	/*
	 * This method must be implemented by derived class,
	 * and return list with all resources.
	 * If resources does not exist then return empty list.
	 */
	virtual std::vector<Resource *> getResourcesList() = 0;
	virtual std::vector<Resource *> getResourcesList(const Operation& filter) = 0;
	/*
	 * This method must be implemented by derived class,
	 * and return list with resources that has been instantiated.
	 * If resources does not exist then return empty list.
	 */
	virtual std::vector<Resource *> getInstantiatedResourcesList() = 0;
	virtual std::vector<Resource *> getInstantiatedResourcesList(const Operation& filter) = 0;
	/*
	 * This method must be implemented by derived class, and handle
     * information about resource operation (READ, WRITE, EXECUTE, DISCOVER, DELETE).
	 * Called by IWppInstance after resource operation performed by SERVER.
	 */
	virtual void serverOperationNotifier(Operation::TYPE type, const ResourceID &resId) = 0;
	/*
	 * This method must be implemented by derived class, and handle
     * information about resource operation (READ, WRITE, DELETE).
	 * Called by IWppInstance after resource operation performed by USER.
	 */
	virtual void userOperationNotifier(Operation::TYPE type, const ResourceID &resId) = 0;

private: /* Interface used by WppObject<T> or IWppInstance class */
	/* ------------- Implementation of user set/get methods ------------- */
	template<typename T>
	bool userSet(const ResourceID &resId, const T &value);
	template<typename T>
	bool userGet(const ResourceID &resId, T &value);
	/* ------------- Compatibility with core data structure ------------- */
	/*
	 * This methods can be used for convert resource to lwm2m_data_t
	 * structure representation, or fill resource with lwm2m_data_t data.
	 */
	bool resourceToLwm2mData(Resource &resource, ID_T instanceId, lwm2m_data_t &data);
	bool lwm2mDataToResource(const lwm2m_data_t &data, Resource &resource, ID_T instanceId);
	/* ------------- Server callback ------------- */
	uint8_t resourceRead(ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP);
	uint8_t resourceWrite(ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_write_type_t writeType);
	uint8_t resourceExecute(ID_T instanceId, ID_T resId, uint8_t * buffer, int length);
	uint8_t resourceDiscover(ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP);

protected:
	WppClient &_client;
	InstanceID _id;
};

/* ---------- Implementation of template methods ----------*/
/*
 * Sets resource value
 */
template<typename T>
bool IWppInstance::userSet(const ResourceID &resId, const T &value) {
	Resource *const resource = getResource(resId.resId);
	if (!resource) return false;

	bool result = resource->set(value, resId.resInstId);
	if (result) {
		client().notifyValueChanged({_id, {resId.resId, resId.resInstId}});
		userOperationNotifier(Operation::WRITE, resId);
	}

	return result;
}

/*
 * Returns copy of resource value
 */
template<typename T>
bool IWppInstance::userGet(const ResourceID &resId, T &value) {
	Resource *const resource = getResource(resId.resId);
	if (!resource) return false;

	bool result = resource->get(value, resId.resInstId);
	if (result) {
		userOperationNotifier(Operation::READ, resId);
	}

	return result;
}

} /* namespace wpp */

#endif /* I_WPP_INSTANCE_H_ */
