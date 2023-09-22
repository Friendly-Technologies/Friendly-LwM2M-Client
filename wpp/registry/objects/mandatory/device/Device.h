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
#include "IWppInstance.h"
#include "WppInstSubject.h"

namespace wpp {

class Device: public IWppInstance, public WppInstSubject<Device> {
public:
	enum ID: ID_T {
		REBOOT = 4,
		ERROR_CODE = 11,
		SUPPORTED_BINDINGS = 16,
	};

public:
	Device(WppClient &client, const InstanceID &id);

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
    	{REBOOT, 			  {REBOOT, 				WppOperation(WppOperation::EXECUTE),  IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE}},
		{ERROR_CODE, 		  {ERROR_CODE, 			WppOperation(WppOperation::READ), 	IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT}},
		{SUPPORTED_BINDINGS,  {SUPPORTED_BINDINGS,  WppOperation(WppOperation::READ),     IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING}},
	};
};

} /* namespace wpp */

#endif /* WPP_DEVICE_H_ */
