/*
 * Regestry.h
 *
 *  Created on: 18 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_REGISTRY_H_
#define WPP_REGISTRY_H_

#include <mutex>
#include <vector>

#include "ObjectImpl.h"

/* ---------- Mandatory objects include blok begin ---------- */
#if OBJ_M_3_DEVICE_V12
#include "m_3_device_v12/Device.h"
#endif
#if OBJ_M_1_LWM2M_SERVER_V11
#include "m_1_lwm2m_server_v11/Lwm2mServer.h"
#endif
#if OBJ_M_0_LWM2M_SECURITY_V11
#include "m_0_lwm2m_security_v11/Lwm2mSecurity.h"
#endif
/* ---------- Mandatory objects include blok end ---------- */

/* ---------- Optional objects include blok begin ---------- */
#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
#include "o_4_connectivity_monitoring_v13/ConnectivityMonitoring.h"
#endif
#if OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
#include "o_2_lwm2m_access_control_v11/Lwm2mAccessControl.h"
#endif
#if OBJ_O_5_FIRMWARE_UPDATE_V12
#include "o_5_firmware_update_v12/FirmwareUpdate.h"
#endif
/* ---------- Optional objects include blok end ---------- */

namespace wpp {
// TODO: Split mandatory and optional registers
class WppRegistry {
public:
	WppRegistry(lwm2m_context_t &context);
	~WppRegistry();

	WppRegistry(const WppRegistry&) = delete;
	WppRegistry(WppRegistry&&) = delete;
	WppRegistry& operator=(const WppRegistry&) = delete;
	WppRegistry& operator=(WppRegistry&&) = delete;

	bool registerObj(Object &object);
	bool deregisterObj(Object &object);
	bool isObjRegistered(Object &object);
	bool isObjExist(OBJ_ID objId);

	Object *object(OBJ_ID objId);

	/* ---------- Mandatory objects prototype blok begin ---------- */
	#if OBJ_M_3_DEVICE_V12
	ObjectImpl<Device> & device();
	#endif
	#if OBJ_M_1_LWM2M_SERVER_V11
	ObjectImpl<Lwm2mServer> & lwm2mServer();
	#endif
	#if OBJ_M_0_LWM2M_SECURITY_V11
	ObjectImpl<Lwm2mSecurity> & lwm2mSecurity();
	#endif
	/* ---------- Mandatory objects prototype blok end ---------- */

	/* ---------- Optional objects prototype blok begin ---------- */
	#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
	ObjectImpl<ConnectivityMonitoring> & connectivityMonitoring();
	#endif
	#if OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
	ObjectImpl<Lwm2mAccessControl> & lwm2mAccessControl();
	#endif
	#if OBJ_O_5_FIRMWARE_UPDATE_V12
	ObjectImpl<FirmwareUpdate> & firmwareUpdate();
	#endif
	/* ---------- Optional objects prototype blok end ---------- */

private:
	lwm2m_context_t &_context;
	std::vector<Object *> _objects;
};

} // namespace wpp

#endif // WPP_REGISTRY_H_
