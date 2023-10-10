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
#include "Instance.h"
#include "InstSubject.h"

namespace wpp {

class Server : public Instance, public InstSubject<Server> {
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
	Server(lwm2m_context_t &context, const OBJ_LINK_T &id);

protected:
	/* ---------------Instance implementation part --------------- */
	/*
	 * Returns Resource object if it is exist
	 */
	Resource * getResource(ID_T id) override;
	/*
	 * Returns list with available resources
	 */
	std::vector<Resource *> getResourcesList() override;
	std::vector<Resource *> getResourcesList(const ResOp& filter) override;
	/*
	 * Returns list with available instantiated resources
	 */
	std::vector<Resource *> getInstantiatedResourcesList() override;
	std::vector<Resource *> getInstantiatedResourcesList(const ResOp& filter) override;
	/*
	 * Reset all resources values and internal state to default.
	 */
	void setDefaultState() override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;

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
    	{SHORT_SERV_ID, 			  {SHORT_SERV_ID, 				ResOp(ResOp::READ),                  IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{LIFETIME, 					  {LIFETIME, 					ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{DISABLE,       			  {DISABLE,       				ResOp(ResOp::EXECUTE),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE}},
		{NOTIFICATION_STORING, 		  {NOTIFICATION_STORING, 		ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL}},
		{BUINDING,      			  {BUINDING,      				ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
		{REGISTRATION_UPDATE_TRIGGER, {REGISTRATION_UPDATE_TRIGGER, ResOp(ResOp::EXECUTE), 				 IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE}},
		{APN_LINK,      		      {APN_LINK,      				ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::OBJ_LINK}},
    	{TRIGGER,       			  {TRIGGER,      				ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL}},
    };
};

} /* namespace wpp */

#endif /* WPP_SERVER_H_ */
