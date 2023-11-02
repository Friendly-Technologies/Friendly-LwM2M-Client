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

/* The start of the includes of the mandatory objects. */
#if OBJ_M_3_DEVICE_V12
#include "m_3_device_v12/Device.h"
#endif
#if OBJ_M_0_LWM2M_SECURITY_V11
#include "m_0_lwm2m_security_v11/Lwm2mSecurity.h"
#endif
#if OBJ_M_1_LWM2M_SERVER_V11
#include "m_1_lwm2m_server_v11/Lwm2mServer.h"
#endif
/* The end of the includes of the mandatory objects. */
/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!! */

/* The start of the includes of the optional objects. */
#if OPTIONAL_ACL_OBJ
#include "acl/Acl.h"
#endif
#if OPTIONAL_FIRMWARE_UPD_OBJ
#include "firmware_upd/FirmwareUpd.h"
#endif
#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
#include "o_4_connectivity_monitoring_v13/ConnectivityMonitoring.h"
#endif
/* The end of the includes of the optional objects. */
/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!! */

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

	/* The start of the prototypes of the mandatory objects. */
	/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!! */
	#if OBJ_M_3_DEVICE_V12
	ObjectImpl<Device> & device();
	#endif
	#if OBJ_M_0_LWM2M_SECURITY_V11
	ObjectImpl<Lwm2mSecurity> & lwm2mSecurity();
	#endif
	#if OBJ_M_1_LWM2M_SERVER_V11
	ObjectImpl<Lwm2mServer> & lwm2mServer();
	#endif
	/* The end of the prototypes of the mandatory objects. */
	/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!! */

	/* The start of the prototypes of the optional objects. */
	#if OPTIONAL_ACL_OBJ
	ObjectImpl<Acl>& acl();
	#endif
	#if OPTIONAL_FIRMWARE_UPD_OBJ
	ObjectImpl<FirmwareUpd>& firmwareUpd();
	#endif
	#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
	ObjectImpl<ConnectivityMonitoring> & connectivityMonitoring();
	#endif
	/* The end of the prototypes of the optional objects. */
	/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE !!! */

private:
	lwm2m_context_t &_context;
	std::vector<Object *> _objects;
};

} // namespace wpp

#endif // WPP_REGISTRY_H_
