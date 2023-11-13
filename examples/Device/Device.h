#ifndef DEVICE_H_
#define DEVICE_H_

#include "WppRegistry.h"
#include "ObjActObserver.h"


using namespace wpp;

class DeviceImpl: public wpp::ObjActObserver {
public:
    DeviceImpl();
    ~DeviceImpl();

    void init(Object &obj);
    void requestReboot();
    bool isNeededReboot();

private:
	void objectRestore(Object &object) override;
    bool _reboot;
};

#endif // DEVICE_H_