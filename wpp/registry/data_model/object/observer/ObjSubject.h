#ifndef WPP_OBJ_SUBJECT_H_
#define WPP_OBJ_SUBJECT_H_

#include "Operation.h"
#include "ObjOpObserver.h"
#include "ObjActObserver.h"

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
     * @brief The Action enum represents the possible actions that can be performed on an object.
     */
    enum Action : uint8_t {
        RESTORE, /**< Restore action for object. */
    };

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

    /**
     * @brief Subscribes an observer to receive notifications about required restore actions for objects.
     * 
     * @param observer A pointer to the ObjActObserver object to be subscribed.
     */
    void actSubscribe(ObjActObserver *observer) {
        if (!observer) return;
        if (std::find(_actObservers.begin(), _actObservers.end(), observer) == _actObservers.end()) 
            _actObservers.push_back(observer);
    }

    /**
     * @brief Unsubscribes an observer from receiving notifications about required restore actions for objects.
     * 
     * @param observer A pointer to the ObjActObserver object to be unsubscribed.
     */
    void actUnsubscribe(ObjActObserver *observer) {
        _actObservers.erase(std::find(_actObservers.begin(), _actObservers.end(), observer));
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
    void operationNotify(Object &obj, ID_T instanceId, Operation::TYPE type) {
        for(ObjOpObserver* observer : _opObservers) {
            if (type == Operation::CREATE) {
                observer->instanceCreated(obj, instanceId);
            } else if (type == Operation::DELETE) {
                observer->instanceDeleting(obj, instanceId);
            }
        }
    }

    /**
     * @brief Requests observers to perform an action on an object.
     * 
     * This function is called to request the subscribed observers to perform a specific action on the object.
     * 
     * @param obj The Object instance that the action is requested for.
     * @param action The action to be performed on the object.
     */
    void observerDoAction(Object &obj, Action action) {
        for(ObjActObserver* observer : _actObservers) {
            if (action == Action::RESTORE) {
                observer->objectRestore(obj);
            }
        }
    }

private:
    std::vector<ObjOpObserver *> _opObservers;
    std::vector<ObjActObserver *> _actObservers;
};

}

#endif //WPP_OBJ_SUBJECT_H_