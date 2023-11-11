#ifndef WPP_OBJ_SUBJECT_H_
#define WPP_OBJ_SUBJECT_H_

#include "InstOp.h"
#include "ObjOpObserver.h"
#include "ObjActObserver.h"

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
	void subscribe(ObjOpObserver *observer) {
        if (!observer) return;
        if (std::find(_opObservers.begin(), _opObservers.end(), observer) == _opObservers.end()) 
            _opObservers.push_back(observer);
    }
	void unsubscribe(ObjOpObserver *observer) {
        _opObservers.erase(std::find(_opObservers.begin(), _opObservers.end(), observer));
    }

    /*
	 * Subscribers will be notified about required restor
     * action for object.
	 */
	void subscribe(ObjActObserver *observer) {
        if (!observer) return;
        if (std::find(_actObservers.begin(), _actObservers.end(), observer) == _actObservers.end()) 
            _actObservers.push_back(observer);
    }
	void unsubscribe(ObjActObserver *observer) {
        _actObservers.erase(std::find(_actObservers.begin(), _actObservers.end(), observer));
    }
    
protected:
    /*
	 * Notify observers about operation
	 */
	void observerNotify(Object &obj, ID_T instanceId, InstOp::TYPE type) {
        for(ObjOpObserver* observer : _opObservers) {
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
        for(ObjActObserver* observer : _actObservers) {
            if (action == Action::RESTORE) {
                observer->objectRestore(obj);
            }
        }
    }

private:
    std::vector<ObjOpObserver *> _opObservers;
    std::vector<ObjActObserver *> _actObservers;
};

}

#endif //WPP_OBJ_SUBJECT_H_