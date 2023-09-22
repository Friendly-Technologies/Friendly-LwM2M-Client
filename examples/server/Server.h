#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include "WppRegistry.h"
#include "IObjObserver.h"
#include "IInstObserver.h"

using namespace wpp;
using namespace std;

class ServerImpl: public IObjObserver<Server>, public IInstObserver<Server> {
	public:
    void init(Object<Server> &serverObj) {
        serverObj.subscribe(this);
        wpp::Server *server = serverObj.createInstance();
        server->subscribe(this);
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

	virtual void resourceRead(Server &inst, const ResourceID &resId) override {
        cout << "Server: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceWrite(Server &inst, const ResourceID &resId) override {
        cout << "Server: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceExecute(Server &inst, const ResourceID &resId) override {
        cout << "Server: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }
};

#endif // SERVER_H_