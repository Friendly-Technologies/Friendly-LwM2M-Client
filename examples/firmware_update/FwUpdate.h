#ifndef FIRMWARE_UPDATE_H
#define FIRMWARE_UPDATE_H

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class FwUpdateImpl: public ObjObserver, public InstObserver {
	public:
	void init(Object &fwUpdObj) {
		fwUpdObj.subscribe(this);
        Instance *fwUpd = fwUpdObj.createInstance();
        fwUpd->subscribe(this);

        fwUpd->set(FirmwareUpdate::UPDATE_2, (EXECUTE_T)[](ID_T id, const OPAQUE_T& data) {
            cout << "FirmwareUpdate: execute UPDATE_2" << endl;
            return true;
        });
        fwUpd->set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_IDLE);
        fwUpd->set(FirmwareUpdate::UPDATE_RESULT_5, (INT_T)getLastUpdResult());
        fwUpd->set(FirmwareUpdate::FIRMWARE_UPDATE_DELIVERY_METHOD_9, (INT_T)FirmwareUpdate::PUSH);
	}

    FirmwareUpdate::UpdRes getLastUpdResult() {
        return FirmwareUpdate::R_FW_UPD_SUCCESS;
    }

	void objectRestore(Object &object) override {
		cout << "FwUpdateImpl: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object &object, ID_T instanceId) override {
        cout << "FwUpdateImpl: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object &object, ID_T instanceId) override {
		cout << "FwUpdateImpl: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(Instance &inst, const ResLink &resId) override {
        cout << "FwUpdateImpl: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(Instance &inst, const ResLink &resId) override {
        cout << "FwUpdateImpl: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(Instance &inst, const ResLink &resId) override {
        cout << "FwUpdateImpl: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(Instance &inst) override {
        cout << "FwUpdateImpl: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }

    void event(Instance &inst, EVENT_ID_T eventId) override {
        cout << "FwUpdateImpl: event: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ", eventId: " << eventId << endl;
    }
};

#endif // FIRMWARE_UPDATE_H