#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>
#include "WppRegistry.h"
#include "IObjObserver.h"
#include "IInstObserver.h"

using namespace wpp;
using namespace std;

class DeviceImpl: public IObjObserver<Device>, public IInstObserver<Device> {
	public:
    void init(Object<Device> &deviceObj) {
        deviceObj.subscribe(this);
        wpp::Device *device = deviceObj.createInstance();
        device->subscribe(this);
    }

	void objectRestore(Object<Device> &object) override {
		cout << "Device: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object<Device> &object, ID_T instanceId) override {
        cout << "Device: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object<Device> &object, ID_T instanceId) override {
		cout << "Device: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	virtual void resourceRead(Device &inst, const ResourceID &resId) override {
        cout << "Device: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceWrite(Device &inst, const ResourceID &resId) override {
        cout << "Device: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceExecute(Device &inst, const ResourceID &resId) override {
        cout << "Device: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }
};

#endif // DEVICE_H_