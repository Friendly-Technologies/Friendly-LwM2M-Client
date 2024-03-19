/*
 * Regestry.h
 *
 *  Created on: 18 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_REGISTRY_H_
#define WPP_REGISTRY_H_

#include <vector>
#include "ObjectSpec.h"

/* ---------- Mandatory objects include block begin ---------- */
#ifdef OBJ_M_3_DEVICE
#include "m_3_device/Device.h"
#endif
#ifdef OBJ_M_1_LWM2M_SERVER
#include "m_1_lwm2m_server/Lwm2mServer.h"
#endif
#ifdef OBJ_M_0_LWM2M_SECURITY
#include "m_0_lwm2m_security/Lwm2mSecurity.h"
#endif
/* ---------- Mandatory objects include block end ---------- */

/* ---------- Optional objects include block begin ---------- */
#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
#include "o_4_connectivity_monitoring/ConnectivityMonitoring.h"
#endif
#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
#include "o_2_lwm2m_access_control/Lwm2mAccessControl.h"
#endif
#ifdef OBJ_O_3339_AUDIO_CLIP
#include "o_3339_audio_clip/AudioClip.h"
#endif
#ifdef OBJ_O_5_FIRMWARE_UPDATE
#include "o_5_firmware_update/FirmwareUpdate.h"
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
	explicit WppRegistry(lwm2m_context_t &context);
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
	#ifdef OBJ_M_3_DEVICE
	ObjectSpec<Device> & device();
	#endif
	#ifdef OBJ_M_1_LWM2M_SERVER
	ObjectSpec<Lwm2mServer> & lwm2mServer();
	#endif
	#ifdef OBJ_M_0_LWM2M_SECURITY
	ObjectSpec<Lwm2mSecurity> & lwm2mSecurity();
	#endif
	/* ---------- Mandatory objects prototype block end ---------- */

	/* ---------- Optional objects prototype block begin ---------- */
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
	ObjectSpec<ConnectivityMonitoring> & connectivityMonitoring();
	#endif
	#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
	ObjectSpec<Lwm2mAccessControl> & lwm2mAccessControl();
	#endif
	#ifdef OBJ_O_3339_AUDIO_CLIP
	ObjectSpec<AudioClip> & audioClip();
	#endif
	#ifdef OBJ_O_5_FIRMWARE_UPDATE
	ObjectSpec<FirmwareUpdate> & firmwareUpdate();
	#endif
	/* ---------- Optional objects prototype block end ---------- */

private:
	lwm2m_context_t &_context;
	std::vector<Object *> _objects;
};

} // namespace wpp

#endif // WPP_REGISTRY_H_
