#include "Lwm2mServer.h"
#include <iostream>

Lwm2mServerImpl::Lwm2mServerImpl() {}

Lwm2mServerImpl::~Lwm2mServerImpl() {}

void Lwm2mServerImpl::init(Object &obj) {
	obj.actSubscribe(this);
	wpp::Instance *inst0 = obj.createInstance(0);
	inst0->set<INT_T>(Lwm2mServer::SHORT_SERVER_ID_0, TEST_SERVER_SHORT_ID);
	inst0->set<STRING_T>(Lwm2mServer::BINDING_7, WPP_BINDING_UDP);
	inst0->set<TIME_T>(Lwm2mServer::LIFETIME_1, 25);
	inst0->set<BOOL_T>(Lwm2mServer::NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, false);

	#if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::createInst(obj, Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::createInst(*inst0, TEST_SERVER_SHORT_ID);
	#endif
}

void Lwm2mServerImpl::objectRestore(Object &object) {
	cout << "Lwm2mServer: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
	init(object);
}
