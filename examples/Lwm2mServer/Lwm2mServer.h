#ifndef LWM2M_SERVER_H_
#define LWM2M_SERVER_H_

#include "WppRegistry.h"
#include "ObjActObserver.h"


using namespace wpp;

class Lwm2mServerImpl: public wpp::ObjActObserver {
public:
    Lwm2mServerImpl();
    ~Lwm2mServerImpl();

    void init(Object &obj);

private:
	void objectRestore(Object &object) override;
};

#endif // LWM2M_SERVER_H_