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
/*
 * Instance is interface class that implements manipulation with derived class resources.
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
 * must immediately call the method WppClient::notifyValueChanged() or the one that encapsulates
 * this call. It is necessary to notify about the change for all resources except those marked as EXECUTE.
 * 
 * Note: From server side, empty resource == undefined resource.
 */
class Instance: public InstSubject {
friend class Object;

public: /* Interface that can be used by user */
	Instance(lwm2m_context_t &context, const OBJ_LINK_T &id): _context(context), _id(id) {}
	virtual ~Instance() {}

	Instance(const Instance&) = delete;
	Instance(Instance&&) = delete;
	Instance& operator=(const Instance&) = delete;
	Instance& operator=(Instance&&) = delete;

	OBJ_LINK_T getLink() const { return _id; }
	OBJ_ID getObjectID() const { return (OBJ_ID)_id.objId; }
	ID_T getInstanceID() const { return _id.objInstId; }

	/*
	 * Sets resource value
	 */
	template<typename T>
	bool set(ID_T resId, const T &value);
	template<typename T>
	bool set(const ResLink &resId, const T &value);
	/*
	 * Returns copy of resource value
	 */
	template<typename T>
	bool get(ID_T resId, T &value);
	template<typename T>
	bool get(const ResLink &resId, T &value);
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
	bool remove(const ResLink &resId);

protected: /* Interface that can be used by derived class */
	/*
	 * Notify core about resource value change.
	 */
	void notifyValueChanged(const DataLink &data);
	/*
	 * This method return list with resources that has been instantiated.
	 * If resources does not exist then return empty list.
	 */
	std::vector<Resource *> getInstantiatedResList();
	std::vector<Resource *> getInstantiatedResList(const ResOp& filter);
	/*
	 * This method return iterator for resource if it exists.
	 * If resources does not exist then return empty list.
	 */
	std::vector<Resource>::iterator resource(ID_T resId);

protected: /* Interface that must be implemented by derived class */
	/*
	 * This method must be implemented by derived class.
	 * Reset all resources values and internal state to default.
	 */
	virtual void setDefaultState() = 0;
	/*
	 * This method must be implemented by derived class, and handle
     * information about resource operation (READ, WRITE_UPD, 
	 * WRITE_REPLACE_INST, WRITE_REPLACE_RES, EXECUTE, DISCOVER).
	 * Called by Instance after resource operation performed by SERVER.
	 */
	virtual void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) = 0;
	/*
	 * This method must be implemented by derived class, and handle
     * information about resource operation (READ, WRITE_UPD, DELETE).
	 * Called by Instance after resource operation performed by USER.
	 */
	virtual void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) = 0;

private: /* Interface used by Object<T> or Instance class */
	/* ------------- Compatibility with core data structure ------------- */
	/*
	 * This methods can be used for convert resource to lwm2m_data_t
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
	/* ------------- Server operation methods ------------- */
	uint8_t read(int * numDataP, lwm2m_data_t ** dataArray);
	uint8_t write(int numData, lwm2m_data_t * dataArray, lwm2m_write_type_t writeType);
	uint8_t execute(ID_T resId, uint8_t * buffer, int length);
	uint8_t discover(int * numDataP, lwm2m_data_t ** dataArray);

protected:
	lwm2m_context_t &_context;
	OBJ_LINK_T _id;

	std::vector<Resource> _resources;
};

/* ---------- Implementation of template methods ----------*/
/*
 * Sets resource value
 */
template<typename T>
bool Instance::set(ID_T resId, const T &value) { 
	return set({resId, SINGLE_INSTANCE_ID}, value); 
}

template<typename T>
bool Instance::set(const ResLink &resId, const T &value)  {
	auto res = resource(resId.resId);
	if (res == _resources.end()) return false;

	bool result = res->set(value, resId.resInstId);
	if (result) {
		notifyValueChanged({_id, {resId.resId, resId.resInstId}});
		userOperationNotifier(ResOp::WRITE_UPD, resId);
	}

	return result;
}

/*
 * Returns copy of resource value
 */
template<typename T>
bool Instance::get(ID_T resId, T &value) {
	return get({resId, SINGLE_INSTANCE_ID}, value);
}

template<typename T>
bool Instance::get(const ResLink &resId, T &value) {
	auto res = resource(resId.resId);
	if (res == _resources.end()) return false;

	bool result = res->get(value, resId.resInstId);
	if (result) {
		userOperationNotifier(ResOp::READ, resId);
	}

	return result;
}

} /* namespace wpp */

#endif /* WPP_INSTANCE_H_ */
