#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>
#include "WppRegistry.h"
#include "IWppObjObserver.h"
#include "IWppInstObserver.h"

using namespace wpp;
using namespace std;

class DeviceImpl: public IWppObjObserver<Device>, public IWppInstObserver<Device> {
	public:
    void init(WppObject<Device> &deviceObj) {
        deviceObj.subscribe(this);
        wpp::Device *device = deviceObj.createInstance();
        device->subscribe(this);

        device->set(Device::REBOOT, (EXECUTE_T)[](ID_T id, const OPAQUE_T& data) {
            cout << "Device: execute REBOOT" << endl;
        });
        device->set(Device::ERROR_CODE, (INT_T)0);
        device->set(Device::SUPPORTED_BINDINGS, (STRING_T)"U");
    }

	void objectRestore(WppObject<Device> &object) override {
		cout << "Device: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(WppObject<Device> &object, ID_T instanceId) override {
        cout << "Device: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(WppObject<Device> &object, ID_T instanceId) override {
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