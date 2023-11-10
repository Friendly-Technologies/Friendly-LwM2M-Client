#ifndef FIRMWARE_UPDATE_H
#define FIRMWARE_UPDATE_H

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"
#include "InstEventObserver.h"
#include "WppClient.h"

#include "FwDownloaderStub.h"

using namespace wpp;
using namespace std;

class FwUpdateImpl: public ObjActObserver, public InstEventObserver {
public:
	void init(Object &fwUpdObj) {
		fwUpdObj.subscribe(this);
        Instance *fwUpd = fwUpdObj.createInstance();
        fwUpd->subscribe(this);

        fwUpd->set(FirmwareUpdate::UPDATE_2, (EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) {
            cout << "FirmwareUpdate: execute UPDATE_2" << endl;
            return true;
        });
        fwUpd->set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_IDLE);
        fwUpd->set(FirmwareUpdate::UPDATE_RESULT_5, (INT_T)getLastUpdResult());
        #if RES_5_8
        fwUpd->set(FirmwareUpdate::FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8, (INT_T)FirmwareUpdate::COAP);
        fwUpd->set({FirmwareUpdate::FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8, 1}, (INT_T)FirmwareUpdate::COAPS);
        #endif
        fwUpd->set(FirmwareUpdate::FIRMWARE_UPDATE_DELIVERY_METHOD_9, (INT_T)FirmwareUpdate::PUSH);
        #if RES_5_10
        fwUpd->set(FirmwareUpdate::CANCEL_10, (EXECUTE_T)[this](Instance& inst, ID_T resId, const OPAQUE_T& data) {
            cout << "FirmwareUpdate: execute CANCEL_10" << endl;
            this->_downloader.cancelDownloading();
            return true;
        });
        #endif
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
        cout << "FwUpdateImpl: event: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ", eventId: " << (int)eventId << endl;
        if (eventId == FirmwareUpdate::E_URI_DOWNLOADIN) {
            STRING_T uri;
            inst.get(FirmwareUpdate::PACKAGE_URI_1, uri);
            _downloader.startDownloading(uri, [this](string file){ 
                cout << "FW is downloaded to file: " << file << endl;
                this->fwIsDownloaded(); 
            });
        }
    }

    void fwIsDownloaded() {
        WppClient *client = WppClient::takeOwnershipBlocking();
        client->registry().firmwareUpdate().instance()->set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_DOWNLOADED);
        client->giveOwnership();
    }

private:
    FwDownloaderStub _downloader;
};

#endif // FIRMWARE_UPDATE_H