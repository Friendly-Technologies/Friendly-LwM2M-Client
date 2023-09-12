#include "WppRegistry.h"

#include "WppClient.h"

namespace wpp {

WppRegistry *WppRegistry::_registry = NULL;
std::mutex WppRegistry::_regGuard;

WppRegistry::WppRegistry(const OBJ_RESTORE_T &objRestoreFunc) : _objRestoreFunc(objRestoreFunc) {}

/* ------------- Registry management ------------- */
bool WppRegistry::create(const OBJ_RESTORE_T &objRestoreFunc) {
    if (!WppClient::isCreated()) return false;
    _registry = new WppRegistry(objRestoreFunc);
    return true;
}

bool WppRegistry::isCreated() {
    return _registry;
}

WppRegistry* WppRegistry::takeOwnership() {
    if (!_regGuard.try_lock()) return NULL;
    return _registry;
}

void WppRegistry::giveOwnership() {
    _regGuard.unlock();
}

WppRegistry* WppRegistry::registry() {
    return _registry;
}


/* ------------- Registry functionality ------------- */
void WppRegistry::restoreObject(Lwm2mObject &object) {
    if (_objRestoreFunc) _objRestoreFunc(object);
}


} //wpp