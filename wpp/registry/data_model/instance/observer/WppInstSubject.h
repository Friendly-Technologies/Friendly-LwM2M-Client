#ifndef WPP_INST_SUBJECT_H_
#define WPP_INST_SUBJECT_H_

#include "WppOperation.h"
#include "IWppInstObserver.h"

namespace wpp {

template <typename T>
class WppInstSubject {
public:
    /*
	 * Subscribers will be notified about the write, delete
	 * and execute of instance resources initiated by server.
	 */
	void subscribe(IWppInstObserver<T> *observer) {
    if (!observer) return;
    if (std::find(_observers.begin(), _observers.end(), observer) == _observers.end()) 
        _observers.push_back(observer);
    }
	void unsubscribe(IWppInstObserver<T> *observer) {
        _observers.erase(std::find(_observers.begin(), _observers.end(), observer));
    }

protected:
    /*
	 * Notify observers about operation
	 */
	void observerNotify(T &inst, const ResourceID &resId, WppOperation::TYPE type) {
        for(IWppInstObserver<T>* observer : _observers) {
            if (type == WppOperation::TYPE::READ) {
                observer->resourceRead(inst, resId);
            } else if (type == WppOperation::TYPE::WRITE) {
                observer->resourceWrite(inst, resId);
            } else if (type == WppOperation::TYPE::EXECUTE) {
                observer->resourceExecute(inst, resId);
            }
        }
    }

private:
    std::vector<IWppInstObserver<T> *> _observers;
};

}

#endif //WPP_INST_SUBJECT_H_