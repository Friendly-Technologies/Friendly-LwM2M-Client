#ifndef FW_URI_DOWNLOADER_H
#define FW_URI_DOWNLOADER_H

#include <iostream>
#include "WppRegistry.h"

#include "FwDownloaderCoap.h"
#include "FwDownloaderHttp.h"

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
            OPAQUE_T psk_id, psk_key;
            security.get(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3, psk_id);
            security.get(Lwm2mSecurity::SECRET_KEY_5, psk_key);
            _coapDownloader.startDownloading(uri, string(psk_id.begin(), psk_id.end()), psk_key, downloadedClb);
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

    STRING_T read_metadata(uint8_t line_num) {
        ifstream is;
        string str = "default";
    
        cout << "FwUpdateImpl: read_metadata" << endl;

        is.open("test_http.fw", ios::binary);
        if (is.is_open()) {
            // if file can't be opened it means it's not exists. Will return "default"
            for (uint8_t i = 0; i < line_num; i++) {
                getline(is, str);
                // TODO: split by ":=" str
            }
        }
        is.close();

        return str;
    }

private:
    bool _isDownloaded = false;
    FwUpdRes _downloadResult = R_INITIAL;

    FwDownloaderCoap _coapDownloader;
    FwDownloaderHttp _httpDownloader;
};

#endif // FW_URI_DOWNLOADER_H