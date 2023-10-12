#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class ServerImpl: public ObjObserver<LwM2MServer>, public InstObserver<LwM2MServer> {
	public:
    void init(Object<LwM2MServer> &serverObj) {
        serverObj.subscribe(this);
        wpp::LwM2MServer *server = serverObj.createInstance();
        server->subscribe(this);

        server->set(LwM2MServer::SHORT_SERVER_ID_0, (INT_T)123);
        server->set(LwM2MServer::BINDING_7, WPP_BINDING_UDP);
        server->set(LwM2MServer::LIFETIME_1, (INT_T)25);
        server->set(LwM2MServer::NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, false);
    }

	void objectRestore(Object<LwM2MServer> &object) override {
		cout << "LwM2MServer: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object<LwM2MServer> &object, ID_T instanceId) override {
        cout << "LwM2MServer: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
        object.instance(instanceId)->subscribe(this);
    }

    void instanceDeleting(Object<LwM2MServer> &object, ID_T instanceId) override {
		cout << "LwM2MServer: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(LwM2MServer &inst, const ResLink &resId) override {
        cout << "LwM2MServer: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(LwM2MServer &inst, const ResLink &resId) override {
        cout << "LwM2MServer: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(LwM2MServer &inst, const ResLink &resId) override {
        cout << "LwM2MServer: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(LwM2MServer &inst) override {
        cout << "LwM2MServer: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }
};

#endif // SERVER_H_