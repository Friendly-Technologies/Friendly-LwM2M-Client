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
#include "Instance.h"
#include "InstSubject.h"

namespace wpp {

class Security: public Instance, public InstSubject<Security> {
public:
	enum ID: ID_T {
		SERVER_URI = 0,
		BOOTSTRAP_SERVER = 1,
		SECURITY_MODE = 2,
		PUBLIC_KEY = 3,
		SERVER_PUBLIC_KEY = 4,
		SECRET_KEY = 5,
		SERVER_ID = 10,
		HOLD_OFF_TIME = 11,
	};

public:
	Security(lwm2m_context_t &context, const OBJ_LINK_T &id);
	
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
	 * Resources always must have at least one instance.
	 */
	void resourcesInit();

private:
    std::unordered_map<ID_T, Resource> _resources = {
    	//  KEY            				 VALUE
    	{SERVER_URI, 		{SERVER_URI, 		ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
		{BOOTSTRAP_SERVER, 	{BOOTSTRAP_SERVER, 	ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL}},
		{SECURITY_MODE,     {SECURITY_MODE,     ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{PUBLIC_KEY, 		{PUBLIC_KEY, 		ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
		{SERVER_PUBLIC_KEY, {SERVER_PUBLIC_KEY, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
		{SECRET_KEY, 		{SECRET_KEY, 		ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE}},
		{SERVER_ID, 		{SERVER_ID, 		ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT}},
		{HOLD_OFF_TIME, 	{HOLD_OFF_TIME, 	ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT}},
	};
};

} /* namespace wpp */

#endif /* WPP_SECURITY_H_ */
