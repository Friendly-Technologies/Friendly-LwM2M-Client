#ifndef FIRMWARE_DOWNLOADER_STUB_H
#define FIRMWARE_DOWNLOADER_STUB_H

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <functional>

using namespace std;

class FwDownloaderStub {
    struct DownloadJob {
        string uri = "";
        function<void(string)> downloadedClb;
        bool downloading = false;
    };

public:
    FwDownloaderStub() {
        _terminateThread = false;

        _downloadingThread = new thread([this]() {
            while (!this->_terminateThread) {
                while (!this->_job.downloading && !_terminateThread) this_thread::sleep_for(chrono::milliseconds(100));
                if (_terminateThread) continue;

                this->_jobGuard.lock();
                string uri = this->_job.uri;
                function<void(string)> downloadedClb = this->_job.downloadedClb;
                this->_jobGuard.unlock();
                cout << "Start downloading from uri: " << uri << endl;
                
                int cnt = 10;
                while (--cnt && this->_job.downloading) {
                    cout << "Downloading..." << endl;
                    this_thread::sleep_for(chrono::seconds(1));
                }

                if (!this->_job.downloading) {
                    cout << "Downloading is canceled" << endl;
                    continue;
                }
                
                cout << "Downloading is compleated" << endl;
                downloadedClb("Downloaded file name");
                _job.downloading = false;
            }
            cout << "Downloading thread is terminated" << endl;
        });
    }

    ~FwDownloaderStub() {
        _terminateThread = true;
        cancelDownloading();
        if (_downloadingThread) {
            if (_downloadingThread->joinable()) {
                _downloadingThread->join();
            }
            delete _downloadingThread;
        }
    }

	void startDownloading(string uri, function<void(string)> downloadedClb) {
        // TODO befor starting download check if not already downloading
        _jobGuard.lock();
        _job = {uri, downloadedClb, true};
        _jobGuard.unlock();
    }

    bool isDownloading() {
        return _job.downloading && !_terminateThread;
    }

    void cancelDownloading() {
        _job.downloading = false;
    }

private:
    bool _terminateThread;
    thread *_downloadingThread;

    mutex _jobGuard;
    DownloadJob _job;
};

#endif // FIRMWARE_DOWNLOADER_STUB_H