#include "Lwm2mServer.h"
#include <iostream>

Lwm2mServerObj::Lwm2mServerObj() {}

Lwm2mServerObj::~Lwm2mServerObj() {}

void Lwm2mServerObj::init(Object &obj) {
	obj.subscribe(this);
	wpp::Instance *inst0 = obj.createInstance(0);
	inst0->set(Lwm2mServer::SHORT_SERVER_ID_0, INT_T(123));
	inst0->set(Lwm2mServer::BINDING_7, WPP_BINDING_UDP);
	inst0->set(Lwm2mServer::LIFETIME_1, TIME_T(25));
	inst0->set(Lwm2mServer::NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, false);
}

void Lwm2mServerImpl::objectRestore(Object &object) {
	cout << "Lwm2mServer: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
	init(object);
}
