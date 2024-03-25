#include "FirmwareUpdate.h"

FirmwareUpdateImpl::FirmwareUpdateImpl() {}

FirmwareUpdateImpl::~FirmwareUpdateImpl() {}

void FirmwareUpdateImpl::init(Object &obj) {
    _lastUpdateResult = FwUpdRes::R_INITIAL;

	obj.actSubscribe(this);
	FirmwareUpdate *fwInst = static_cast<FirmwareUpdate *>(obj.createInstance(0));

    fwInst->setFwUpdater(*this);
    fwInst->setFwInternalDownloader(_internalDownloader);
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
    _lastUpdateResult = FwUpdRes::R_FW_UPD_SUCCESS;
}

bool FirmwareUpdateImpl::isUpdated() {
    cout << "FwUpdateImpl: isUpdated" << endl;
    return true;
}

FwUpdRes FirmwareUpdateImpl::lastUpdateResult() {
    cout << "FwUpdateImpl: lastUpdateResult" << endl;
    return _lastUpdateResult;
}

#if RES_5_6
STRING_T FirmwareUpdateImpl::pkgName() {
    cout << "FwUpdateImpl: pkgName" << endl;
    return "test";
}
#endif

#if RES_5_7
STRING_T FirmwareUpdateImpl::pkgVersion() {
    cout << "FwUpdateImpl: pkgVersion" << endl;
    return "1.0";
}
#endif

void FirmwareUpdateImpl::reset() {
    cout << "FwUpdateImpl: reset" << endl;
    _lastUpdateResult = FwUpdRes::R_INITIAL;
}