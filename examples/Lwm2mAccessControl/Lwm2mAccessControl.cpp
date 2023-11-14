#include "Lwm2mAccessControl.h"

Lwm2mAccessControlImpl::Lwm2mAccessControlImpl() {}

Lwm2mAccessControlImpl::~Lwm2mAccessControlImpl() {}

void Lwm2mAccessControlImpl::init(Object &obj) {
	obj.subscribe(this);
	wpp::Instance *inst0 = obj.createInstance(0);
	Lwm2mAccessControl->set(Lwm2mAccessControl::OBJECT_ID_0, INT_T(35464));
	Lwm2mAccessControl->set(Lwm2mAccessControl::OBJECT_INSTANCE_ID_1, INT_T(35464));
	Lwm2mAccessControl->set(Lwm2mAccessControl::ACCESS_CONTROL_OWNER_3, INT_T(35464));
	#if RES_2_2
    uint8_t acl = Lwm2mAccessControl::R_O_W_ATTR | Lwm2mAccessControl::WRITE | Lwm2mAccessControl::EXECUTE;
    accssCtrl->set(wpp::Lwm2mAccessControl::ACL_2, INT_T(acl));
    #endif
}


void Lwm2mAccessControlImpl::objectRestore(Object &object) {
	cout << "AccessControl: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
	init(object);
}

