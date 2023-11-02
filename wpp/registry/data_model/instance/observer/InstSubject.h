#ifndef WPP_INST_SUBJECT_H_
#define WPP_INST_SUBJECT_H_

#include "ResOp.h"
#include "InstObserver.h"

namespace wpp {

class Instance;

class InstSubject {
public:
    /*
	 * Subscribers will be notified about the write, delete
	 * and execute of instance resources initiated by server.
	 */
	void subscribe(InstObserver *observer) {
    if (!observer) return;
    if (std::find(_observers.begin(), _observers.end(), observer) == _observers.end()) 
        _observers.push_back(observer);
    }
	void unsubscribe(InstObserver *observer) {
        _observers.erase(std::find(_observers.begin(), _observers.end(), observer));
    }

protected:
    /*
	 * Notify observers about operation
	 */
	void observerNotify(Instance &inst, const ResLink &resId, ResOp::TYPE type) {
        for(InstObserver *observer : _observers) {
            switch (type) {
            case ResOp::READ: 
                observer->resourceRead(inst, resId);
                break;
            case ResOp::WRITE_UPD:
            case ResOp::WRITE_REPLACE_RES:
                observer->resourceWrite(inst, resId);
                break;
            case ResOp::WRITE_REPLACE_INST:
                observer->resourcesReplaced(inst);
                break;
            case ResOp::EXECUTE:
                 observer->resourceExecute(inst, resId);
                break;
            default: break;
            }
        }
    }

private:
    std::vector<InstObserver*> _observers;
};

}

#endif //WPP_INST_SUBJECT_H_