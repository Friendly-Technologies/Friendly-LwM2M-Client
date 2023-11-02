#ifndef CONNECTIVITY_MONITORING_H_
#define CONNECTIVITY_MONITORING_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class ConnectivityMonitoringImpl: public ObjObserver, public InstObserver {
	public:
	void init(Object &connectivityMonitoringObj) {
		connectivityMonitoringObj.subscribe(this);
        Instance *connectivityMonitoring = connectivityMonitoringObj.createInstance();
        connectivityMonitoring->subscribe(this);

		cout << "ConnectivityMonitoringImpl: instance initialized" << endl;
	}

	void objectRestore(Object &object) override {
		cout << "Device: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object &object, ID_T instanceId) override {
        cout << "Device: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object &object, ID_T instanceId) override {
		cout << "Device: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(Instance &inst, const ResLink &resId) override {
        cout << "Device: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(Instance &inst, const ResLink &resId) override {
        cout << "Device: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(Instance &inst, const ResLink &resId) override {
        cout << "Device: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(Instance &inst) override {
        cout << "Device: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }
};

#endif // CONNECTIVITY_MONITORING_H_