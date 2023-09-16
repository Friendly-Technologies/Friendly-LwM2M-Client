#ifndef IOBJOBSERVER_H_
#define IOBJOBSERVER_H_

#include "Object.h"

namespace wpp {

template <typename T>
class Object;

template <typename T>
class IObjObserver {
public:
    virtual ~IObjObserver() {};
    /*
     * Notifies observer immediately after creating instance by server. 
     * Instance is already created during this call.
     */
    virtual void instanceCreated(Object<T> &object, ID_T instanceId) = 0;
    /*
     * Notifies observer about instance deleting by server. 
     * Instance will be deleted after completing this call.
     */
    virtual void instanceDeleting(Object<T> &object, ID_T instanceId) = 0;
};

} // namespace wpp

#endif /* IOBJOBSERVER_H_ */