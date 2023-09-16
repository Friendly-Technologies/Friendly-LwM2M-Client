/*
 * Server.h
 *
 *  Created on: 7 Jul 2023
 *      Author: valentin
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "IInstance.h"
#include "ObjectInfo.h"
#include "InstSubject.h"

namespace wpp {

class Server : public IInstance, public InstSubject<Server> {
public:
	enum ID: ID_T {
		SHORT_SERV_ID = 0,
		LIFETIME = 1,
		DISABLE = 4,
		NOTIFICATION_STORING = 6,
		BUINDING = 7,
		REGISTRATION_UPDATE_TRIGGER = 8,
		APN_LINK = 10,
		TRIGGER = 21,
	};

public:
	Server(OBJ_ID objID, ID_T instanceID);

	/* --------------- User helpful methods for manage resources data --------------- */
	/*
	 * Sets resource value
	 */
	template<typename T>
	bool set(const T &value, ID_T resId, ID_T resInstId = 0);

	/*
	 * Returns copy of resource value
	 */
	template<typename T>
	bool get(T &value, ID_T resId, ID_T resInstId = 0);

	/*
	 * Removes all instances from resource
	 */
	bool clear(ID_T resId);

	/*
	 * Removes resource instance with specified ID from MULTIPLE resource
	 */
	bool remove(ID_T resId, ID_T resInstId);

	/*
	 * Handles information about resource operation that made user
	 */
	void userPerformedOperation(Operation::TYPE type, ID_T resId, ID_T resInstId = 0);

protected:
	/* ---------------IInstance implementation part --------------- */
	/*
	 * Returns Resource object if it is exist
	 */
	Resource * getResource(ID_T id) override;
	/*
	 * Returns list with available resources
	 */
	std::vector<Resource *> getResourcesList() override;
	std::vector<Resource *> getResourcesList(const Operation& filter) override;
	/*
	 * Returns list with available instantiated resources
	 */
	std::vector<Resource *> getInstantiatedResourcesList() override;
	std::vector<Resource *> getInstantiatedResourcesList(const Operation& filter) override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Operation::TYPE type, const ResourceID &resId) override;

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Initialize resources with defoult values
	 */
	void resourcesInit();


private:
    std::unordered_map<ID_T, Resource> _resources = {
    	//  KEY            				 VALUE
    	{SHORT_SERV_ID, 			  {SHORT_SERV_ID, 				Operation(Operation::READ),                  IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{LIFETIME, 					  {LIFETIME, 					Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{DISABLE,       			  {DISABLE,       				Operation(Operation::EXECUTE),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE}},
		{NOTIFICATION_STORING, 		  {NOTIFICATION_STORING, 		Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL}},
		{BUINDING,      			  {BUINDING,      				Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
		{REGISTRATION_UPDATE_TRIGGER, {REGISTRATION_UPDATE_TRIGGER, Operation(Operation::EXECUTE), 				 IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE}},
		{APN_LINK,      		      {APN_LINK,      				Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::OBJ_LINK}},
    	{TRIGGER,       			  {TRIGGER,      				Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL}},
    };
};


/* ---------- Implementation of template methods ----------*/
/*
 * Sets resource value
 */
template<typename T>
bool Server::set(const T &value, ID_T resId, ID_T resInstId) {
	Resource *const resource = getResource(resId);
	if (!resource) return false;

	bool result = resource->set(value, resInstId);
	if (result) {
		//TODO: Call lwm2m_resource_value_changed
		userPerformedOperation(Operation::WRITE, resId, resInstId);
	}

	return result;
}

/*
 * Returns copy of resource value
 */
template<typename T>
bool Server::get(T &value, ID_T resId, ID_T resInstId) {
	Resource *const resource = getResource(resId);
	if (!resource) return false;

	bool result = resource->get(value, resInstId);
	if (result) {
		userPerformedOperation(Operation::READ, resId, resInstId);
	}

	return result;
}

} /* namespace wpp */

#endif /* SERVER_H_ */
