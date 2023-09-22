#ifndef WPP_OBJ_SUBJECT_H_
#define WPP_OBJ_SUBJECT_H_

#include "WppOperation.h"
#include "IWppObjObserver.h"
#include "WppObject.h"

namespace wpp {


template <typename T>
class WppObject;

template <typename T>
class WppObjSubject {
public:
    enum Action : uint8_t {
        RESTORE,
    };

public:
    /*
	 * Subscribers will be notified about the creation
	 * and deletion of object instances initiated by server.
	 */
	void subscribe(IWppObjObserver<T> *observer) {
    if (!observer) return;
    if (std::find(_observers.begin(), _observers.end(), observer) == _observers.end()) 
        _observers.push_back(observer);
    }
	void unsubscribe(IWppObjObserver<T> *observer) {
        _observers.erase(std::find(_observers.begin(), _observers.end(), observer));
    }
    
protected:
    /*
	 * Notify observers about operation
	 */
	void observerNotify(WppObject<T> &obj, ID_T instanceId, WppOperation::TYPE type) {
        for(IWppObjObserver<T>* observer : _observers) {
            if (type == WppOperation::TYPE::CREATE) {
                observer->instanceCreated(obj, instanceId);
            } else if (type == WppOperation::TYPE::DELETE) {
                observer->instanceDeleting(obj, instanceId);
            }
        }
    }
    /*
     * Requests observer to do action
     */
    void observerDoAction(WppObject<T> &obj, Action action) {
        for(IWppObjObserver<T>* observer : _observers) {
            if (action == Action::RESTORE) {
                observer->objectRestore(obj);
            }
        }
    }

private:
    std::vector<IWppObjObserver<T> *> _observers;
};

}

#endif //WPP_OBJ_SUBJECT_H_