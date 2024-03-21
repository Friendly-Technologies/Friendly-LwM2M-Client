#include "FirmwareUpdate.h"

FirmwareUpdateImpl::FirmwareUpdateImpl() {}

FirmwareUpdateImpl::~FirmwareUpdateImpl() {}

void FirmwareUpdateImpl::init(Object &obj) {
	obj.actSubscribe(this);
	FirmwareUpdate *fwInst = static_cast<FirmwareUpdate *>(obj.createInstance(0));

    fwInst->setFwUpdater(*this);
    fwInst->setFwAutoDownloader(_autoDownloader);
    #if RES_5_8
    fwInst->setFwExternalDownloader(_uriDownloader);
    #endif
}

void FirmwareUpdateImpl::objectRestore(Object &object) {
	cout << "FwUpdateImpl: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
    init(object);
}

void FirmwareUpdateImpl::startUpdating() {
    cout << "FwUpdateImpl: startUpdating" << endl;
}

bool FirmwareUpdateImpl::isUpdated() {
    cout << "FwUpdateImpl: isUpdated" << endl;
    return true;
}

FwUpdRes FirmwareUpdateImpl::lastUpdateResult() {
    cout << "FwUpdateImpl: lastUpdateResult" << endl;
    return FwUpdRes::R_FW_UPD_SUCCESS;
}

STRING_T FirmwareUpdateImpl::pkgName() {
    cout << "FwUpdateImpl: pkgName" << endl;
    return "test";
}

STRING_T FirmwareUpdateImpl::pkgVersion() {
    cout << "FwUpdateImpl: pkgVersion" << endl;
    return "1.0";
}