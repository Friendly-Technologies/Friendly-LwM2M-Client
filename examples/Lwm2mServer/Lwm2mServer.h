#ifndef LWM2M_SERVER_H_
#define LWM2M_SERVER_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"

#define TEST_SERVER_SHORT_ID 123

using namespace wpp;
using namespace std;

class Lwm2mServerImpl: public wpp::ObjActObserver {
public:
    Lwm2mServerImpl();
    ~Lwm2mServerImpl();

    void init(Object &obj);
	void objectRestore(Object &object) override;
};

#endif // LWM2M_SERVER_H_