#ifndef WPP_OBJ_SUBJECT_H_
#define WPP_OBJ_SUBJECT_H_

#include "Operation.h"
#include "IObjObserver.h"
#include "Object.h"

namespace wpp {


template <typename T>
class Object;

template <typename T>
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
	void subscribe(IObjObserver<T> *observer) {
    if (!observer) return;
    if (std::find(_observers.begin(), _observers.end(), observer) == _observers.end()) 
        _observers.push_back(observer);
    }
	void unsubscribe(IObjObserver<T> *observer) {
        _observers.erase(std::find(_observers.begin(), _observers.end(), observer));
    }
    
protected:
    /*
	 * Notify observers about operation
	 */
	void observerNotify(Object<T> &obj, ID_T instanceId, Operation::TYPE type) {
        for(IObjObserver<T>* observer : _observers) {
            if (type == Operation::TYPE::CREATE) {
                observer->instanceCreated(obj, instanceId);
            } else if (type == Operation::TYPE::DELETE) {
                observer->instanceDeleting(obj, instanceId);
            }
        }
    }
    /*
     * Requests observer to do action
     */
    void observerDoAction(Object<T> &obj, Action action) {
        for(IObjObserver<T>* observer : _observers) {
            if (action == Action::RESTORE) {
                observer->objectRestore(obj);
            }
        }
    }

private:
    std::vector<IObjObserver<T> *> _observers;
};

}

#endif //WPP_OBJ_SUBJECT_H_