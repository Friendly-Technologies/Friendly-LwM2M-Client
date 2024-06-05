#ifndef WPP_OBJ_OP_OBSERVER_H_
#define WPP_OBJ_OP_OBSERVER_H_

#include "WppTypes.h"

namespace wpp {

class Object;

/**
 * @brief The ObjOpObserver class is an abstract base class that defines the interface for observing object operations.
 * 
 * This class provides a set of virtual functions that can be overridden to receive notifications about object operations.
 * It is intended to be subclassed by concrete observer classes that are interested in monitoring the creation and deletion
 * of instances of a specific object.
 */
class ObjOpObserver {
public:
    virtual ~ObjOpObserver() {};

    /**
     * @brief Notifies the observer immediately after an instance of an object is created by the server.
     * 
     * This function is called by the server to notify the observer that an instance of the object has been created.
     * The instance is already created at the time of this call.
     * 
     * @param object The object for which the instance was created.
     * @param instanceId The ID of the created instance.
     */
    virtual void instanceCreated(Object &object, ID_T instanceId) {};

    /**
     * @brief Notifies the observer about the deletion of an instance of an object by the server.
     * 
     * This function is called by the server to notify the observer that an instance of the object is being deleted.
     * The instance will be deleted after completing this call.
     * 
     * @param object The object from which the instance is being deleted.
     * @param instanceId The ID of the instance being deleted.
     */
    virtual void instanceDeleting(Object &object, ID_T instanceId) {};
};

} // namespace wpp

#endif /* WPP_OBJ_OP_OBSERVER_H_ */