#ifndef FIRMWARE_DOWNLOADER_HTTP_H
#define FIRMWARE_DOWNLOADER_HTTP_H

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <functional>
#include <curl/curl.h>

using namespace std;

class FwDownloaderHttp {
    struct DownloadJob {
        string url = "";
        function<void(string)> downloadedClb;
        bool downloading = false;
    };

public:
    FwDownloaderHttp() {
        _terminateThread = false;

        _downloadingThread = new thread([this]() {
            while (!this->_terminateThread) {
                while (!this->_job.downloading && !_terminateThread) this_thread::sleep_for(chrono::milliseconds(100));
                if (_terminateThread) continue;

                this->_jobGuard.lock();
                string url = this->_job.url;
                function<void(string)> downloadedClb = this->_job.downloadedClb;
                this->_jobGuard.unlock();
                string file = "test_http.fw";
                cout << "Start downloading from url: " << url << endl;
                
                CURL *curl;
                FILE *fp;
                CURLcode res;

                curl = curl_easy_init();
                if (curl) {
                    fp = fopen(file.c_str(), "wb");
                    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void *ptr, size_t size, size_t nmemb, FILE *stream) {
                        size_t written = fwrite(ptr, size, nmemb, stream);
                        return written;
                    });
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                    // To allow for insecure connections
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    
                    res = curl_easy_perform(curl);

                    /* Check for errors */
                    if (res != CURLE_OK) {
                        cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
                    }

                    /* Cleanup */
                    fclose(fp);
                    curl_easy_cleanup(curl);
                } else {
                    cout << "curl_easy_init() failed" << endl;
                }
                
                cout << "Downloading is compleated" << endl;
                downloadedClb("test_http.fw");
                _job.downloading = false;
            }
            cout << "Downloading thread is terminated" << endl;
        });
    }

    ~FwDownloaderHttp() {
        _terminateThread = true;
        cancelDownloading();
        if (_downloadingThread) {
            if (_downloadingThread->joinable()) {
                _downloadingThread->join();
            }
            delete _downloadingThread;
        }
    }

	void startDownloading(string url, function<void(string)> downloadedClb) {
        // TODO befor starting download check if not already downloading
        _jobGuard.lock();
        _job = {url, downloadedClb, true};
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

#endif // FIRMWARE_DOWNLOADER_HTTP_H