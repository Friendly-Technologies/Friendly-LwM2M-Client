#include "WppRegistry.h"

#include "WppClient.h"

namespace wpp {

/* ------------- Registry functionality ------------- */
WppRegistry::WppRegistry(const OBJ_RESTORE_T &objRestoreFunc) : _objRestoreFunc(objRestoreFunc) {}

void WppRegistry::restoreObject(OBJ_ID id) {
    if (_objRestoreFunc) _objRestoreFunc(*this, id);
}

/* ------------- Mandatory objects start ------------- */
Object<Security>& WppRegistry::security() {
    if (!Object<Security>::isCreated()) Object<Security>::create(SECURITY_OBJ_INFO);
    return *Object<Security>::object();
}

Object<Server>& WppRegistry::server() {
    if (!Object<Server>::isCreated()) Object<Server>::create(SERVER_OBJ_INFO);
    return *Object<Server>::object();
}

Object<Device>& WppRegistry::device() {
    if (!Object<Device>::isCreated()) Object<Device>::create(DEVICE_OBJ_INFO);
    return *Object<Device>::object();
}
/* ------------- Mandatory objects end ------------- */

/* ------------- Optional objects start ------------- */
#if ACL_OBJ
#endif

#if CONN_MONITORING_OBJ
#endif

#if FIRMWARE_UPD_OBJ
#endif
/* ------------- Optional objects end ------------- */


} //wpp