#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class ServerImpl: public ObjObserver<Server>, public InstObserver<Server> {
	public:
    void init(Object<Server> &serverObj) {
        serverObj.subscribe(this);
        wpp::Server *server = serverObj.createInstance();
        server->subscribe(this);

        server->set(Server::SHORT_SERV_ID, (INT_T)123);
        server->set(Server::BUINDING, (STRING_T)"U");
        server->set(Server::LIFETIME, (INT_T)25);
        server->set(Server::NOTIFICATION_STORING, false);
    }

	void objectRestore(Object<Server> &object) override {
		cout << "Server: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object<Server> &object, ID_T instanceId) override {
        cout << "Server: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object<Server> &object, ID_T instanceId) override {
		cout << "Server: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(Server &inst, const ResLink &resId) override {
        cout << "Server: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(Server &inst, const ResLink &resId) override {
        cout << "Server: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(Server &inst, const ResLink &resId) override {
        cout << "Server: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(Server &inst) override {
        cout << "Server: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }
};

#endif // SERVER_H_