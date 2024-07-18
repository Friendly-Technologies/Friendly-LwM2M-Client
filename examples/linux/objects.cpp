#include "objects.h"
#include <iostream>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

/* ------------- Internal helpful states and methods ------------- */

static bool _rebootDevice = false;

#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
static void getIpAddress(string* ip) {
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    // retrieve the current interfaces - returns 0 on success
    if (getifaddrs(&interfaces) == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            if (temp_addr->ifa_addr->sa_family == AF_INET && strcmp(temp_addr->ifa_name, "en0")) {
                *ip = inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    freeifaddrs(interfaces);  // free memory
}
#endif // OBJ_O_4_CONNECTIVITY_MONITORING

/* ------------- Methods to init objects ------------- */

void serverInit(WppClient &client) {
    client.registry().registerObj(Lwm2mServer::object(client));
    Instance *server = Lwm2mServer::createInst(client);
	
	server->set<INT_T>(Lwm2mServer::SHORT_SERVER_ID_0, TEST_SERVER_SHORT_ID);
	server->set<STRING_T>(Lwm2mServer::BINDING_7, WPP_BINDING_UDP);
	server->set<TIME_T>(Lwm2mServer::LIFETIME_1, 25);
	server->set<BOOL_T>(Lwm2mServer::NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, false);

	#if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::create(Lwm2mServer::object(client), Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::create(*server, TEST_SERVER_SHORT_ID);
	#endif
}

void securityInit(WppClient &client) {
    client.registry().registerObj(Lwm2mSecurity::object(client));
    wpp::Instance *security = Lwm2mSecurity::createInst(client);
    string url = "coaps://qa62.friendly-tech.com:"; //"coaps://leshan.eclipseprojects.io:"

    // PSK key: 00112233445566778899998877665544
    // RPK public.pem: 3059301306072a8648ce3d020106082a8648ce3d03010703420004bada5475344ba22961a7d965ac518e73481a5f77832bd996c2fa3527e8f3c4248dda621fa9c1348d1365c357357c54869477e387fd2c2675b1c6f28aa506677b
    // RPK private.pem: 92045322a5b34562e1ffec4bcdcc257b9ecfc3478bfaea4b6b0731350202ef2d

	#ifdef LWM2M_BOOTSTRAP
        security->set<BOOL_T>(Lwm2mSecurity::BOOTSTRAP_SERVER_1, true);
        security->set<INT_T>(Lwm2mSecurity::CLIENT_HOLD_OFF_TIME_11, 10);
        #if DTLS_WITH_PSK
        string pskId = "FRIENDLY_TEST_DEV_ID";
        security->set<INT_T>(Lwm2mSecurity::SECURITY_MODE_2, LWM2M_SECURITY_MODE_PRE_SHARED_KEY);
        security->set(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3, OPAQUE_T(pskId.begin(), pskId.end()));
        security->set(Lwm2mSecurity::SECRET_KEY_5, OPAQUE_T {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44});
        url += "5681";
        #elif DTLS_WITH_RPK
        security->set<INT_T>(Lwm2mSecurity::SECURITY_MODE_2, LWM2M_SECURITY_MODE_RAW_PUBLIC_KEY);
        security->set(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3, OPAQUE_T {0x04, 0xba, 0xda, 0x54, 0x75, 0x34, 0x4b, 0xa2, 0x29, 0x61, 0xa7, 0xd9, 0x65, 0xac, 0x51, 0x8e, 0x73, 0x48, 0x1a, 0x5f, 0x77, 0x83, 0x2b, 0xd9, 0x96, 0xc2, 0xfa, 0x35, 0x27, 0xe8, 0xf3, 0xc4, 0x24, 0x8d, 0xda, 0x62, 0x1f, 0xa9, 0xc1, 0x34, 0x8d, 0x13, 0x65, 0xc3, 0x57, 0x35, 0x7c, 0x54, 0x86, 0x94, 0x77, 0xe3, 0x87, 0xfd, 0x2c, 0x26, 0x75, 0xb1, 0xc6, 0xf2, 0x8a, 0xa5, 0x06, 0x67, 0x7b});
        security->set(Lwm2mSecurity::SECRET_KEY_5, OPAQUE_T {0x92, 0x04, 0x53, 0x22, 0xa5, 0xb3, 0x45, 0x62, 0xe1, 0xff, 0xec, 0x4b, 0xcd, 0xcc, 0x25, 0x7b, 0x9e, 0xcf, 0xc3, 0x47, 0x8b, 0xfa, 0xea, 0x4b, 0x6b, 0x07, 0x31, 0x35, 0x02, 0x02, 0xef, 0x2d});
        url += "5682";
        #else
        url += "5680";
        #endif
    #else
        #if DTLS_WITH_PSK
            url += "5684";
            string pskId = "FRIENDLY_TEST_DEV_ID";
            security->set<INT_T>(Lwm2mSecurity::SECURITY_MODE_2, LWM2M_SECURITY_MODE_PRE_SHARED_KEY);
            security->set(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3, OPAQUE_T(pskId.begin(), pskId.end()));
            security->set(Lwm2mSecurity::SECRET_KEY_5, OPAQUE_T {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44});
        #elif DTLS_WITH_RPK
            url += "5684";
            security->set<INT_T>(Lwm2mSecurity::SECURITY_MODE_2, LWM2M_SECURITY_MODE_RAW_PUBLIC_KEY);
            security->set(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3, OPAQUE_T {0x04, 0xba, 0xda, 0x54, 0x75, 0x34, 0x4b, 0xa2, 0x29, 0x61, 0xa7, 0xd9, 0x65, 0xac, 0x51, 0x8e, 0x73, 0x48, 0x1a, 0x5f, 0x77, 0x83, 0x2b, 0xd9, 0x96, 0xc2, 0xfa, 0x35, 0x27, 0xe8, 0xf3, 0xc4, 0x24, 0x8d, 0xda, 0x62, 0x1f, 0xa9, 0xc1, 0x34, 0x8d, 0x13, 0x65, 0xc3, 0x57, 0x35, 0x7c, 0x54, 0x86, 0x94, 0x77, 0xe3, 0x87, 0xfd, 0x2c, 0x26, 0x75, 0xb1, 0xc6, 0xf2, 0x8a, 0xa5, 0x06, 0x67, 0x7b});
            security->set(Lwm2mSecurity::SECRET_KEY_5, OPAQUE_T {0x92, 0x04, 0x53, 0x22, 0xa5, 0xb3, 0x45, 0x62, 0xe1, 0xff, 0xec, 0x4b, 0xcd, 0xcc, 0x25, 0x7b, 0x9e, 0xcf, 0xc3, 0x47, 0x8b, 0xfa, 0xea, 0x4b, 0x6b, 0x07, 0x31, 0x35, 0x02, 0x02, 0xef, 0x2d});
        #else
            url += "5683";
            security->set<INT_T>(Lwm2mSecurity::SECURITY_MODE_2, LWM2M_SECURITY_MODE_NONE);
        #endif
        security->set<BOOL_T>(Lwm2mSecurity::BOOTSTRAP_SERVER_1, false);
    #endif

    security->set<STRING_T>(Lwm2mSecurity::LWM2M_SERVER_URI_0, url);
    security->set<INT_T>(Lwm2mSecurity::SHORT_SERVER_ID_10, TEST_SERVER_SHORT_ID);
}

void deviceInit(WppClient &client) {
    client.registry().registerObj(Device::object(client));
	wpp::Instance *device = Device::createInst(client);

    device->set<EXECUTE_T>(Device::REBOOT_4, [](Instance& inst, ID_T resId, const OPAQUE_T& data) {
    	cout << "Device: execute REBOOT_4" << endl;
    	_rebootDevice = true;
        return true;
    });
    device->set<INT_T>(Device::ERROR_CODE_11, 0, Device::NO_ERROR);
    device->set<STRING_T>(Device::SUPPORTED_BINDING_AND_MODES_16, WPP_BINDING_UDP);
    device->set<STRING_T>(Device::MANUFACTURER_0, "Friendly");
    device->set<STRING_T>(Device::MODEL_NUMBER_1, "Lightweight M2M Client");
    device->set<STRING_T>(Device::SERIAL_NUMBER_2, "0123456789");

    #if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::create(Device::object(client), Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::create(*device, TEST_SERVER_SHORT_ID);
	#endif
}

#ifdef OBJ_O_5_FIRMWARE_UPDATE
void fwUpdaterInit(WppClient &client) {
    #if RES_5_8
    static FwUriDownloader fwUriDownloader;
	#endif
    static FwAutoDownloader fwAutoDownloader;
	static FirmwareUpdater fwUpdater;

    client.registry().registerObj(FirmwareUpdate::object(client));
    FirmwareUpdate::createInst(client);

    FirmwareUpdate::setFwUpdater(client, fwUpdater);
    FirmwareUpdate::setFwInternalDownloader(client, fwAutoDownloader);
    #if RES_5_8
    FirmwareUpdate::setFwExternalDownloader(client, fwUriDownloader);
    #endif

    #if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::create(FirmwareUpdate::object(client), Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::create(*FirmwareUpdate::instance(client), TEST_SERVER_SHORT_ID);
	#endif
}
#endif

#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
void acInit(WppClient &client) {
    client.registry().registerObj(Lwm2mAccessControl::object(client));
}
#endif

#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
void connMonitoringInit(WppClient &client) {
    client.registry().registerObj(ConnectivityMonitoring::object(client));
    Instance *connMon = ConnectivityMonitoring::createInst(client);

    STRING_T ip;
    getIpAddress(&ip);
	connMon->set<INT_T>(ConnectivityMonitoring::NETWORK_BEARER_0, ConnectivityMonitoring::NtwrkBrr::ETHERNET);
	connMon->set<INT_T>(ConnectivityMonitoring::RADIO_SIGNAL_STRENGTH_2, -20);
    connMon->set<STRING_T>(ConnectivityMonitoring::IP_ADDRESSES_4, 0, ip);

    #if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::create(ConnectivityMonitoring::object(client), Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::create(*connMon, TEST_SERVER_SHORT_ID);
	#endif
}
#endif

#ifdef OBJ_O_3339_AUDIO_CLIP
void audioClipInit(WppClient &client) {
    client.registry().registerObj(AudioClip::object(client));
	AudioClip::createInst(client);
	#if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::create(AudioClip::object(client), Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::create(*AudioClip::instance(client), TEST_SERVER_SHORT_ID);
	#endif
}
#endif

/* ------------- Helpful methods ------------- */

bool isDeviceShouldBeRebooted() {
    return _rebootDevice;
}