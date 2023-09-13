/*
 * WppManager.h
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#ifndef WppClient_H_
#define WppClient_H_

#include <string>
#include <functional>

#include "WppRegistry.h"
#include "Lwm2mObject.h"
#include "WppPlatformI.h"
#include "WppConnectionI.h"

//TODO: #include "liblwm2m.h"
#include <dep.h>

namespace wpp {

class WppPlatformI;
class WppConnectionI;
 
class WppClient {
public:
	struct ClientInfo {
		std::string endpointName;
		std::string msisdn;
		std::string altPath;
	};

private:
	WppClient(const ClientInfo &info, WppConnectionI &connection, WppPlatformI &platform);

public:
	~WppClient();

	/* ------------- WppClient management ------------- */
	static bool create(const ClientInfo &info, WppConnectionI &connection, WppPlatformI &platform);
	static bool isCreated();
	static WppClient* client();

	/* ------------- WppClient components ------------- */
	WppConnectionI & connection();
	WppPlatformI & platform();

	/* ------------- Wakaama core state processing ------------- */
	lwm2m_client_state_t getState();

	/*
	 * Try to take registry internaly.
	 */
	void loop(time_t &sleepTime);

	bool updateServerRegistration(INT_T serverId, bool withObjects);
	bool updateServerRegistration(bool withObjects);

	void deregister();

	/* ------------- Wakaama core object managing ------------- */
	bool registerObject(Lwm2mObject &object);
	bool deregisterObject(Lwm2mObject &object);
	bool isObjectRegistered(Lwm2mObject &object);

	/* ------------- Wakaama core observer notify ------------- */
	void notifyValueChanged(const DataID &data);

private:
	/* ------------- Wakaama client initialisation ------------- */
	void init();
	bool isInitialized();

	bool lwm2mContextOpen();
	void lwm2mContextClose();
	lwm2m_context_t * getContext();

	bool lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath);

private:
	static WppClient *_client;

	bool _isInit;
	ClientInfo _info;

	WppConnectionI &_connection;
	WppPlatformI &_platform;

	lwm2m_context_t *_lwm2m_context;
};

} /* namespace wpp */

#endif /* WppClient_H_ */
