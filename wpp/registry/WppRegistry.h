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

/* ---------- Mandatory objects include block begin ---------- */
#if OBJ_M_3_DEVICE_V12
#include "m_3_device_v12/Device.h"
#endif
#if OBJ_M_1_LWM2M_SERVER_V11
#include "m_1_lwm2m_server_v11/Lwm2mServer.h"
#endif
#if OBJ_M_0_LWM2M_SECURITY_V11
#include "m_0_lwm2m_security_v11/Lwm2mSecurity.h"
#endif
/* ---------- Mandatory objects include block end ---------- */

/* ---------- Optional objects include block begin ---------- */
#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
#include "o_4_connectivity_monitoring_v13/ConnectivityMonitoring.h"
#endif
#if OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
#include "o_2_lwm2m_access_control_v11/Lwm2mAccessControl.h"
#endif
#if OBJ_O_5_FIRMWARE_UPDATE_V11
#include "o_5_firmware_update_v11/FirmwareUpdate.h"
#endif
/* ---------- Optional objects include block end ---------- */

namespace wpp {

// TODO: Split mandatory and optional registers
/**
 * @brief The WppRegistry class represents a registry for managing LWM2M objects.
 * 
 * This class provides functionality to register, deregister, and access LWM2M objects.
 * It also allows checking if an object is registered or exists in the registry.
 * The registry can contain both mandatory and optional objects.
 * 
 * @note This class is not copyable or movable.
 */
class WppRegistry {
public:
	WppRegistry(lwm2m_context_t &context);
	~WppRegistry();

	WppRegistry(const WppRegistry&) = delete;
	WppRegistry(WppRegistry&&) = delete;
	WppRegistry& operator=(const WppRegistry&) = delete;
	WppRegistry& operator=(WppRegistry&&) = delete;

	/**
	 * @brief Registers an Object in the registry.
	 *
	 * @param object The Object to register.
	 * @return True if the registration is successful, false otherwise.
	 */
	bool registerObj(Object &object);

	/**
	 * @brief Deregisters an Object from the registry.
	 *
	 * @param object The Object to deregister.
	 * @return True if the deregistration is successful, false otherwise.
	 */
	bool deregisterObj(Object &object);

	/**
	 * @brief Checks if an Object is registered in the registry.
	 *
	 * @param object The Object to check.
	 * @return True if the Object is registered, false otherwise.
	 */
	bool isObjRegistered(Object &object);

	/**
	 * @brief Checks if an Object with the given objId exists in the registry.
	 *
	 * @param objId The ID of the Object to check.
	 * @return True if the Object exists, false otherwise.
	 */
	bool isObjExist(OBJ_ID objId);

	/**
	 * @brief Retrieves a pointer to the Object with the given objId.
	 *
	 * @param objId The ID of the Object to retrieve.
	 * @return A pointer to the Object if found, nullptr otherwise.
	 */
	Object *object(OBJ_ID objId);

	/* ---------- Mandatory objects prototype block begin ---------- */
	#if OBJ_M_3_DEVICE_V12
	ObjectImpl<Device> & device();
	#endif
	#if OBJ_M_1_LWM2M_SERVER_V11
	ObjectImpl<Lwm2mServer> & lwm2mServer();
	#endif
	#if OBJ_M_0_LWM2M_SECURITY_V11
	ObjectImpl<Lwm2mSecurity> & lwm2mSecurity();
	#endif
	/* ---------- Mandatory objects prototype block end ---------- */

	/* ---------- Optional objects prototype block begin ---------- */
	#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
	ObjectImpl<ConnectivityMonitoring> & connectivityMonitoring();
	#endif
	#if OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
	ObjectImpl<Lwm2mAccessControl> & lwm2mAccessControl();
	#endif
	#if OBJ_O_5_FIRMWARE_UPDATE_V11
	ObjectImpl<FirmwareUpdate> & firmwareUpdate();
	#endif
	/* ---------- Optional objects prototype block end ---------- */

private:
	lwm2m_context_t &_context;
	std::vector<Object *> _objects;
};

} // namespace wpp

#endif // WPP_REGISTRY_H_
