#ifndef FIRMWARE_UPDATE_H_
#define FIRMWARE_UPDATE_H_

#include <iostream>
#include <fstream>
#include "WppRegistry.h"
#include "ObjActObserver.h"
#include "WppClient.h"
#include "WppTaskQueue.h"

#if RES_5_8
#include "FwUriDownloader.h"
#endif
#include "FwAutoDownloader.h"

using namespace wpp;
using namespace std;

class FirmwareUpdateImpl: public ObjActObserver, public FwUpdater {
public:
    FirmwareUpdateImpl();
    ~FirmwareUpdateImpl();

    void init(Object &obj);

private:
    void objectRestore(Object &object) override;

    void startUpdating() override;
    bool isUpdated() override;
    FwUpdRes lastUpdateResult() override;
    STRING_T pkgName() override;
    STRING_T pkgVersion() override;

private:
    #if RES_5_8
    FwUriDownloader _uriDownloader;
    #endif
    FwAutoDownloader _autoDownloader;
};

#endif // FIRMWARE_UPDATE_H_