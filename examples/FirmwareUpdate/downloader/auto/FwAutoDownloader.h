#ifndef FW_AUTO_DOWNLOADER_H
#define FW_AUTO_DOWNLOADER_H

#include <iostream>
#include "WppRegistry.h"
#include "FirmwareChecker.h"

using namespace wpp;
using namespace std;

class FwAutoDownloader : public FwInternalDl {
public:
    void downloadIsStarted() override {
        cout << "FwAutoDownloader::downloadIsStarted()" << endl;
    }

    bool saveDownloadedBlock(const OPAQUE_T &dataBlock) override {
        cout << "FwAutoDownloader::saveDownloadedBlock(), size: " << dataBlock.size() << endl;
        _fwPackage.insert(_fwPackage.end(), dataBlock.begin(), dataBlock.end());
        return true;
    }

    void downloadIsCompleted() override {
        cout << "FwAutoDownloader::downloadIsCompleted()" << endl;
        remove("test_fw.fw");
        writeToFile("test_fw.fw", _fwPackage);
    }

    FwUpdRes downloadResult() override {
        FwUpdRes res = FirmwareChecker::getFwDownloadRes();
        cout << "FwAutoDownloader::downloadResult(): " << (int)res << endl;
        return res;
    }

    void reset() override {
        cout << "FwAutoDownloader::reset()" << endl;
        _fwPackage.clear();
    }

private:
    void writeToFile(STRING_T fileName, const OPAQUE_T &buff) {
        if (buff.empty()) return;

        ofstream file(fileName, ios::binary | ios::app);
        if (!file.is_open()) return;
        file.write(reinterpret_cast<const char*>(buff.data()), buff.size());
        file.close();
    }

private:
    OPAQUE_T _fwPackage;
};

#endif // FW_AUTO_DOWNLOADER_H
