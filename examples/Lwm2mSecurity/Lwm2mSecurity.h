#ifndef LWM2M_SECURITY_H_
#define LWM2M_SECURITY_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"

using namespace wpp;
using namespace std;

class Lwm2mSecurityImpl: public wpp::ObjActObserver {
public:
    Lwm2mSecurityImpl();
    ~Lwm2mSecurityImpl();

    void init(Object &obj);
	void objectRestore(Object &object) override;
};

#endif // LWM2M_SECURITY_H_