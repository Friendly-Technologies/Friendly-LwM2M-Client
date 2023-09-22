/*
 * Server.h
 *
 *  Created on: 7 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_SERVER_H_
#define WPP_SERVER_H_

#include "ServerConfig.h"
#include "ServerInfo.h"
#include "IWppInstance.h"
#include "WppInstSubject.h"

namespace wpp {

class Server : public IWppInstance, public WppInstSubject<Server> {
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
	Server(WppClient &client, const InstanceID &id);

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
    	{SHORT_SERV_ID, 			  {SHORT_SERV_ID, 				WppOperation(WppOperation::READ),                  IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{LIFETIME, 					  {LIFETIME, 					WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{DISABLE,       			  {DISABLE,       				WppOperation(WppOperation::EXECUTE),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE}},
		{NOTIFICATION_STORING, 		  {NOTIFICATION_STORING, 		WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL}},
		{BUINDING,      			  {BUINDING,      				WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
		{REGISTRATION_UPDATE_TRIGGER, {REGISTRATION_UPDATE_TRIGGER, WppOperation(WppOperation::EXECUTE), 				 IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE}},
		{APN_LINK,      		      {APN_LINK,      				WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::OBJ_LINK}},
    	{TRIGGER,       			  {TRIGGER,      				WppOperation(WppOperation::READ|WppOperation::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL}},
    };
};

} /* namespace wpp */

#endif /* WPP_SERVER_H_ */
