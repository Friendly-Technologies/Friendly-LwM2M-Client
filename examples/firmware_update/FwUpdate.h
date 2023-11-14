#ifndef FIRMWARE_UPDATE_H
#define FIRMWARE_UPDATE_H

#include <iostream>
#include <fstream>
#include "WppRegistry.h"
#include "ObjActObserver.h"
#include "InstEventObserver.h"
#include "WppClient.h"
#include "Device.h"
#include "WppTaskQueue.h"

#include "FwDownloaderStub.h"

using namespace wpp;
using namespace std;

class FwUpdateImpl: public ObjActObserver, public InstEventObserver {
public:
    FwUpdateImpl(DeviceImpl &device) : _device(device) {}

	void init(Object &fwUpdObj) {
		fwUpdObj.subscribe(this);
        Instance *fwUpd = fwUpdObj.createInstance();
        fwUpd->subscribe(this);

        fwUpd->set(FirmwareUpdate::UPDATE_2, (EXECUTE_T)[this](Instance& inst, ID_T resId, const OPAQUE_T& data) {
            cout << "FirmwareUpdate: execute UPDATE_2" << endl;
            this->update(inst);
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
        #if RES_5_13
        fwUpd->set(FirmwareUpdate::MAXIMUM_DEFER_PERIOD_13, (UINT_T)10);
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
            _downloader.startDownloading(uri, [](string file){ 
                cout << "FW is downloaded to file: " << file << endl;
                WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, [](WppClient &client, WppTaskQueue::ctx_t ctx) -> bool {
                    client.registry().firmwareUpdate().instance()->set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_DOWNLOADED);
                    return true;
                });
                //this->fwIsDownloaded(); 
            });
        }
    }

    void fwIsDownloaded() {
        WppClient *client = WppClient::takeOwnershipBlocking();
        client->registry().firmwareUpdate().instance()->set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_DOWNLOADED);
        client->giveOwnership();
    }

    void update(Instance& inst) {
        #if RES_5_13
        static uint8_t reqCnt = 0;
        if (!reqCnt) {
            inst.set(FirmwareUpdate::UPDATE_RESULT_5, (INT_T)FirmwareUpdate::R_FW_UPD_DEFERRED);
            reqCnt++;
            return;
        }
        #endif

        STRING_T fileName = "test";
        #if RES_5_6 && RES_5_7
        STRING_T pkgName, pkgVersion;
        inst.get(FirmwareUpdate::PKGNAME_6, pkgName);
        inst.get(FirmwareUpdate::PKGVERSION_7, pkgVersion);
        fileName += pkgName + pkgVersion;
        #endif
        fileName += ".fw";

        const OPAQUE_T *fwData = NULL;
        inst.getPtr(FirmwareUpdate::PACKAGE_0, &fwData);
        if (fwData && !fwData->empty()) this->saveToFile(fileName, fwData);

        _device.requestReboot();
    }

    void saveToFile(STRING_T fileName, const OPAQUE_T *buff) {
        if (buff == NULL) return;

        std::ofstream file(fileName, std::ios::binary);
        if (!file.is_open()) return;
        file.write(reinterpret_cast<const char*>(buff->data()), buff->size());
        file.close();
    }

private:
    FwDownloaderStub _downloader;
    DeviceImpl &_device;
};

#endif // FIRMWARE_UPDATE_H
