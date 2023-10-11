#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class DeviceImpl: public ObjObserver<WppDevice>, public InstObserver<WppDevice> {
	public:
    DeviceImpl(): _reboot(false) {}

    void init(Object<WppDevice> &deviceObj) {
        deviceObj.subscribe(this);
        wpp::WppDevice *device = deviceObj.createInstance();
        device->subscribe(this);

        device->set(WppDevice::REBOOT, (EXECUTE_T)[this](ID_T id, const OPAQUE_T& data) {
            cout << "WppDevice: execute REBOOT" << endl;
            this->_reboot = true;
        });
        device->set(WppDevice::ERROR_CODE_11, (INT_T)0);
        device->set(WppDevice::SUPPORTED_BINDING_AND_MODES_16, (STRING_T)"U");
        device->set(WppDevice::MANUFACTURER_0, (STRING_T)"Open Mobile Alliance");
        device->set(WppDevice::MODEL_NUMBER_1, (STRING_T)"Lightweight M2M Client");
        device->set(WppDevice::SERIAL_NUMBER_2, (STRING_T)"345000123");
    }

    bool isNeededReboot() {
        return _reboot;
    }

    private:
	void objectRestore(Object<WppDevice> &object) override {
		cout << "WppDevice: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object<WppDevice> &object, ID_T instanceId) override {
        cout << "WppDevice: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object<WppDevice> &object, ID_T instanceId) override {
		cout << "WppDevice: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(WppDevice &inst, const ResLink &resId) override {
        cout << "WppDevice: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(WppDevice &inst, const ResLink &resId) override {
        cout << "WppDevice: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(WppDevice &inst, const ResLink &resId) override {
        cout << "WppDevice: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(WppDevice &inst) override {
        cout << "WppDevice: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }

    private:
    bool _reboot;
};

#endif // DEVICE_H_