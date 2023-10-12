#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class ServerImpl: public ObjObserver<WppLwM2MServer>, public InstObserver<WppLwM2MServer> {
	public:
    void init(Object<WppLwM2MServer> &serverObj) {
        serverObj.subscribe(this);
        wpp::WppLwM2MServer *server = serverObj.createInstance();
        server->subscribe(this);

        server->set(WppLwM2MServer::SHORT_SERVER_ID_0, (INT_T)123);
        server->set(WppLwM2MServer::BINDING_7, WPP_BINDING_UDP);
        server->set(WppLwM2MServer::LIFETIME_1, (INT_T)25);
        server->set(WppLwM2MServer::NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, false);
    }

	void objectRestore(Object<WppLwM2MServer> &object) override {
		cout << "WppLwM2MServer: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object<WppLwM2MServer> &object, ID_T instanceId) override {
        cout << "WppLwM2MServer: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
        object.instance(instanceId)->subscribe(this);
    }

    void instanceDeleting(Object<WppLwM2MServer> &object, ID_T instanceId) override {
		cout << "WppLwM2MServer: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(WppLwM2MServer &inst, const ResLink &resId) override {
        cout << "WppLwM2MServer: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(WppLwM2MServer &inst, const ResLink &resId) override {
        cout << "WppLwM2MServer: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(WppLwM2MServer &inst, const ResLink &resId) override {
        cout << "WppLwM2MServer: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(WppLwM2MServer &inst) override {
        cout << "WppLwM2MServer: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }
};

#endif // SERVER_H_