#ifndef FW_AUTO_DOWNLOADER_H
#define FW_AUTO_DOWNLOADER_H

#include <iostream>
#include "WppRegistry.h"

using namespace wpp;
using namespace std;

class FwAutoDownloader : public FwAutoDl {
public:
    void downloadIsStarted() override {
        cout << "FwAutoDownloader::downloadIsStarted" << endl;
    }

    bool saveDownloadedBlock(const OPAQUE_T &dataBlock) override {
        cout << "FwAutoDownloader::saveDownloadedBlock, size: " << dataBlock.size() << endl;
        return true;
    }

    void downloadIsCompleted() override {
        cout << "FwAutoDownloader::downloadIsCompleted" << endl;
    }

    FwUpdRes downloadResult() override {
        cout << "FwAutoDownloader::downloadResult" << endl;
        return R_INITIAL;
    }

    void reset() override {
        cout << "FwAutoDownloader::reset" << endl;
    }
};

#endif // FW_AUTO_DOWNLOADER_H
