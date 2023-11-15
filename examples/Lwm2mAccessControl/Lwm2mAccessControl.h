#ifndef LWM2M_ACCESS_CONTROL_H_
#define LWM2M_ACCESS_CONTROL_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"

#include <ifaddrs.h>

using namespace wpp;

class Lwm2mAccessControlImpl: public wpp::ObjActObserver {
public:
    Lwm2mAccessControlImpl();
    ~Lwm2mAccessControlImpl();

    void init(Object &obj);
	void objectRestore(Object &object) override;
};

#endif // LWM2M_ACCESS_CONTROL_H_