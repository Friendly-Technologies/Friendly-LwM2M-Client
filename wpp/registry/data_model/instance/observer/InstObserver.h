#ifndef WPP_INST_OBSERVER_H_
#define WPP_INST_OBSERVER_H_

namespace wpp {

class Instance;

class InstObserver {
public:
    virtual ~InstObserver() {};
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
    /*
     * Notifies observer about the generation of a specific event for
     * a specific implementation of the object. Each implementation of
     * the object can have its own specific events.
     */
    virtual void event(Instance &inst, EVENT_ID_T eventId) {};
};

} // namespace wpp

#endif /* WPP_INST_OBSERVER_H_ */