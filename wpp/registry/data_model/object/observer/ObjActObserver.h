#ifndef WPP_OBJ_ACT_OBSERVER_H_
#define WPP_OBJ_ACT_OBSERVER_H_

#include "WppTypes.h"

namespace wpp {

class Object;

/**
 * @brief The ObjActObserver class is an abstract base class that defines the interface for observing object actions.
 * 
 * This class provides a set of virtual functions that can be overridden to receive notifications about object actions.
 * It is intended to be subclassed by concrete observer classes that are interested in monitoring the required restore
 * actions for a specific object.
 */
class ObjActObserver {
public:
    virtual ~ObjActObserver() {};
    /**
     * @brief Notifies observer about the need to restore the object. 
     * User must restore object to default state.
     */
    virtual void objectRestore(Object &object) = 0;
};

} // namespace wpp

#endif /* WPP_OBJ_ACT_OBSERVER_H_ */