#ifndef WPP_OBJ_ACT_OBSERVER_H_
#define WPP_OBJ_ACT_OBSERVER_H_

#include "types.h"

namespace wpp {

class Object;

class ObjActObserver {
public:
    virtual ~ObjActObserver() {};
    /*
     * Notifies observer about the need to restore the object. 
     * User must restore object to default state.
     */
    virtual void objectRestore(Object &object) = 0;
};

} // namespace wpp

#endif /* WPP_OBJ_ACT_OBSERVER_H_ */