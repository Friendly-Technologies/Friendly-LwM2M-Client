#ifndef LWM2M_ACCESS_CONTROL_H_
#define LWM2M_ACCESS_CONTROL_H_

#include "WppRegistry.h"
#include "ObjActObserver.h"


using namespace wpp;

class Lwm2mAccessControlImpl: public wpp::ObjActObserver {
public:
    Lwm2mAccessControlImpl();
    ~Lwm2mAccessControlImpl();

    void init(Object &obj);

private:
	void objectRestore(Object &object) override;
};

#endif // LWM2M_ACCESS_CONTROL_H_