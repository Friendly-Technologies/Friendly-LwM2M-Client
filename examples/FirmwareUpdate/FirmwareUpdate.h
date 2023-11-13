#ifndef FIRMWARE_UPDATE_H_
#define FIRMWARE_UPDATE_H_

#include "WppRegistry.h"
#include "InstEventObserver.h"


using namespace wpp;

class FirmwareUpdateImpl: public wpp::InstEventObserver {
public:
    FirmwareUpdateImpl();
    ~FirmwareUpdateImpl();

    void init(Object &obj);

    FirmwareUpdate::UpdRes getLastUpdResult();
    void objectRestore(Object &object) override;
    void fwIsDownloaded();
    void update(Instance& inst);
    void saveToFile(STRING_T fileName, const OPAQUE_T *buff);

private:
	void instEvent(Instance &inst, EVENT_ID_T eventId) override;
    FwDownloaderStub _downloader;
    DeviceImpl &_device;
};

#endif // FIRMWARE_UPDATE_H_