#ifndef FIRMWARE_UPDATE_H
#define FIRMWARE_UPDATE_H

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"
#include "InstEventObserver.h"

using namespace wpp;
using namespace std;

class FwUpdateImpl: public ObjActObserver, public InstEventObserver {
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

    void instEvent(Instance &inst, EVENT_ID_T eventId) override {
        cout << "FwUpdateImpl: event: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ", eventId: " << eventId << endl;
        if (eventId == FirmwareUpdate::E_URI_DOWNLOADIN) _uriDownloading = true;
    }

    void fwIsDownloaded(Instance &inst) {
        if (!_uriDownloading) return;

        _uriDownloading = false;
        inst.set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_DOWNLOADED);
    }

private:
    bool _uriDownloading = false;
};

#endif // FIRMWARE_UPDATE_H