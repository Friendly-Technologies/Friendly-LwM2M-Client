#ifndef WPP_INST_OBSERVER_H_
#define WPP_INST_OBSERVER_H_

namespace wpp {

template <typename T>
class InstObserver {
public:
    virtual ~InstObserver() {};
    /*
     * Notifies observer immediately after read resource by server. 
     * Resource is already readed during this call.
     */
    virtual void resourceRead(T &inst, const ResLink &resource) = 0;
    /*
     * Notifies observer immediately after write resource by server. 
     * Resource is already writed during this call.
     */
    virtual void resourceWrite(T &inst, const ResLink &resource) = 0;
    /*
     * Notifies observer immediately after execute resource by server. 
     * Resource is already executed during this call.
     */
    virtual void resourceExecute(T &inst, const ResLink &resource) = 0;
};

} // namespace wpp

#endif /* WPP_INST_OBSERVER_H_ */