#ifndef WPP_INST_EVENT_OBSERVER_H_
#define WPP_INST_EVENT_OBSERVER_H_

#include "types.h" 

namespace wpp {

class Instance;

class InstEventObserver {
public:
    virtual ~InstEventObserver() {};
    /*
     * Notifies observer about the generation of a specific event for
     * a specific implementation of the object. Each implementation of
     * the object can have its own specific events.
     */
    virtual void event(Instance &inst, EVENT_ID_T eventId) {};
};

} // namespace wpp

#endif /* WPP_INST_EVENT_OBSERVER_H_ */