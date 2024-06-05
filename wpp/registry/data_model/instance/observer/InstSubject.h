#ifndef WPP_INST_SUBJECT_H_
#define WPP_INST_SUBJECT_H_

#include "ItemOp.h"
#include "InstOpObserver.h"
#include "InstEventObserver.h"

namespace wpp {

class Instance;

/**
 * @brief The InstSubject class represents a subject that allows subscribers to be notified about various events related to instance resources.
 * 
 * This class provides functionality for subscribing and unsubscribing observers to receive notifications about operations, block operations, and custom events.
 * The observers will be notified when there are write, delete, replace, and execute operations on instance resources initiated by the server.
 * Custom instance events can also be subscribed to and unsubscribed from.
 * 
 * The InstSubject class is designed to be inherited by other classes that need to provide observer functionality for instance resources.
 */
class InstSubject {
public:
    /**
     * @brief Subscribes an observer to receive notifications about instance resource operations.
     * 
     * The observer will be notified when there are write, delete, replace, and execute operations on instance resources initiated by the server.
     * 
     * @param observer A pointer to the observer object.
     */
    void opSubscribe(InstOpObserver *observer) {
        if (!observer) return;
        if (std::find(_opObservers.begin(), _opObservers.end(), observer) == _opObservers.end()) 
            _opObservers.push_back(observer);
    }

    /**
     * @brief Unsubscribes an observer from receiving notifications about instance resource operations.
     * 
     * @param observer A pointer to the observer object.
     */
    void opUnsubscribe(InstOpObserver *observer) {
        _opObservers.erase(std::find(_opObservers.begin(), _opObservers.end(), observer));
    }

    /**
     * @brief Subscribes an observer to receive notifications about custom instance events.
     * 
     * The observer will be notified when there are custom events related to the instance.
     * 
     * @param observer A pointer to the observer object.
     */
    void eventSubscribe(InstEventObserver *observer) {
        if (!observer) return;
        if (std::find(_eventObservers.begin(), _eventObservers.end(), observer) == _eventObservers.end()) 
            _eventObservers.push_back(observer);
    }

    /**
     * @brief Unsubscribes an observer from receiving notifications about custom instance events.
     * 
     * @param observer A pointer to the observer object.
     */
    void eventUnsubscribe(InstEventObserver *observer) {
        _eventObservers.erase(std::find(_eventObservers.begin(), _eventObservers.end(), observer));
    }

protected:
    /**
     * @brief Notifies the observers about an operation on an instance resource.
     * 
     * This function is called internally to notify the subscribed observers about an operation on an instance resource.
     * The type of operation (read, write, replace, execute) is specified by the 'type' parameter.
     * 
     * @param inst The instance object on which the operation is performed.
     * @param resId The resource link of the instance resource.
     * @param type The type of operation.
     */
    void operationNotify(Instance &inst, const ResLink &resLink, ItemOp::TYPE type) {
        for(InstOpObserver *observer : _opObservers) {
            switch (type) {
            case ItemOp::WRITE:
                observer->resourceWrite(inst, resLink);
                break;
            case ItemOp::EXECUTE:
                 observer->resourceExecute(inst, resLink);
                break;
            default: break;
            }
        }
    }

    /**
     * @brief Notifies the observers about a custom instance event.
     * 
     * This function is called internally to notify the subscribed observers about a custom instance event.
     * 
     * @param inst The instance object on which the event occurred.
     * @param eventId The ID of the event.
     */
    void eventNotify(Instance &inst, EVENT_ID_T eventId) {
        for(InstEventObserver *observer : _eventObservers) observer->instEvent(inst, eventId);
    }

private:
    std::vector<InstOpObserver*> _opObservers;
    std::vector<InstEventObserver*> _eventObservers;
};

}

#endif //WPP_INST_SUBJECT_H_