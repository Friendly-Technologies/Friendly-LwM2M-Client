#ifndef FIRMWARE_UPDATE_H_
#define FIRMWARE_UPDATE_H_

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

class FirmwareUpdateImpl: public wpp::ObjActObserver, public wpp::InstEventObserver {
public:
    FirmwareUpdateImpl();
    ~FirmwareUpdateImpl();

    void init(Object &obj);

    FirmwareUpdate::UpdRes getLastUpdResult();
    void objectRestore(Object &object) override;
	void instEvent(Instance &inst, EVENT_ID_T eventId) override;
    void fwIsDownloaded();
    void update(Instance& inst);
    void saveToFile(STRING_T fileName, const OPAQUE_T *buff);

private:
    FwDownloaderStub _downloader;
    DeviceImpl &_device;
};

#endif // FIRMWARE_UPDATE_H_