#ifndef FIRMWARE_UPDATE_H_
#define FIRMWARE_UPDATE_H_

#include <iostream>
#include <fstream>
#include "WppRegistry.h"
#include "ObjActObserver.h"
#include "InstEventObserver.h"
#include "InstBlockOpObserver.h"
#include "WppClient.h"
#include "Device.h"
#include "WppTaskQueue.h"

#include "FwDownloaderStub.h"

using namespace wpp;
using namespace std;

class FirmwareUpdateImpl: public wpp::ObjActObserver, public wpp::InstEventObserver, public wpp::InstBlockOpObserver {
public:
    FirmwareUpdateImpl(DeviceImpl &device);
    ~FirmwareUpdateImpl();

    void init(Object &obj);

private:
    void objectRestore(Object &object) override;
	void instEvent(Instance &inst, EVENT_ID_T eventId) override;
    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    void resourceBlockWrite(Instance &inst, const ResLink &resource, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) override;
    #endif
    
    FirmwareUpdate::UpdRes getLastUpdResult();
    void fwIsDownloaded();
    void update(Instance& inst);
    string getFileName();
    void writeToFile(STRING_T fileName, const OPAQUE_T &buff);

private:
    FwDownloaderStub _downloader;
    DeviceImpl &_device;
};

#endif // FIRMWARE_UPDATE_H_