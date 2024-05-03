#include "FirmwareUpdate.h"
#include "FirmwareChecker.h"
#include "Lwm2mServer.h"

FirmwareUpdateImpl::FirmwareUpdateImpl() {}

FirmwareUpdateImpl::~FirmwareUpdateImpl() {}

void FirmwareUpdateImpl::init(Object &obj) {
    _lastUpdateResult = FwUpdRes::R_INITIAL;

	obj.actSubscribe(this);
	obj.createInstance(0);

    FirmwareUpdate::setFwUpdater(obj.getClient(), *this);
    FirmwareUpdate::setFwInternalDownloader(obj.getClient(), _internalDownloader);
    #if RES_5_8
    FirmwareUpdate::setFwExternalDownloader(obj.getClient(), _uriDownloader);
    #endif

    #if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::create(obj, Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::create(*FirmwareUpdate::instance(obj.getClient()), TEST_SERVER_SHORT_ID);
	#endif
}

void FirmwareUpdateImpl::objectRestore(Object &object) {
	cout << "FwUpdateImpl: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
    init(object);
}

void FirmwareUpdateImpl::startUpdating() {
    cout << "FwUpdateImpl: startUpdating" << endl;
    _lastUpdateResult =  FirmwareChecker::getUpdateResult() ? R_FW_UPD_SUCCESS : R_FW_UPD_FAIL;
}

bool FirmwareUpdateImpl::isUpdated() {
    cout << "FwUpdateImpl: isUpdated: true" << endl;
    return true;
}

FwUpdRes FirmwareUpdateImpl::lastUpdateResult() {
    cout << "FwUpdateImpl: lastUpdateResult: " << (int)_lastUpdateResult << endl;
    return _lastUpdateResult;
}

#if RES_5_6
STRING_T FirmwareUpdateImpl::pkgName() {
    string res = FirmwareChecker::getPkgName();
    cout << "FwUpdateImpl: pkgName: " << res << endl;
    return res == "default" ? "current_fw" : res;
}
#endif

#if RES_5_7
STRING_T FirmwareUpdateImpl::pkgVersion() {
    string res = FirmwareChecker::getPkgVersion();
    cout << "FwUpdateImpl: pkgVersion: " << res << endl;
    return res == "default" ? "1.0.0" : res;
}
#endif

void FirmwareUpdateImpl::reset() {
    cout << "FwUpdateImpl: reset" << endl;
    _lastUpdateResult = FwUpdRes::R_INITIAL;
}
