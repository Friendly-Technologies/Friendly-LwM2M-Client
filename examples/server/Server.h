#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"

using namespace wpp;
using namespace std;

class ServerImpl: public ObjObserver, public InstOpObserver {
	public:
    void init(Object &serverObj) {
        serverObj.subscribe(this);
        wpp::Instance *server = serverObj.createInstance();
        server->subscribe(this);

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

    void instanceCreated(Object &object, ID_T instanceId) override {
        cout << "Lwm2mServer: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
        object.instance(instanceId)->subscribe(this);
    }

    void instanceDeleting(Object &object, ID_T instanceId) override {
		cout << "Lwm2mServer: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(Instance &inst, const ResLink &resId) override {
        cout << "Lwm2mServer: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(Instance &inst, const ResLink &resId) override {
        cout << "Lwm2mServer: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(Instance &inst, const ResLink &resId) override {
        cout << "Lwm2mServer: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(Instance &inst) override {
        cout << "Lwm2mServer: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }
};

#endif // SERVER_H_