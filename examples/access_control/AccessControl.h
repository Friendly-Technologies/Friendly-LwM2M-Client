#ifndef ACCESS_CONTROL_H_
#define ACCESS_CONTROL_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"

#include <ifaddrs.h>

using namespace wpp;
using namespace std;

class AccessControl: public ObjActObserver {
public:
	void init(Object &accssCtrlObj) {
		accssCtrlObj.subscribe(this);
        Instance *accssCtrl = accssCtrlObj.createInstance();

        accssCtrl->set(wpp::Lwm2mAccessControl::OBJECT_ID_0, INT_T(35464));
        accssCtrl->set(wpp::Lwm2mAccessControl::OBJECT_INSTANCE_ID_1, INT_T(35464));
        accssCtrl->set(wpp::Lwm2mAccessControl::ACCESS_CONTROL_OWNER_3, INT_T(35464));

        #if RES_2_2
        uint8_t acl = Lwm2mAccessControl::R_O_W_ATTR | Lwm2mAccessControl::WRITE | Lwm2mAccessControl::EXECUTE;
        accssCtrl->set(wpp::Lwm2mAccessControl::ACL_2, INT_T(acl));
        #endif
	}

	void objectRestore(Object &object) override {
		cout << "AccessControl: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}
};

#endif // ACCESS_CONTROL_H_
