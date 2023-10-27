#include "WppRegistry.h"
#include "WppLogs.h"
#include "liblwm2m.h"

namespace wpp {

WppRegistry::WppRegistry(lwm2m_context_t &context): _context(context) {
    WPP_LOGD(TAG_WPP_REG, "Creating registry instance");
}

bool WppRegistry::registerObj(Lwm2mObject &object) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Register object with id: %d", object.getObjectID());
	return !lwm2m_add_object(&_context, &object.getLwm2mObject());
}

bool WppRegistry::deregisterObj(Lwm2mObject &object) {
	WPP_LOGD_ARG(TAG_WPP_CLIENT, "Deregister object with id: %d", object.getObjectID());
	return !lwm2m_remove_object(&_context, object.getLwm2mObject().objID);
}

bool WppRegistry::isObjRegistered(Lwm2mObject &object) {
	lwm2m_object_t * lwm2m_object = (lwm2m_object_t *)LWM2M_LIST_FIND(_context.objectList, object.getLwm2mObject().objID);
	return lwm2m_object != NULL;
}

/* The start of the prototypes of the mandatory objects. */
# if OBJ_M_3_DEVICE_V12
Object <Device> & WppRegistry::device() {
	if (!Object <Device>::isCreated()) Object <Device>::create(_context, DEVICE_OBJ_INFO);
	return *Object <Device>::object();
}
# endif
# if OBJ_M_0_LWM2M_SECURITY_V11
Object<Lwm2mSecurity> & WppRegistry::lwm2mSecurity() {
	if (!Object<Lwm2mSecurity>::isCreated()) Object<Lwm2mSecurity>::create(_context, LWM2M_SECURITY_OBJ_INFO);
	return *Object<Lwm2mSecurity>::object();
}
# endif
# if OBJ_M_1_LWM2M_SERVER_V11
Object<Lwm2mServer> & WppRegistry::lwm2mServer() {
	if (!Object<Lwm2mServer>::isCreated()) Object<Lwm2mServer>::create(_context, LWM2M_SERVER_OBJ_INFO);
	return *Object<Lwm2mServer>::object();
}
# endif
/* The end of the prototypes of the mandatory objects. */
/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE! */

/* The start of the prototypes of the optional objects. */
#if OPTIONAL_ACL_OBJ
#endif

#if OPTIONAL_CONN_MONITORING_OBJ
#endif

#if OPTIONAL_FIRMWARE_UPD_OBJ
#endif
/* The end of the prototypes of the optional objects. */
/* !!! DO NOT DELETE OR CHANGE THE COMMENT ABOVE! */


} //wpp
