#include "WppRegistry.h"

#include "WppClient.h"

namespace wpp {

/* ------------- Registry functionality ------------- */
WppRegistry::WppRegistry(const OBJ_RESTORE_T &objRestoreFunc) : _objRestoreFunc(objRestoreFunc) {}

void WppRegistry::restoreObject(Lwm2mObject &object) {
    if (_objRestoreFunc) _objRestoreFunc(*this, object);
}


} //wpp