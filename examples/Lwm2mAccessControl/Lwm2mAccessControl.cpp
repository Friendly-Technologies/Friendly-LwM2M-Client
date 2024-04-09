#include "Lwm2mAccessControl.h"

Lwm2mAccessControlImpl::Lwm2mAccessControlImpl() {}

Lwm2mAccessControlImpl::~Lwm2mAccessControlImpl() {}

void Lwm2mAccessControlImpl::init(Object &obj) {
	obj.actSubscribe(this);
	wpp::Instance *inst0 = obj.createInstance();
	inst0->set(Lwm2mAccessControl::OBJECT_ID_0, INT_T(OBJ_ID::LWM2M_SERVER));
	inst0->set(Lwm2mAccessControl::OBJECT_INSTANCE_ID_1, INT_T(LWM2M_MAX_ID));
	inst0->set(Lwm2mAccessControl::ACCESS_CONTROL_OWNER_3, INT_T(LWM2M_MAX_ID));
	#if RES_2_2
    uint8_t acl = Lwm2mAccessControl::ALL_RIGHTS;
    inst0->set({Lwm2mAccessControl::ACL_2, 123}, INT_T(acl));
    #endif
}

void Lwm2mAccessControlImpl::objectRestore(Object &object) {
	std::cout << "AccessControl: objectRestore: " << (ID_T)object.getObjectID() << std::endl;
	object.clear();
	init(object);
}

