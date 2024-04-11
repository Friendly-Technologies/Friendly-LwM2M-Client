#include "Device.h"
#include "Lwm2mServer.h"

DeviceImpl::DeviceImpl(): _reboot(false) {}

DeviceImpl::~DeviceImpl() {}

void DeviceImpl::init(Object &obj) {
	obj.actSubscribe(this);
	wpp::Instance *inst0 = obj.createInstance(0);

    inst0->set(Device::REBOOT_4, (EXECUTE_T)[this](Instance& inst, ID_T resId, const OPAQUE_T& data) {
    	cout << "Device: execute REBOOT_4" << endl;
    	this->requestReboot();
        return true;
    });
    inst0->set(Device::ERROR_CODE_11, (INT_T)Device::NO_ERROR);
    inst0->set(Device::SUPPORTED_BINDING_AND_MODES_16, WPP_BINDING_UDP);
    inst0->set(Device::MANUFACTURER_0, (STRING_T)"Wakaama Plus");
    inst0->set(Device::MODEL_NUMBER_1, (STRING_T)"Lightweight M2M Client");
    inst0->set(Device::SERIAL_NUMBER_2, (STRING_T)"0123456789");

    #if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::createInst(obj, Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::createInst(*inst0, TEST_SERVER_SHORT_ID);
	#endif
}

void DeviceImpl::objectRestore(Object &object) {
	object.clear();
	init(object);
}

void DeviceImpl::requestReboot() {
    _reboot = true;
}

bool DeviceImpl::isNeededReboot() {
    return _reboot;
}
