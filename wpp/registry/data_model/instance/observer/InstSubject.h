#ifndef WPP_INST_SUBJECT_H_
#define WPP_INST_SUBJECT_H_

#include "ResOp.h"
#include "InstOpObserver.h"
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
#include "InstBlockOpObserver.h"
#endif
#include "InstEventObserver.h"

namespace wpp {

class Instance;

/**
 * @brief The InstSubject class represents a subject that allows subscribers to be notified about various events related to instance resources.
 * 
 * This class provides functionality for subscribing and unsubscribing observers to receive notifications about operations, block operations, and custom events.
 * The observers will be notified when there are write, delete, replace, and execute operations on instance resources initiated by the server.
 * Additionally, if the LWM2M_RAW_BLOCK1_REQUESTS macro is defined, the observers will also be notified about block write and block execute operations.
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

    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    /**
     * @brief Subscribes an observer to receive notifications about block operations on instance resources.
     * 
     * The observer will be notified when there are block write and block execute operations on instance resources initiated by the server.
     * 
     * @param observer A pointer to the observer object.
     */
    void blockOpSubscribe(InstBlockOpObserver *observer) {
        if (!observer) return;
        if (std::find(_blockOpObservers.begin(), _blockOpObservers.end(), observer) == _blockOpObservers.end()) 
            _blockOpObservers.push_back(observer);
    }

    /**
     * @brief Unsubscribes an observer from receiving notifications about block operations on instance resources.
     * 
     * @param observer A pointer to the observer object.
     */
    void blockOpUnsubscribe(InstBlockOpObserver *observer) {
        _blockOpObservers.erase(std::find(_blockOpObservers.begin(), _blockOpObservers.end(), observer));
    }
    #endif

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
    void operationNotify(Instance &inst, const ResLink &resLink, ResOp::TYPE type) {
        for(InstOpObserver *observer : _opObservers) {
            switch (type) {
            case ResOp::READ: 
                observer->resourceRead(inst, resLink);
                break;
            case ResOp::WRITE:
                observer->resourceWrite(inst, resLink);
                break;
            case ResOp::EXECUTE:
                 observer->resourceExecute(inst, resLink);
                break;
            default: break;
            }
        }
    }

    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    /**
     * @brief Notifies the observers about a block operation on an instance resource.
     * 
     * This function is called internally to notify the subscribed observers about a block operation on an instance resource.
     * The type of block operation (block write, block execute) is specified by the 'type' parameter.
     * 
     * @param inst The instance object on which the block operation is performed.
     * @param resId The resource link of the instance resource.
     * @param type The type of block operation.
     * @param buff The opaque buffer containing the block data.
     * @param blockNum The block number.
     * @param isLastBlock Indicates whether this is the last block of the operation.
     */
    void blockOperationNotify(Instance &inst, const ResLink &resLink, ResOp::TYPE type, const OPAQUE_T &buff, size_t blockNum, bool isLastBlock) {
        for(InstBlockOpObserver *observer : _blockOpObservers) {
            switch (type) {
            case ResOp::BLOCK_WRITE: 
                observer->resourceBlockWrite(inst, resId, buff, blockNum, isLastBlock);
                break;
            case ResOp::BLOCK_EXECUTE:
                observer->resourceBlockExecute(inst, resId, buff, blockNum, isLastBlock);
                break;
            default: break;
            }
        }
    }
    #endif

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
    #ifdef LWM2M_RAW_BLOCK1_REQUESTS
    std::vector<InstBlockOpObserver*> _blockOpObservers;
    #endif
    std::vector<InstEventObserver*> _eventObservers;
};

}

#endif //WPP_INST_SUBJECT_H_