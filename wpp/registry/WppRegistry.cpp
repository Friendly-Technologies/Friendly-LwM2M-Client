#include "WppRegistry.h"
#include "WppClient.h"
#include "WppLogs.h"

namespace wpp {

WppRegistry::WppRegistry(WppClient &client): _client(client) {
    WPP_LOGD(TAG_WPP_REG, "Creating registry instance");
}

/* The start of the prototypes of the mandatory objects. */
#if MANDATORY_SECURITY_OBJ
WppObject<Security>& WppRegistry::security() {
    if (!WppObject<Security>::isCreated()) WppObject<Security>::create(_client, SECURITY_OBJ_INFO);
    return *WppObject<Security>::object();
}
#endif
#if MANDATORY_SERVER_OBJ
WppObject<Server>& WppRegistry::server() {
    if (!WppObject<Server>::isCreated()) WppObject<Server>::create(_client, SERVER_OBJ_INFO);
    return *WppObject<Server>::object();
}
#endif
#if MANDATORY_DEVICE_OBJ
WppObject<Device>& WppRegistry::device() {
    if (!WppObject<Device>::isCreated()) WppObject<Device>::create(_client, DEVICE_OBJ_INFO);
    return *WppObject<Device>::object();
}
#endif
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