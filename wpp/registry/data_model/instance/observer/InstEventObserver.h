#ifndef WPP_INST_EVENT_OBSERVER_H_
#define WPP_INST_EVENT_OBSERVER_H_

#include "WppTypes.h" 

namespace wpp {

class Instance;

/**
 * @brief The InstEventObserver class is an abstract base class that defines the interface for observing instance events.
 * 
 * This class provides a set of virtual functions that can be overridden to receive notifications about instance events.
 * It is intended to be subclassed by concrete observer classes that are interested in monitoring the generation of
 * specific events for a specific instance.
 */
class InstEventObserver {
public:
    virtual ~InstEventObserver() {};
    /**
     * @brief Notifies observer about the generation of a specific event for
     * a specific implementation of the object. Each implementation of
     * the object can have its own specific events.
     */
    virtual void instEvent(Instance &inst, EVENT_ID_T eventId) {};
};

} // namespace wpp

#endif /* WPP_INST_EVENT_OBSERVER_H_ */