#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"

using namespace wpp;
using namespace std;

class ServerImpl: public ObjActObserver {
	public:
    void init(Object &serverObj) {
        serverObj.subscribe(this);
        wpp::Instance *server = serverObj.createInstance();

        server->set(Lwm2mServer::SHORT_SERVER_ID_0, (INT_T)123);
        server->set(Lwm2mServer::BINDING_7, WPP_BINDING_UDP);
        server->set(Lwm2mServer::LIFETIME_1, (INT_T)25);
        server->set(Lwm2mServer::NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, false);
    }

	void objectRestore(Object &object) override {
		cout << "Lwm2mServer: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}
};

#endif // SERVER_H_