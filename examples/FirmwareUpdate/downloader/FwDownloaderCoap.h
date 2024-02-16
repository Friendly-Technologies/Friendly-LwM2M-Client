#ifndef FIRMWARE_DOWNLOADER_COAP_H
#define FIRMWARE_DOWNLOADER_COAP_H

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <functional>
#include <coap3/coap.h>

using namespace std;

class FwDownloaderCoap {
    struct DownloadJob {
        string url = "";
        function<void(string)> downloadedClb;
        bool downloading = false;
    };

public:
    FwDownloaderCoap() {
        _terminateThread = false;

        _downloadingThread = new thread([this]() {
            while (!this->_terminateThread) {
                while (!this->_job.downloading && !_terminateThread) this_thread::sleep_for(chrono::milliseconds(100));
                if (_terminateThread) continue;

                this->_jobGuard.lock();
                string url = this->_job.url;
                function<void(string)> downloadedClb = this->_job.downloadedClb;
                this->_jobGuard.unlock();
                cout << "Start downloading from url: " << url << endl;
                
                coap_context_t  *ctx = NULL;
                coap_session_t *session = NULL;
                coap_address_t dst;
                coap_pdu_t  *pdu;
                coap_addr_info_t *info_list = NULL;
                unsigned int wait_ms = 0;
                unsigned char buf[100];
                coap_uri_t uri;
                coap_optlist_t *optlist = NULL;
                coap_dtls_pki_t dtls_pki;

                memset(&dtls_pki, 0, sizeof(coap_dtls_pki_t));
                dtls_pki.version = COAP_DTLS_PKI_SETUP_VERSION;
                dtls_pki.verify_peer_cert = 0; // Verify the server's certificate
                dtls_pki.pki_key.key_type = COAP_PKI_KEY_PEM; // Using PEM format for keys

                /* Initialize libcoap library */
                coap_startup();

                coap_set_log_level(COAP_LOG_WARN);
                coap_dtls_set_log_level(COAP_LOG_WARN);

                if (coap_split_uri((unsigned char *)url.c_str(), url.size(), &uri) < 0) {
                    cout << "invalid CoAP URI" << endl;
                    _job.downloading = false;
                    continue;
                }

                /* resolve destination address where data should be sent */
                info_list = coap_resolve_address_info(&uri.host, uri.port, uri.port, uri.port, uri.port, 0, 1 << uri.scheme, COAP_RESOLVE_TYPE_REMOTE);
                if (info_list == NULL) {
                    cout << "failed to resolve address" << endl;
                    _job.downloading = false;
                    continue;
                }

                memcpy(&dst, &info_list->addr, sizeof(dst));
                coap_free_address_info(info_list);

                ctx = coap_new_context(NULL);
                if (!ctx) {
                    cout << "cannot create context" << endl;
                    _job.downloading = false;
                    continue;
                }

                /* Add COAP_BLOCK_USE_LIBCOAP | COAP_BLOCK_SINGLE_BODY to receive large responses*/
                coap_context_set_block_mode(ctx, COAP_BLOCK_USE_LIBCOAP);
                if (uri.scheme == COAP_URI_SCHEME_COAP) {
                    session = coap_new_client_session(ctx, NULL, &dst, COAP_PROTO_UDP);
                    if (session == NULL) {
                        coap_log_emerg("cannot create client session for COAP_URI_SCHEME_COAP\n");
                        _job.downloading = false;
                        continue;
                    }
                } else if (uri.scheme == COAP_URI_SCHEME_COAPS) {
                    session = coap_new_client_session_pki(ctx, NULL, &dst, COAP_PROTO_DTLS, &dtls_pki);
                    if (session == NULL) {
                        coap_log_emerg("cannot create client session for COAP_URI_SCHEME_COAPS\n");
                        _job.downloading = false;
                        continue;
                    }
                } else {
                    coap_log_emerg("cannot create client session for unsupported scheme %d\n", uri.scheme);
                    _job.downloading = false;
                    continue;
                }

                coap_register_response_handler(ctx, message_handler);

                /* Convert provided uri into CoAP options */
                if (coap_uri_into_options(&uri, &dst, &optlist, 1, buf, sizeof(buf)) < 0) {
                    cout << "Failed to create options for URI" << endl;
                    _job.downloading = false;
                    continue;
                }

                if (!(pdu = coap_new_pdu(COAP_MESSAGE_CON, COAP_REQUEST_CODE_GET, session))) {
                    _job.downloading = false;
                    continue;
                }

                coap_add_optlist_pdu(pdu, &optlist);
                coap_show_pdu(COAP_LOG_INFO, pdu);

                wait_ms = 90000;
                cout << "sending CoAP request" << endl;
                if (coap_send(session, pdu) == COAP_INVALID_MID) {
                    cout << "cannot send CoAP pdu" << endl;
                    _job.downloading = false;
                    continue;
                }

                while (coap_io_pending(ctx)) { /* i/o not yet complete */
                    uint32_t timeout_ms;
                    int result = -1;
                    timeout_ms = wait_ms;

                    result = coap_io_process(ctx, timeout_ms);

                    if (result >= 0 && wait_ms > 0) {
                        if ((unsigned)result >= wait_ms) {
                            cout << "timeout\n" << endl;
                            break;
                        } else {
                            wait_ms -= result;
                        }
                    }
                }

                /* Clean up library usage */
                coap_session_release(session);
                coap_free_context(ctx);
                coap_cleanup();
                coap_delete_optlist(optlist);
                
                cout << "Downloading is compleated" << endl;
                downloadedClb("test_coap.fw");
                _job.downloading = false;
            }
            cout << "Downloading thread is terminated" << endl;
        });
    }

    ~FwDownloaderCoap() {
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
    static coap_response_t message_handler(coap_session_t *session COAP_UNUSED, const coap_pdu_t *sent, const coap_pdu_t *received, const coap_mid_t id COAP_UNUSED) {
        size_t len;
        const uint8_t *databuf;
        size_t offset;
        size_t total;
        coap_pdu_code_t rcv_code = coap_pdu_get_code(received);

        if (COAP_RESPONSE_CLASS(rcv_code) == 2) {
            if (coap_get_data_large(received, &len, &databuf, &offset, &total)) {
                cout << "Received " << len << " bytes" << endl;
                FILE *file = fopen("test_coap.fw", "a");
                if (file != NULL) {
                    size_t written = fwrite(databuf, 1, len, file);
                    if (written != len) {
                        perror("Failed to write all bytes to file");
                    } else {
                        printf("Payload saved to 'test_coap.fw'\n");
                    }
                    fclose(file);
                } else {
                    perror("Failed to open file");
                }
            }

        }

        return COAP_RESPONSE_OK;
    }

private:
    bool _terminateThread;
    thread *_downloadingThread;

    mutex _jobGuard;
    DownloadJob _job;
};

#endif // FIRMWARE_DOWNLOADER_COAP_H