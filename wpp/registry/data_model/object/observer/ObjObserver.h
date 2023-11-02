#ifndef I_WPP_OBJ_OBSERVER_H_
#define I_WPP_OBJ_OBSERVER_H_

#include "types.h"

namespace wpp {

class Object;

class ObjObserver {
public:
    virtual ~ObjObserver() {};
    /*
     * Notifies observer about the need to restore the object. 
     * User must restore object to default state.
     */
    virtual void objectRestore(Object &object) = 0;
    /*
     * Notifies observer immediately after creating instance by server. 
     * Instance is already created during this call.
     */
    virtual void instanceCreated(Object &object, ID_T instanceId) {};
    /*
     * Notifies observer about instance deleting by server. 
     * Instance will be deleted after completing this call.
     */
    virtual void instanceDeleting(Object &object, ID_T instanceId) {};
};

} // namespace wpp

#endif /* I_WPP_OBJ_OBSERVER_H_ */