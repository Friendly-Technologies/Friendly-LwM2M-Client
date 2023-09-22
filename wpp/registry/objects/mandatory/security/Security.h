/*
 * Security.h
 *
 *  Created on: 24 Jul 2023
 *      Author: valentin
 */

#ifndef SECURITY_H_
#define SECURITY_H_

#include "SecurityConfig.h"
#include "SecurityInfo.h"
#include "IInstance.h"
#include "InstSubject.h"

namespace wpp {

class Security: public IInstance, public InstSubject<Security> {
public:
	enum ID: ID_T {
		SERVER_URI = 0,
		BOOTSTRAP_SERVER = 1,
		SECURITY_MODE = 2,
		PUBLIC_KEY = 3,
		SERVER_PUBLIC_KEY = 4,
		SECRET_KEY = 5,
	};

public:
	Security(WppClient &client, const InstanceID &id);
	
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
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(Operation::TYPE type, const ResourceID &resId) override;

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Initialize resources with default values.
	 * Resource always must have at least one instance.
	 */
	void resourcesInit();

private:
    std::unordered_map<ID_T, Resource> _resources = {
    	//  KEY            				 VALUE
    	{SERVER_URI, 		{SERVER_URI, 		Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
		{BOOTSTRAP_SERVER, 	{BOOTSTRAP_SERVER, 	Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL}},
		{SECURITY_MODE,     {SECURITY_MODE,     Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{PUBLIC_KEY, 		{PUBLIC_KEY, 		Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
		{SERVER_PUBLIC_KEY, {SERVER_PUBLIC_KEY, Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
		{SECRET_KEY, 		{SECRET_KEY, 		Operation(Operation::READ|Operation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
	};
};

} /* namespace wpp */

#endif /* SECURITY_H_ */
