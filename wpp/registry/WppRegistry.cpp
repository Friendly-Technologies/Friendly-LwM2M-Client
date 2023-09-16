#include "WppRegistry.h"

#include "WppClient.h"

namespace wpp {

/* ------------- Registry functionality ------------- */
WppRegistry::WppRegistry(const OBJ_RESTORE_T &objRestoreFunc) : _objRestoreFunc(objRestoreFunc) {}

void WppRegistry::restoreObject(OBJ_ID id) {
    if (_objRestoreFunc) _objRestoreFunc(*this, id);
}


} //wpp