#ifndef LWM2M_SECURITY_H_
#define LWM2M_SECURITY_H_

#include "WppRegistry.h"
#include "ObjActObserver.h"


using namespace wpp;

class Lwm2mSecurityImpl: public wpp::ObjActObserver {
public:
    Lwm2mSecurityImpl();
    ~Lwm2mSecurityImpl();

    void init(Object &obj);

private:
	void objectRestore(Object &object) override;
};

#endif // LWM2M_SECURITY_H_