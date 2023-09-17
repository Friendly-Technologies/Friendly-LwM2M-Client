/*
 * Regestry.h
 *
 *  Created on: 18 Jul 2023
 *      Author: valentin
 */

#ifndef WPPREGISTRY_H_
#define WPPREGISTRY_H_

#include <mutex>

#include "Object.h"

/* ------------- Mandatory objects include start ------------- */
#if MANDATORY_SECURITY_OBJ
#include "mandatory/security/Security.h"
#endif
#if MANDATORY_SERVER_OBJ
#include "mandatory/server/Server.h"
#endif
#if MANDATORY_DEVICE_OBJ
#include "mandatory/device/Device.h"
#endif
/* ------------- Mandatory objects include end ------------- */

/* ------------- Optional objects include start ------------- */
#if OPTIONAL_ACL_OBJ
#include "optional/acl/Acl.h"
#endif
#if OPTIONAL_CONN_MONITORING_OBJ
#include "optional/conn_monitoring/ConnMonitoring.h"
#endif
#if OPTIONAL_FIRMWARE_UPD_OBJ
#include "optional/firmware_upd/FirmwareUpd.h"
#endif
/* ------------- Optional objects include end ------------- */

namespace wpp {

template<typename T>
class Object;

// TODO: Split mandatory and optional registers
// TODO: Add ability to check whether some object id is exist
// TODO: Add ability to get Lwm2mObject by ID
// TODO: Try to redesign the registry and objects to simplify access
class WppRegistry {
public:
	/*
	 * OBJ_RESTORE_T represents callback that notifies user about the requirement of restoring the object state to default
	 *
	 * NOTE: Keep in mind that while std::function itself is always copy able,
	 * it might hold a callable object (like a lambda) that captures
	 * variables which may not be copy able. If you try to copy a
	 * std::function that holds a non-copyable callable, it will compile,
	 * but will throw a std::bad_function_call exception at runtime if
	 * you try to call the copied std::function.
	 */
	using OBJ_RESTORE_T = std::function<void(WppRegistry &, OBJ_ID)>;

public:
	WppRegistry(const OBJ_RESTORE_T &objRestoreFunc);
	~WppRegistry() {}

	WppRegistry(const WppRegistry&) = delete;
	WppRegistry(WppRegistry&&) = delete;
	WppRegistry& operator=(const WppRegistry&) = delete;
	WppRegistry& operator=(WppRegistry&&) = delete;

	/* ------------- Registry functionality ------------- */
	void restoreObject(OBJ_ID id);

	/* ------------- Mandatory objects prototype start ------------- */
	#if MANDATORY_SECURITY_OBJ
	Object<Security>& security();
	#endif
	#if MANDATORY_SERVER_OBJ
	Object<Server>& server();
	#endif
	#if MANDATORY_DEVICE_OBJ
	Object<Device>& device();
	#endif
	/* ------------- Mandatory objects prototype end ------------- */

	/* ------------- Optional objects prototype start ------------- */
	#if OPTIONAL_ACL_OBJ
	Object<Acl>& acl();
	#endif
	#if OPTIONAL_CONN_MONITORING_OBJ
	Object<ConnMonitoring>& connMonitoring();
	#endif
	#if OPTIONAL_FIRMWARE_UPD_OBJ
		Object<FirmwareUpd>& firmwareUpd();
	#endif
	/* ------------- Optional objects prototype end ------------- */

private:
	OBJ_RESTORE_T _objRestoreFunc;
};

} // namespace wpp

#endif // WPPREGISTRY_H_
