/*
 * WppManager.h
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#ifndef WPPCORE_H_
#define WPPCORE_H_

#include <string>
#include <functional>

#include "WppRegistry.h"
#include "Lwm2mObject.h"

#include <dep.h>

namespace wpp {

class WppCore {
public:
	/*
	 * Keep in mind that while std::function itself is always copy able,
	 * it might hold a callable object (like a lambda) that captures
	 * variables which may not be copy able. If you try to copy a
	 * std::function that holds a non-copyable callable, it will compile,
	 * but will throw a std::bad_function_call exception at runtime if
	 * you try to call the copied std::function.
	 */
	using OBJ_RESTORE_T = std::function<void(Lwm2mObject &)>;

private:
	WppCore(const std::string &endpointName, const std::string &msisdn, const std::string &altPath, const OBJ_RESTORE_T &restoreFunc);

public:
	~WppCore();

	/* ------------- WppCore management ------------- */
	static bool create(const std::string &endpointName, const std::string &msisdn, const std::string &altPath, const OBJ_RESTORE_T &restoreFunc);
	static bool isCreated();
	static WppCore* instance();

	/* ------------- Wakaama core state processing ------------- */
	lwm2m_context_t * getContext();

	lwm2m_client_state_t getState();
	void loop(time_t &sleepTime);

	bool updateRegistration(Server &server, bool withObjects);
	bool updateRegistration(bool withObjects);
	void deregister();

	/* ------------- Wakaama core object managing ------------- */
	bool registerObject(Lwm2mObject &object);
	bool deregisterObject(Lwm2mObject &object);
	bool isObjectRegistered(Lwm2mObject &object);

	void restoreObject(Lwm2mObject &object);

	/* ------------- Wakaama core observer notify ------------- */
	void notifyValueChanged(const DataID &data);

private:
	/* ------------- Wakaama core initialisation ------------- */
	bool lwm2mContextOpen();
	void lwm2mContextClose();
	bool lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath);

private:
	static WppCore *_manager;

	lwm2m_context_t *_lwm2m_context;
	OBJ_RESTORE_T _restoreFunc;
};

} /* namespace wpp */

#endif /* WPPCORE_H_ */
