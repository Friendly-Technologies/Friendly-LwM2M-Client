#ifndef WPP_OBJ_SUBJECT_H_
#define WPP_OBJ_SUBJECT_H_

#include "InstOp.h"
#include "ObjObserver.h"
#include "Object.h"

namespace wpp {

class Object;

class ObjSubject {
public:
    enum Action : uint8_t {
        RESTORE,
    };

public:
    /*
	 * Subscribers will be notified about the creation
	 * and deletion of object instances initiated by server.
	 */
	void subscribe(ObjObserver *observer) {
        if (!observer) return;
        if (std::find(_observers.begin(), _observers.end(), observer) == _observers.end()) 
            _observers.push_back(observer);
    }
	void unsubscribe(ObjObserver *observer) {
        _observers.erase(std::find(_observers.begin(), _observers.end(), observer));
    }
    
protected:
    /*
	 * Notify observers about operation
	 */
	void observerNotify(Object &obj, ID_T instanceId, InstOp::TYPE type) {
        for(ObjObserver* observer : _observers) {
            if (type == InstOp::CREATE) {
                observer->instanceCreated(obj, instanceId);
            } else if (type == InstOp::DELETE) {
                observer->instanceDeleting(obj, instanceId);
            }
        }
    }
    /*
     * Requests observer to do action
     */
    void observerDoAction(Object &obj, Action action) {
        for(ObjObserver* observer : _observers) {
            if (action == Action::RESTORE) {
                observer->objectRestore(obj);
            }
        }
    }

private:
    std::vector<ObjObserver *> _observers;
};

}

#endif //WPP_OBJ_SUBJECT_H_