#ifndef FW_URI_DOWNLOADER_H
#define FW_URI_DOWNLOADER_H

#include <iostream>
#include "WppRegistry.h"

using namespace wpp;
using namespace std;

class FwUriDownloader : public FwExternalDl {
public:
    std::vector<FwUpdProtocol> supportedProtocols() override {
        cout << "FwUriDownloader::supportedProtocols" << endl;
        return {FwUpdProtocol::HTTP, FwUpdProtocol::HTTPS, FwUpdProtocol::COAP, FwUpdProtocol::COAPS};
    }

    void startDownloading(const STRING_T &uri, Lwm2mSecurity &security) override {
        cout << "FwUriDownloader::startDownloading, uri: " << uri << endl;
    }

    bool isDownloaded() override {
        cout << "FwUriDownloader::isDownloaded" << endl;
        return true;
    }

    FwUpdRes downloadResult() override {
        cout << "FwUriDownloader::downloadResult" << endl;
        return R_INITIAL;
    }

    void reset() override {
        cout << "FwUriDownloader::reset" << endl;
    }
};

#endif // FW_URI_DOWNLOADER_H