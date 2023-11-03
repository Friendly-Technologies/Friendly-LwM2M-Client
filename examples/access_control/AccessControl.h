#ifndef ACCESS_CONTROL_H_
#define ACCESS_CONTROL_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

#include <ifaddrs.h>

using namespace wpp;
using namespace std;


class AccessControl: public ObjObserver, public InstObserver {
	public:
	void init(Object &accssCtrlObj) {
		accssCtrlObj.subscribe(this);
        Instance *accssCtrl = accssCtrlObj.createInstance();
        accssCtrl->subscribe(this);

        // accssCtrl->set();
        // accssCtrl->set();
        // accssCtrl->set();
	}

	void objectRestore(Object &object) override {
		cout << "AccessControl: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object &object, ID_T instanceId) override {
        cout << "AccessControl: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object &object, ID_T instanceId) override {
		cout << "AccessControl: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(Instance &inst, const ResLink &resId) override {
        cout << "AccessControl: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(Instance &inst, const ResLink &resId) override {
        cout << "AccessControl: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(Instance &inst, const ResLink &resId) override {
        cout << "AccessControl: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(Instance &inst) override {
        cout << "AccessControl: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }
};

#endif // ACCESS_CONTROL_H_
