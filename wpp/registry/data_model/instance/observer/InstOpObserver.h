#ifndef WPP_INST_OP_OBSERVER_H_
#define WPP_INST_OP_OBSERVER_H_

#include "types.h" 

namespace wpp {

class Instance;

class InstOpObserver {
public:
    virtual ~InstOpObserver() {};
    /*
     * Notifies observer immediately after read resource by server. 
     * Resource is already readed during this call.
     */
    virtual void resourceRead(Instance &inst, const ResLink &resource) {};
    /*
     * Notifies observer immediately after write resource by server. 
     * Resource is already written during this call.
     */
    virtual void resourceWrite(Instance &inst, const ResLink &resource) {};
    /*
     * Notifies observer immediately after execute resource by server. 
     * Resource is already executed during this call.
     */
    virtual void resourceExecute(Instance &inst, const ResLink &resource) {};
    /*
     * Notifies observer immediately after replace instance by server. 
     * Instance and resource are already written during this call.
     */
    virtual void resourcesReplaced(Instance &inst) {};
};

} // namespace wpp

#endif /* WPP_INST_OP_OBSERVER_H_ */