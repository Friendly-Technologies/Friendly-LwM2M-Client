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
#include "ResourceContainer.h"

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
 * of any of the resources changes, then the developer MUST immediately call the method 
 * Instance::notifyResChanged(). It is necessary to notify about the change for all resources
 * except those marked as EXECUTE.
 */
class Instance: public InstSubject, public ResourceContainer {
public:
	Instance(lwm2m_context_t &context, const OBJ_LINK_T &id): _context(context), _id(id) {}
	virtual ~Instance() {}

	Instance(const Instance&) = delete;
	Instance(Instance&&) = delete;
	Instance& operator=(const Instance&) = delete;
	Instance& operator=(Instance&&) = delete;

	/* ------------- User helpful methods ------------- */
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
	void notifyResChanged(ID_T resId, ID_T resInstId = ID_T_MAX_VAL);

	/**
 	 * @brief This method return list with resources that has been instantiated.
	 * If resources does not exist then return empty list.
	 */
	std::vector<Resource *> getInstantiatedResList();
	std::vector<Resource *> getInstantiatedResList(const ItemOp& filter);

	/**
 	 * @brief This method return list with all resources that has been defined.
	 * If resources does not exist then return empty list.
	 */
	std::vector<Resource *> getResList();

	/**
 	 * @brief Handle information about resource operation (WRITE, DELETE).
	 * Called by ResourceContainer after resource operation performed.
	 */
	void resourceOperationNotifier(ItemOp::TYPE type, ID_T resId, ID_T resInstId) override;


protected: /* Interface that must be implemented by derived class */
	/**
 	 * @brief This method must be implemented by the derived class, and handle
	 * information about resource operation (WRITE, EXECUTE). 
	 * Called by Instance after resource operation performed by SERVER if the operation is  
	 * WRITE, if the operation is EXECUTE then called before this operation.
	 * When the EXECUTE operation, the handler that was set before the serverOperationNotifier()
	 * call is used.
	 * @param securityInst - Contains security instance when the request received
	 * 						 from the server or NULL if the request is initiated by the core.
	 */
	virtual void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) = 0;

	/**
 	 * @brief This method must be implemented by the derived class, and handle
     * information about resource operation (WRITE, DELETE).
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

private:
	lwm2m_context_t &_context;
	OBJ_LINK_T _id;
};

} /* namespace wpp */

#endif /* WPP_INSTANCE_H_ */
