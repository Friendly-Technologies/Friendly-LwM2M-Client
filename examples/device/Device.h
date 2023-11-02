#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class DeviceImpl: public ObjObserver, public InstObserver {
	public:
    DeviceImpl(): _reboot(false) {}

    void init(Object &deviceObj) {
        deviceObj.subscribe(this);
        wpp::Instance *device = deviceObj.createInstance();
        device->subscribe(this);

        device->set(Device::REBOOT_4, (EXECUTE_T)[this](ID_T id, const OPAQUE_T& data) {
            cout << "Device: execute REBOOT_4" << endl;
            this->_reboot = true;
        });
        device->set(Device::ERROR_CODE_11, (INT_T)Device::NO_ERROR);
        device->set(Device::SUPPORTED_BINDING_AND_MODES_16, WPP_BINDING_UDP);
        device->set(Device::MANUFACTURER_0, (STRING_T)"Open Mobile Alliance");
        device->set(Device::MODEL_NUMBER_1, (STRING_T)"Lightweight M2M Client");
        device->set(Device::SERIAL_NUMBER_2, (STRING_T)"345000123");
    }

    bool isNeededReboot() {
        return _reboot;
    }

    private:
	void objectRestore(Object &object) override {
		cout << "Device: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object &object, ID_T instanceId) override {
        cout << "Device: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object &object, ID_T instanceId) override {
		cout << "Device: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(Instance &inst, const ResLink &resId) override {
        cout << "Device: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(Instance &inst, const ResLink &resId) override {
        cout << "Device: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(Instance &inst, const ResLink &resId) override {
        cout << "Device: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(Instance &inst) override {
        cout << "Device: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }

    private:
    bool _reboot;
};

#endif // DEVICE_H_