/*
 * Device.h
 *
 *  Created on: 24 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_DEVICE_H_
#define WPP_DEVICE_H_

#include "DeviceConfig.h"
#include "DeviceInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class Device: public Instance, public InstSubject<Device> {
public:
	enum ID: ID_T {
		MANUFACTURE = 0,
		MODEL_NUM = 1,
		SERIAL_NUM = 2,
		FW_VERSION = 3,
		REBOOT = 4,
		BATT_LVL = 9,
		ERROR_CODE = 11,
		MEM_FREE = 10,
		CURR_TIME = 13,
		UTC_OFFSET = 14,
		TIMEZONE = 15,
		SUPPORTED_BINDINGS = 16,
	};

	/* --------------- Сode_h block 1 start --------------- */
	/* --------------- Сode_h block 1 end --------------- */

public:
	Device(WppClient &client, const OBJ_LINK_T &id);
	~Device();

	/* --------------- Сode_h block 2 start --------------- */
	/* --------------- Сode_h block 2 end --------------- */

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

	/* --------------- Сode_h block 3 start --------------- */
	/* --------------- Сode_h block 3 end --------------- */
	
private:
    std::unordered_map<ID_T, Resource> _resources = {
    	//  KEY            				 VALUE
		{MANUFACTURE, 		  {MANUFACTURE, 		ResOp(ResOp::READ), 			 IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::STRING}},
		{MODEL_NUM, 		  {MODEL_NUM, 			ResOp(ResOp::READ), 			 IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::STRING}},
		{SERIAL_NUM, 		  {SERIAL_NUM, 			ResOp(ResOp::READ), 			 IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::STRING}},
		{FW_VERSION, 		  {FW_VERSION, 			ResOp(ResOp::READ), 			 IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::STRING}},
    	{REBOOT, 			  {REBOOT, 				ResOp(ResOp::EXECUTE),  		 IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE}},
		{BATT_LVL, 		  	  {BATT_LVL, 			ResOp(ResOp::READ), 			 IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::INT}},
		{ERROR_CODE, 		  {ERROR_CODE, 			ResOp(ResOp::READ), 			 IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{MEM_FREE, 		  	  {MEM_FREE, 			ResOp(ResOp::READ), 			 IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::INT}},
		{CURR_TIME, 		  {CURR_TIME, 			ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::TIME}},
		{UTC_OFFSET, 		  {UTC_OFFSET, 			ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::STRING}},
		{TIMEZONE, 		  	  {TIMEZONE, 			ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL, TYPE_ID::STRING}},
		{SUPPORTED_BINDINGS,  {SUPPORTED_BINDINGS,  ResOp(ResOp::READ),     		 IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
	};

	/* --------------- Сode_h block 4 start --------------- */
	/* --------------- Сode_h block 4 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_DEVICE_H_ */
