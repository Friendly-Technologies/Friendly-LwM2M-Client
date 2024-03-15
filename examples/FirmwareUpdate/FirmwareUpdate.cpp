#include "FirmwareUpdate.h"

FirmwareUpdateImpl::FirmwareUpdateImpl(DeviceImpl &device) : _device(device) {}

FirmwareUpdateImpl::~FirmwareUpdateImpl() {}

void FirmwareUpdateImpl::init(Object &obj) {
	obj.actSubscribe(this);
	wpp::Instance *inst0 = obj.createInstance(0);
	inst0->eventSubscribe(this);
    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    inst0->blockOpSubscribe(this);
    #endif
	inst0->set(FirmwareUpdate::UPDATE_2, (EXECUTE_T)[this](Instance& inst, ID_T id, const OPAQUE_T& data) {
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
	inst0->set(FirmwareUpdate::CANCEL_10, (EXECUTE_T)[this](Instance& inst, ID_T id, const OPAQUE_T& data) {
		cout << "FirmwareUpdate: execute CANCEL_10" << endl;
		this->_downloader.cancelDownloading();
		return true;
    });
    #endif
    #if RES_5_13
    inst0->set(FirmwareUpdate::MAXIMUM_DEFER_PERIOD_13, (UINT_T)10);
    #endif
}

void FirmwareUpdateImpl::objectRestore(Object &object) {
	cout << "FwUpdateImpl: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
    init(object);
}

void FirmwareUpdateImpl::instEvent(Instance &inst, EVENT_ID_T eventId) {
    cout << "FwUpdateImpl: event: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ", eventId: " << (int)eventId << endl;
    if (eventId == FirmwareUpdate::E_URI_DOWNLOADIN) {
        STRING_T uri;
        inst.get(FirmwareUpdate::PACKAGE_URI_1, uri);
        _downloader.startDownloading(uri, [](string file) { 
            cout << "FW is downloaded to file: " << file << endl;
            WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, [](WppClient &client, WppTaskQueue::ctx_t ctx) -> bool {
                client.registry().firmwareUpdate().instance()->set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_DOWNLOADED);
                return true;
            });
            //this->fwIsDownloaded(); 
        });
    }
}

#ifdef LWM2M_RAW_BLOCK1_REQUESTS
void FirmwareUpdateImpl::resourceBlockWrite(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) {
    if (resource.resId != FirmwareUpdate::PACKAGE_0) return;
    cout << "FwUpdateImpl: resourceBlockWrite: len->" << buff.size() << ", blockNum: " << blockNum << ", isLastBlock:" << isLastBlock << endl;
    writeToFile(getFileName(), buff);
}
#endif

FirmwareUpdate::UpdRes FirmwareUpdateImpl::getLastUpdResult() {
    return FirmwareUpdate::R_FW_UPD_SUCCESS;
}

void FirmwareUpdateImpl::fwIsDownloaded() {
        WppClient *client = WppClient::takeOwnershipBlocking();
        client->registry().firmwareUpdate().instance()->set(FirmwareUpdate::STATE_3, (INT_T)FirmwareUpdate::S_DOWNLOADED);
        client->giveOwnership();
    }

void FirmwareUpdateImpl::update(Instance& inst) {
    #if RES_5_13
    static uint8_t reqCnt = 0;
    if (!reqCnt) {
        inst.set(FirmwareUpdate::UPDATE_RESULT_5, (INT_T)FirmwareUpdate::R_FW_UPD_DEFERRED);
        reqCnt++;
        return;
    }
    #endif

    #ifndef LWM2M_RAW_BLOCK1_REQUESTS
    const OPAQUE_T *fwData = NULL;
    inst.getPtr(FirmwareUpdate::PACKAGE_0, &fwData);
    if (fwData) writeToFile(getFileName(), *fwData);
    #endif

    _device.requestReboot();
}

string FirmwareUpdateImpl::getFileName() {
    STRING_T fileName = "test";

    #if RES_5_6 && RES_5_7
    STRING_T pkgName, pkgVersion;
    inst.get(FirmwareUpdate::PKGNAME_6, pkgName);
    inst.get(FirmwareUpdate::PKGVERSION_7, pkgVersion);
    fileName += pkgName + pkgVersion;
    #endif
    fileName += ".fw";

    return fileName;
}

void FirmwareUpdateImpl::writeToFile(STRING_T fileName, const OPAQUE_T &buff) {
    if (buff.empty()) return;

    ofstream file(fileName, ios::binary | ios::app);
    if (!file.is_open()) return;
    file.write(reinterpret_cast<const char*>(buff.data()), buff.size());
    file.close();
}
