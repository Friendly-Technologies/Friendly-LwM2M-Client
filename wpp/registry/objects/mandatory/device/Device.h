/*
 * Device.h
 *
 *  Created on: 24 Jul 2023
 *      Author: valentin
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "IInstance.h"
#include "ObjectInfo.h"
#include "InstSubject.h"

namespace wpp {

class Device: public IInstance, public InstSubject<Device> {
public:
	Device(OBJ_ID objID, ID_T instanceID);

protected:
	/* ---------------IInstance implementation part --------------- */
	/*
	 * Returns Resource object if it is exist
	 */
	Resource * getResource(ID_T id) override;
	/*
	 * Returns return list with available resources
	 */
	std::vector<Resource *> getResourcesList() override;
	std::vector<Resource *> getResourcesList(const Operation& filter) override;
	/*
	 * Returns return list with available instantiated resources
	 */
	std::vector<Resource *> getInstantiatedResourcesList() override;
	std::vector<Resource *> getInstantiatedResourcesList(const Operation& filter) override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Operation::TYPE type, const ResourceID &resourceId) override;

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Initialize resources with defoult values
	 */
	void resourcesInit();
};

} /* namespace wpp */

#endif /* DEVICE_H_ */
