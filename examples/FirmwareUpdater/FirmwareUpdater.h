#ifndef FIRMWARE_UPDATE_H_
#define FIRMWARE_UPDATE_H_

#include "WppClient.h"

using namespace wpp;
using namespace std;

class FirmwareUpdater: public FwUpdater {
public:
    FirmwareUpdater();
    ~FirmwareUpdater();

private:
    void startUpdating() override;
    bool isUpdated() override;
    FwUpdRes lastUpdateResult() override;
    #if RES_5_6
    STRING_T pkgName() override;
    #endif
    #if RES_5_7
    STRING_T pkgVersion() override;
    #endif
    void reset() override;

private:
    FwUpdRes _lastUpdateResult;
};

#endif // FIRMWARE_UPDATE_H_