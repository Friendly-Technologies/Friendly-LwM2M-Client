/*
 * Security.h
 *
 *  Created on: 24 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_SECURITY_H_
#define WPP_SECURITY_H_

#include "SecurityConfig.h"
#include "SecurityInfo.h"
#include "IWppInstance.h"
#include "WppInstSubject.h"

namespace wpp {

class Security: public IWppInstance, public WppInstSubject<Security> {
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
	/* ---------------IWppInstance implementation part --------------- */
	/*
	 * Returns WppResource object if it is exist
	 */
	WppResource * getResource(ID_T id) override;
	/*
	 * Returns list with available resources
	 */
	std::vector<WppResource *> getResourcesList() override;
	std::vector<WppResource *> getResourcesList(const WppOperation& filter) override;
	/*
	 * Returns list with available instantiated resources
	 */
	std::vector<WppResource *> getInstantiatedResourcesList() override;
	std::vector<WppResource *> getInstantiatedResourcesList(const WppOperation& filter) override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(WppOperation::TYPE type, const ResourceID &resId) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(WppOperation::TYPE type, const ResourceID &resId) override;

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Initialize resources with default values.
	 * WppResource always must have at least one instance.
	 */
	void resourcesInit();

private:
    std::unordered_map<ID_T, WppResource> _resources = {
    	//  KEY            				 VALUE
    	{SERVER_URI, 		{SERVER_URI, 		WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
		{BOOTSTRAP_SERVER, 	{BOOTSTRAP_SERVER, 	WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL}},
		{SECURITY_MODE,     {SECURITY_MODE,     WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{PUBLIC_KEY, 		{PUBLIC_KEY, 		WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
		{SERVER_PUBLIC_KEY, {SERVER_PUBLIC_KEY, WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
		{SECRET_KEY, 		{SECRET_KEY, 		WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
	};
};

} /* namespace wpp */

#endif /* WPP_SECURITY_H_ */
