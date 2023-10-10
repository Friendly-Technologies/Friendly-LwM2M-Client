/*
 * Regestry.h
 *
 *  Created on: 18 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_REGISTRY_H_
#define WPP_REGISTRY_H_

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
// TODO: Split mandatory and optional registers
// TODO: Add ability to check whether some object id is exist
// TODO: Add ability to get Lwm2mObject by ID
// TODO: Try to redesign the registry and objects to simplify access
class WppRegistry {
public:
	WppRegistry(lwm2m_context_t &context);
	~WppRegistry() {}

	WppRegistry(const WppRegistry&) = delete;
	WppRegistry(WppRegistry&&) = delete;
	WppRegistry& operator=(const WppRegistry&) = delete;
	WppRegistry& operator=(WppRegistry&&) = delete;

	bool registerObj(Lwm2mObject &object);
	bool deregisterObj(Lwm2mObject &object);
	bool isObjRegistered(Lwm2mObject &object);

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
	lwm2m_context_t &_context;
};

} // namespace wpp

#endif // WPP_REGISTRY_H_
