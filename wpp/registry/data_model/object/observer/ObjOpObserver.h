#ifndef WPP_OBJ_OP_OBSERVER_H_
#define WPP_OBJ_OP_OBSERVER_H_

#include "types.h"

namespace wpp {

class Object;

class ObjOpObserver {
public:
    virtual ~ObjOpObserver() {};
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

#endif /* WPP_OBJ_OP_OBSERVER_H_ */