#ifndef IOBJOBSERVER_H_
#define IOBJOBSERVER_H_

namespace wpp {

template <typename T>
class Object;

template <typename T>
class IObjObserver {
public:
    virtual ~IObjObserver() {};
    /*
     * Notifies observer about the need to restore the object. 
     * User must restore object to default state.
     */
    virtual void objectRestore(Object<T> &object) = 0;
    /*
     * Notifies observer immediately after creating instance by server. 
     * Instance is already created during this call.
     */
    virtual void instanceCreated(Object<T> &object, ID_T instanceId) {};
    /*
     * Notifies observer about instance deleting by server. 
     * Instance will be deleted after completing this call.
     */
    virtual void instanceDeleting(Object<T> &object, ID_T instanceId) {};
};

} // namespace wpp

#endif /* IOBJOBSERVER_H_ */