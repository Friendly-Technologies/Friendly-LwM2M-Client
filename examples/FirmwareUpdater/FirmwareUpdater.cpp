#include "FirmwareUpdater.h"
#include <iostream>
#include "FirmwareChecker.h"

FirmwareUpdater::FirmwareUpdater() {
    _lastUpdateResult = FwUpdRes::R_INITIAL;
}

FirmwareUpdater::~FirmwareUpdater() {}

void FirmwareUpdater::startUpdating() {
    cout << "FwUpdateImpl: startUpdating" << endl;
    _lastUpdateResult =  FirmwareChecker::getUpdateResult() ? R_FW_UPD_SUCCESS : R_FW_UPD_FAIL;
}

bool FirmwareUpdater::isUpdated() {
    cout << "FwUpdateImpl: isUpdated: true" << endl;
    return true;
}

FwUpdRes FirmwareUpdater::lastUpdateResult() {
    cout << "FwUpdateImpl: lastUpdateResult: " << (int)_lastUpdateResult << endl;
    return _lastUpdateResult;
}

#if RES_5_6
STRING_T FirmwareUpdater::pkgName() {
    string res = FirmwareChecker::getPkgName();
    cout << "FwUpdateImpl: pkgName: " << res << endl;
    return res == "default" ? "current_fw" : res;
}
#endif

#if RES_5_7
STRING_T FirmwareUpdater::pkgVersion() {
    string res = FirmwareChecker::getPkgVersion();
    cout << "FwUpdateImpl: pkgVersion: " << res << endl;
    return res == "default" ? "1.0.0" : res;
}
#endif

void FirmwareUpdater::reset() {
    cout << "FwUpdateImpl: reset" << endl;
    _lastUpdateResult = FwUpdRes::R_INITIAL;
}
