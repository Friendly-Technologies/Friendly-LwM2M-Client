#include "WppRegistry.h"

#include "WppClient.h"

namespace wpp {

/* ------------- Registry functionality ------------- */
WppRegistry::WppRegistry(const OBJ_RESTORE_T &objRestoreFunc) : _objRestoreFunc(objRestoreFunc) {}

void WppRegistry::restoreObject(OBJ_ID id) {
    if (_objRestoreFunc) _objRestoreFunc(*this, id);
}

/* ------------- Mandatory objects prototype start ------------- */
#if MANDATORY_SECURITY_OBJ
Object<Security>& WppRegistry::security() {
    if (!Object<Security>::isCreated()) Object<Security>::create(SECURITY_OBJ_INFO);
    return *Object<Security>::object();
}
#endif
#if MANDATORY_SERVER_OBJ
Object<Server>& WppRegistry::server() {
    if (!Object<Server>::isCreated()) Object<Server>::create(SERVER_OBJ_INFO);
    return *Object<Server>::object();
}
#endif
#if MANDATORY_DEVICE_OBJ
Object<Device>& WppRegistry::device() {
    if (!Object<Device>::isCreated()) Object<Device>::create(DEVICE_OBJ_INFO);
    return *Object<Device>::object();
}
#endif
/* ------------- Mandatory objects prototype end ------------- */

/* ------------- Optional objects prototype start ------------- */
#if OPTIONAL_ACL_OBJ
#endif

#if OPTIONAL_CONN_MONITORING_OBJ
#endif

#if OPTIONAL_FIRMWARE_UPD_OBJ
#endif
/* ------------- Optional objects prototype end ------------- */


} //wpp