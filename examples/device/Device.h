#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"

using namespace wpp;
using namespace std;

class DeviceImpl: public ObjActObserver {
public:
    DeviceImpl(): _reboot(false) {}

    void init(Object &deviceObj) {
        deviceObj.subscribe(this);
        wpp::Instance *device = deviceObj.createInstance();

        device->set(Device::REBOOT_4, (EXECUTE_T)[this](ID_T id, const OPAQUE_T& data) {
            cout << "Device: execute REBOOT_4" << endl;
            this->_reboot = true;
            return true;
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

private:
    bool _reboot;
};

#endif // DEVICE_H_