#ifndef WPP_INST_SUBJECT_H_
#define WPP_INST_SUBJECT_H_

#include "ResOp.h"
#include "InstOpObserver.h"
#include "InstEventObserver.h"

namespace wpp {

class Instance;

class InstSubject {
public:
    /*
	 * Subscribers will be notified about the write, delete, replace
	 * and execute of instance resources initiated by server.
	 */
	void subscribe(InstOpObserver *observer) {
        if (!observer) return;
        if (std::find(_opObservers.begin(), _opObservers.end(), observer) == _opObservers.end()) 
            _opObservers.push_back(observer);
    }
	void unsubscribe(InstOpObserver *observer) {
        _opObservers.erase(std::find(_opObservers.begin(), _opObservers.end(), observer));
    }

    /*
	 * Subscribers will be notified about the custom instance events.
	 */
    void subscribe(InstEventObserver *observer) {
        if (!observer) return;
        if (std::find(_eventObservers.begin(), _eventObservers.end(), observer) == _eventObservers.end()) 
            _eventObservers.push_back(observer);
    }
	void unsubscribe(InstEventObserver *observer) {
        _eventObservers.erase(std::find(_eventObservers.begin(), _eventObservers.end(), observer));
    }

protected:
    /*
	 * Notify observers about operation
	 */
	void operationNotify(Instance &inst, const ResLink &resId, ResOp::TYPE type) {
        for(InstOpObserver *observer : _opObservers) {
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

    void eventNotify(Instance &inst, EVENT_ID_T eventId) {
        for(InstEventObserver *observer : _eventObservers) observer->instEvent(inst, eventId);
    }

private:
    std::vector<InstOpObserver*> _opObservers;
    std::vector<InstEventObserver*> _eventObservers;
};

}

#endif //WPP_INST_SUBJECT_H_