#ifndef WPP_OBJ_SUBJECT_H_
#define WPP_OBJ_SUBJECT_H_

#include "ItemOp.h"
#include "ObjOpObserver.h"

namespace wpp {

class Object;

/**
 * @brief The ObjSubject class represents a subject that notifies observers about object operations and actions.
 * 
 * This class provides functionality for subscribing and unsubscribing observers to receive notifications about
 * the creation and deletion of object instances initiated by the server, as well as required restore actions for objects.
 * 
 * The ObjSubject class is designed to be inherited by other classes that require observer functionality.
 * 
 * @note The ObjSubject class requires the presence of ObjOpObserver and ObjActObserver classes for observer functionality.
 */
class ObjSubject {
public:
    /**
     * @brief Subscribes an observer to receive notifications about object operations.
     * 
     * @param observer A pointer to the ObjOpObserver object to be subscribed.
     */
    void opSubscribe(ObjOpObserver *observer) {
        if (!observer) return;
        if (std::find(_opObservers.begin(), _opObservers.end(), observer) == _opObservers.end()) 
            _opObservers.push_back(observer);
    }

    /**
     * @brief Unsubscribes an observer from receiving notifications about object operations.
     * 
     * @param observer A pointer to the ObjOpObserver object to be unsubscribed.
     */
    void opUnsubscribe(ObjOpObserver *observer) {
        _opObservers.erase(std::find(_opObservers.begin(), _opObservers.end(), observer));
    }
    
protected:
    /**
     * @brief Notifies observers about object operations.
     * 
     * This function is called to notify the subscribed observers about the creation and deletion of object instances.
     * 
     * @param obj The Object instance that the operation is performed on.
     * @param instanceId The ID of the object instance.
     * @param type The type of operation (CREATE or DELETE).
     */
    void operationNotify(Object &obj, ID_T instanceId, ItemOp::TYPE type) {
        for(ObjOpObserver* observer : _opObservers) {
            if (type == ItemOp::CREATE) {
                observer->instanceCreated(obj, instanceId);
            } else if (type == ItemOp::DELETE) {
                observer->instanceDeleting(obj, instanceId);
            }
        }
    }

private:
    std::vector<ObjOpObserver *> _opObservers;
};

}

#endif //WPP_OBJ_SUBJECT_H_