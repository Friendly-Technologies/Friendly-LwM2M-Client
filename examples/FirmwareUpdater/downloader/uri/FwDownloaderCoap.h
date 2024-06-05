#ifndef FIRMWARE_DOWNLOADER_COAP_H
#define FIRMWARE_DOWNLOADER_COAP_H

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <functional>
#include <coap3/coap.h>

using namespace std;
using namespace wpp;

class FwDownloaderCoap {
    struct DownloadJob {
        string url = "";
        std::string psk_id = "";
        std::vector<uint8_t> psk_key;
        function<void(string, FwUpdRes)> downloadedClb;
        bool downloading = false;
        bool isResourceExists = true;
    };

public:
    FwDownloaderCoap() {
        _terminateThread = false;

        _downloadingThread = new thread([this]() {
            while (!this->_terminateThread) {
                while (!this->_job.downloading && !_terminateThread) this_thread::sleep_for(chrono::milliseconds(100));
                if (_terminateThread) continue;

                // Remove the file if it exists
                std::remove("test_fw.fw");

                this->_jobGuard.lock();
                string url = this->_job.url;
                std::string pskId = this->_job.psk_id;
                std::vector<uint8_t> pskKey = this->_job.psk_key;
                function<void(string, FwUpdRes)> downloadedClb = this->_job.downloadedClb;
                FwUpdRes fwUpdRes = R_INITIAL;
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
                coap_dtls_cpsk_t dtls_psk;
                coap_block_t block;

                block.num = 0;      // Block number
                block.m = 0;        // More blocks to follow (0 for the first block)
                block.szx = 6;      // Block size (2^(szx + 4)), so szx=6 means 1024 bytes

                memset(&dtls_psk, 0, sizeof(dtls_psk));
                dtls_psk.version = COAP_DTLS_CPSK_SETUP_VERSION;
                dtls_psk.psk_info.identity.s = (const uint8_t *)pskId.c_str();
                dtls_psk.psk_info.identity.length = pskId.size();
                dtls_psk.psk_info.key.s = pskKey.data();
                dtls_psk.psk_info.key.length = pskKey.size();

                /* Initialize libcoap library */
                coap_startup();

                coap_set_log_level(COAP_LOG_DEBUG);
                coap_dtls_set_log_level(COAP_LOG_DEBUG);

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

                coap_set_app_data(ctx, &(this->_job.isResourceExists));

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
                    session = coap_new_client_session_psk2(ctx, NULL, &dst, COAP_PROTO_DTLS, &dtls_psk);
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
                coap_register_nack_handler(ctx, nack_handler);

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

                // Add the Block2 option to the request
                coap_add_option(pdu, COAP_OPTION_BLOCK2, coap_encode_var_safe(buf, sizeof(buf), (block.num << 4) | (block.m << 3) | block.szx), buf);

                wait_ms = 180000;
                cout << "sending CoAP request" << endl;
                if (coap_send(session, pdu) == COAP_INVALID_MID) {
                    cout << "cannot send CoAP pdu" << endl;
                    _job.downloading = false;
                    continue;
                }

                while (coap_io_pending(ctx)) { /* i/o not yet complete */
                    if (!_job.isResourceExists) break;
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

                if (!_job.isResourceExists) {
                    cout << "Bad connection or resource does not exist" << endl;
                    fwUpdRes = R_INVALID_URI;
                } else {
                    cout << "Downloading is completed" << endl;
                }
                
                downloadedClb("test_fw.fw", fwUpdRes);
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

	void startDownloading(string url, function<void(string, FwUpdRes)> downloadedClb) {
        // TODO befor starting download check if not already downloading
        _jobGuard.lock();
        _job = {url, "", {}, downloadedClb, true};
        _jobGuard.unlock();
    }

    void startDownloading(string url, std::string pskId, std::vector<uint8_t> pskKey, function<void(string, FwUpdRes)> downloadedClb) {
        // TODO befor starting download check if not already downloading
        _jobGuard.lock();
        _job = {url, pskId, pskKey, downloadedClb, true};
        _jobGuard.unlock();
    }

    bool isDownloading() {
        return _job.downloading && !_terminateThread;
    }

    void cancelDownloading() {
        _job.downloading = false;
    }

private:
    /**
     * Negative Acknowedge handler that is used as callback in coap_context_t.
     */
    static void nack_handler(coap_session_t *session,
                             const coap_pdu_t *sent,
                             const coap_nack_reason_t reason,
                             const coap_mid_t id COAP_UNUSED)
    {
        cout << "Received NACK for message with reason: " << reason << endl;
        coap_context_t *context = coap_session_get_context(session);
        bool* asd = (bool*)coap_get_app_data(context);
        * asd = false;
    }

    /**
     * Response handler that is used as callback in coap_context_t.
     */
    static coap_response_t message_handler(coap_session_t *session,
                                           const coap_pdu_t *sent, 
                                           const coap_pdu_t *received, 
                                           const coap_mid_t id COAP_UNUSED)
    {
        size_t len;
        const uint8_t *databuf;
        size_t offset;
        size_t total;
        coap_pdu_code_t rcv_code = coap_pdu_get_code(received);

        if (rcv_code != COAP_RESPONSE_CODE_CONTENT) {
            cout << "Response code is not 205" << endl;
            coap_context_t *context = coap_session_get_context(session);
            bool* asd = (bool*)coap_get_app_data(context);
            * asd = false;
           
            return COAP_RESPONSE_OK;
        }

        if (COAP_RESPONSE_CLASS(rcv_code) == 2) {
            if (coap_get_data_large(received, &len, &databuf, &offset, &total)) {
                cout << "Received " << len << " bytes" << endl;
                FILE *file = fopen("test_fw.fw", "a");
                if (file != NULL) {
                    size_t written = fwrite(databuf, 1, len, file);
                    if (written != len) {
                        perror("Failed to write all bytes to file");
                    } else {
                        printf("Payload saved to 'test_fw.fw'\n");
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