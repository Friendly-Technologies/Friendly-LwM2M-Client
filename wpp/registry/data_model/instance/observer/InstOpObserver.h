#ifndef WPP_INST_OP_OBSERVER_H_
#define WPP_INST_OP_OBSERVER_H_

#include "WppTypes.h" 

namespace wpp {

class Instance;

/**
 * @brief The InstOpObserver class is an abstract base class that defines the interface for observing instance operations.
 * 
 * This class provides a set of virtual functions that can be overridden to receive notifications about instance operations.
 * It is intended to be subclassed by concrete observer classes that are interested in monitoring the creation and deletion
 * of resources of a specific instance.
 */
class InstOpObserver {
public:
    virtual ~InstOpObserver() {};
    /**
     * @brief Notifies observer immediately after write resource by server. 
     * Resource is already written during this call.
     */
    virtual void resourceWrite(Instance &inst, const ResLink &resource) {};
    /**
     * @brief Notifies observer immediately after execute resource by server. 
     * Resource is already executed during this call.
     */
    virtual void resourceExecute(Instance &inst, const ResLink &resource) {};
};

} // namespace wpp

#endif /* WPP_INST_OP_OBSERVER_H_ */