#ifndef IOBSERVER_H_
#define IOBSERVER_H_

#include "WppRegistry.h"

namespace wpp {

class WppRegistry;

class IObjObserver {
public:
    virtual ~IObjObserver() {};
    /*
     * Notifies observer immediately after creating instance by server. 
     * Instance is already created during this call.
     */
    virtual void instanceCreated(WppRegistry &reg, const InstanceID &id) = 0;
    /*
     * Notifies observer about instance deleting by server. 
     * Instance will be deleted after completing this call.
     */
    virtual void instanceDeleting(WppRegistry &reg, const InstanceID &id) = 0;
};

} // namespace wpp

#endif /* IOBSERVER_H_ */