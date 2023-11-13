#include "FirmwareUpdate.h"

FirmwareUpdateObj::FirmwareUpdateObj() {}

FirmwareUpdateObj::~FirmwareUpdateObj() {}

void FirmwareUpdateObj::init(Object &obj) {
	obj.subscribe(this);
	wpp::Instance *inst0 = obj.createInstance(0);
	inst0.subscribe(this);

	inst0->set(FirmwareUpdate::UPDATE_2, (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) {
		cout << "FirmwareUpdate: execute UPDATE_2" << endl;
		this->update(inst);
		return true;
	});
	inst0->set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_IDLE);
	inst0->set(FirmwareUpdate::UPDATE_RESULT_5, (INT_T)getLastUpdResult());
    #if RES_5_8
    inst0->set(FirmwareUpdate::FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8, (INT_T)FirmwareUpdate::COAP);
    inst0->set({FirmwareUpdate::FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8, 1}, (INT_T)FirmwareUpdate::COAPS);
    #endif
    inst0->set(FirmwareUpdate::FIRMWARE_UPDATE_DELIVERY_METHOD_9, (INT_T)FirmwareUpdate::PUSH);
	#if RES_5_10
	inst0->set(FirmwareUpdate::CANCEL_10, (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) {
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

void FirmwareUpdateImpl::instEvent(Instance &inst, EVENT_ID_T eventId) {
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

void update(Instance& inst) {
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
