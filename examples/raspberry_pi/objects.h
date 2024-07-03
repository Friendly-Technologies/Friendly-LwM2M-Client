#ifndef USER_OBJECTS_H
#define USER_OBJECTS_H

#include "WppClient.h"
#if OBJ_O_5_FIRMWARE_UPDATE
#include "FwUriDownloader.h"
#include "FwAutoDownloader.h"
#include "FirmwareUpdater.h"
#endif

using namespace wpp;

#define TEST_SERVER_SHORT_ID 123

/* ------------- Methods to init objects ------------- */
void serverInit(WppClient &client);

void securityInit(WppClient &client);

void deviceInit(WppClient &client);

#ifdef OBJ_O_5_FIRMWARE_UPDATE
void fwUpdaterInit(WppClient &client);
#endif

#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
void acInit(WppClient &client);
#endif

#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
void connMonitoringInit(WppClient &client);
#endif

#ifdef OBJ_O_3339_AUDIO_CLIP
void audioClipInit(WppClient &client);
#endif

void digitalOutputInit(WppClient &client);

void pushButtonInit(WppClient &client);
void buttonLoop(WppClient &client);

void mki217Init(WppClient &client);
void mki217Loop(WppClient &client);

/* ------------- Helpful methods ------------- */
bool isDeviceShouldBeRebooted();

#endif //USER_OBJECTS_H