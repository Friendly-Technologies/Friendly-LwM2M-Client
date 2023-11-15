#ifndef DEVICE_H_
#define DEVICE_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"

using namespace wpp;
using namespace std;

class DeviceImpl: public wpp::ObjActObserver {
public:
    DeviceImpl();
    ~DeviceImpl();

    void init(Object &obj);
    void objectRestore(Object &object) override;
    
    void requestReboot();
    bool isNeededReboot();

private:
    bool _reboot;
};

#endif // DEVICE_H_