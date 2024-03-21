#ifndef FW_URI_DOWNLOADER_H
#define FW_URI_DOWNLOADER_H

#include <iostream>
#include "WppRegistry.h"

#include "FwDownloaderCoap.h"
#include "FwDownloaderHttp.h"

using namespace wpp;
using namespace std;

// TODO: Find other way to share credentials with the COAP downloader
const char psk_id[] = "SINAI_TEST_DEV_ID_VS";
const uint8_t psk_key[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44};

class FwUriDownloader : public FwExternalDl {
public:
    std::vector<FwUpdProtocol> supportedProtocols() override {
        cout << "FwUriDownloader::supportedProtocols" << endl;
        return {FwUpdProtocol::HTTP, FwUpdProtocol::HTTPS, FwUpdProtocol::COAP, FwUpdProtocol::COAPS};
    }

    void startDownloading(const STRING_T &uri, Lwm2mSecurity &security) override {
        cout << "FwUriDownloader::startDownloading, uri: " << uri << endl;

        auto downloadedClb = [this](string file) { 
            cout << "FwUriDownloader FW is downloaded to file: " << file << endl;
            _isDownloaded = true;
            _downloadResult = R_INITIAL;
        };

        if (isHttpScheme(uri) || isHttpsScheme(uri)) {
            _httpDownloader.startDownloading(uri, downloadedClb);
        } else if (isCoapScheme(uri)) {
            _coapDownloader.startDownloading(uri, downloadedClb);
        } else if (isCoapsScheme(uri)) {
            // TODO use Lwm2mSecurity to get the credentials
            _coapDownloader.startDownloading(uri, string(psk_id), vector<uint8_t>(psk_key, psk_key + sizeof(psk_key)), downloadedClb);
        }
    }

    bool isDownloaded() override {
        cout << "FwUriDownloader::isDownloaded " << _isDownloaded << endl;
        return _isDownloaded;
    }

    FwUpdRes downloadResult() override {
        cout << "FwUriDownloader::downloadResult " << _downloadResult << endl;
        return _downloadResult;
    }

    void reset() override {
        cout << "FwUriDownloader::reset" << endl;
        _isDownloaded = false;
        _downloadResult = R_INITIAL;
    }

private:
    bool isHttpScheme(const STRING_T &uri) {
        return uri.find("http://") == 0;
    }

    bool isHttpsScheme(const STRING_T &uri) {
        return uri.find("https://") == 0;
    }

    bool isCoapScheme(const STRING_T &uri) {
        return uri.find("coap://") == 0;
    }

    bool isCoapsScheme(const STRING_T &uri) {
        return uri.find("coaps://") == 0;
    }

private:
    bool _isDownloaded = false;
    FwUpdRes _downloadResult = R_INITIAL;

    FwDownloaderCoap _coapDownloader;
    FwDownloaderHttp _httpDownloader;
};

#endif // FW_URI_DOWNLOADER_H