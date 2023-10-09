/*
 * WppManager.h
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_CLIENT_H_
#define WPP_CLIENT_H_

#include <string>
#include <functional>
#include <mutex>

#include "liblwm2m.h"
#include "types.h"
#include "WppRegistry.h"
#include "Lwm2mObject.h"
#include "WppConnection.h"

#define WPP_CLIENT_MAX_SLEEP_TIME_S	30

namespace wpp {
/*
 * To interact with the WppClient, it is necessary to take ownership of it,
 * even if control was transferred from the WppClient to the user-registered
 * callback, the user has the right to interact only with the obtained data
 * through parameters.
 */
class WppClient {
public:
	struct ClientInfo {
		std::string endpointName;
		std::string msisdn;
		std::string altPath;
	};

private:
	WppClient(WppConnection &connection, time_t maxSleepTime);

public:
	~WppClient();

	/* ------------- WppClient management ------------- */
	static bool create(const ClientInfo &info, WppConnection &connection, time_t maxSleepTime=WPP_CLIENT_MAX_SLEEP_TIME_S);
	static void remove();
	static bool isCreated();
	
	/*
	 * At the same time, only one actor can have access to the client, this is done
	 * in order to synchronize access to the registry and avoid possible simultaneous
	 * writing/reading/deleting of Instance or Resource.
	 * 
	 * To gain access to the client, the user must call takeOwnership() which
	 * returns a pointer to the client, or NULL if the client is occupied by
	 * another actor or uninitialized. After the client is finished, the user
	 * must call giveOwnership().
	 */
	static WppClient* takeOwnership();
	void giveOwnership();

	/* ------------- WppClient components ------------- */
	WppConnection & connection();
	WppRegistry & registry();

	/* ------------- Wakaama core state processing ------------- */
	lwm2m_client_state_t getState();

	/*
	 * This function does two things:
	 *  - First it does the work needed by wakaama core (eg. (re)sending some packets).
	 *  - Secondly handle received packets from servers.
	 * Return time interval after which it should be called.
	 */
	time_t loop();

	bool updateServerRegistration(INT_T serverId, bool withObjects);
	bool updateServerRegistration(bool withObjects);

	void deregister();

	/* ------------- Wakaama core object managing ------------- */
	bool registerObject(Lwm2mObject &object);
	bool deregisterObject(Lwm2mObject &object);
	bool isObjectRegistered(Lwm2mObject &object);

private:
	/* ------------- Wakaama client initialisation ------------- */
	bool lwm2mContextOpen();
	void lwm2mContextClose();
	lwm2m_context_t & getContext();

	bool lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath);

private:
	static WppClient *_client;
	static std::mutex _clientGuard;

	WppConnection &_connection;
	WppRegistry *_registry;

	time_t _maxSleepTime;

	lwm2m_context_t *_lwm2m_context;
};

} /* namespace wpp */

#endif /* WPP_CLIENT_H_ */
