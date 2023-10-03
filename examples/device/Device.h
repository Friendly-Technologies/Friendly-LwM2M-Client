#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class DeviceImpl: public ObjObserver<Device>, public InstObserver<Device> {
	public:
    void init(Object<Device> &deviceObj) {
        deviceObj.subscribe(this);
        wpp::Device *device = deviceObj.createInstance();
        device->subscribe(this);

        device->set(Device::REBOOT, (EXECUTE_T)[](ID_T id, const OPAQUE_T& data) {
            cout << "Device: execute REBOOT" << endl;
        });
        device->set(Device::ERROR_CODE, (INT_T)0);
        device->set(Device::SUPPORTED_BINDINGS, (STRING_T)"U");
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

	void resourceRead(Device &inst, const ResLink &resId) override {
        cout << "Device: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(Device &inst, const ResLink &resId) override {
        cout << "Device: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(Device &inst, const ResLink &resId) override {
        cout << "Device: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(Device &inst) override {
        cout << "Device: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }
};

#endif // DEVICE_H_