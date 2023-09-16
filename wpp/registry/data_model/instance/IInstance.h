/*
 *IInstance.h
 *
 *  Created on: 6 Jul 2023
 *      Author: valentin
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <vector>

//TODO: #include "liblwm2m.h"
#include "Resource.h"
#include "Operation.h"
#include "types.h"

namespace wpp {

/*
 * IInstance is interface class that implements manipulation with derived class resources.
 * The main target of this class is to encapsulate operations like resource write and read by core, for avoid
 * multiple definition of this mechanism in instance implementation classes. And give generalised
 * interface to resources for core(server).
 *
 * Note: It should be noted that the interface of the instance responsible for communication
 * with the client is not specified in any way and remains at the discretion of the developer,
 * whether it will be setters and getters or more abstract methods with algorithm encapsulation,
 * it is not important. It is important that the developer is responsible for notifying the
 * core about resource changes, if the value of any of the resources changes, then the developer
 * must immediately call the function lwm2m_resource_value_changed() or the one that encapsulates
 * this call. It is necessary to notify about the change for all resources except those marked as EXECUTE.
 */
// TODO: Add user notification ability about resource operations WRITE, READ, EXECUTE
class IInstance {
	template<typename T>
	friend class Object;

public: /* Interface that can be used by user */
	IInstance(OBJ_ID objID, ID_T instanceID): _objID(objID), _instanceID(instanceID) {}
	virtual ~IInstance() {}

	IInstance(const IInstance&) = delete;
	IInstance(IInstance&&) = delete;
	IInstance& operator=(const IInstance&) = delete;
	IInstance& operator=(IInstance&&) = delete;

	OBJ_ID getObjectID() { return _objID;}
	ID_T getInstanceID() { return _instanceID;}

protected: /* Interface used by IInstance implementation class */
	/*
	 * This method must be implemented by derived class,
	 * and return resource if it is exists.
	 * If resource does not exist then return NULL.
	 */
	virtual Resource * getResource(ID_T resourceId) = 0;

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
	 * Called byIInstance after resource operation performed by server.
	 */
	virtual void serverOperationNotifier(Operation::TYPE type, const ResourceID &resourceId) = 0;

private: /* Interface used by Object<T> or IInstance class */
	/*
	 * This methods can be used for convert resource to lwm2m_data_t
	 * structure representation, or fill resource with lwm2m_data_t data.
	 */
	bool resourceToLwm2mData(Resource &resource, ID_T instanceId, lwm2m_data_t &data);
	bool lwm2mDataToResource(const lwm2m_data_t &data, Resource &resource, ID_T instanceId);

	/* ------------- Server callback ------------- */
	uint8_t resourceRead(ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP);
	uint8_t resourceWrite(ID_T instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_write_type_t writeType);
	uint8_t resourceExecute(ID_T instanceId, ID_T resourceId, uint8_t * buffer, int length);
	uint8_t resourceDiscover(ID_T instanceId, int * numDataP, lwm2m_data_t ** dataArrayP);

protected:
	OBJ_ID _objID;
	ID_T _instanceID;
};

} /* namespace wpp */

#endif /* INSTANCE_H_ */
