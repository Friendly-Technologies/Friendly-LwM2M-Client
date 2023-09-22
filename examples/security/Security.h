#ifndef SECURITY_H_
#define SECURITY_H_

#include <iostream>
#include "WppRegistry.h"
#include "IWppObjObserver.h"
#include "IWppInstObserver.h"

using namespace wpp;
using namespace std;

class SecurityImpl: public IWppObjObserver<Security>, public IWppInstObserver<Security> {
	public:
    void init(WppObject<Security> &securityObj) {
        securityObj.subscribe(this);
        wpp::Security *security = securityObj.createInstance();
        security->subscribe(this);
    }

	void objectRestore(WppObject<Security> &object) override {
		cout << "Security: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(WppObject<Security> &object, ID_T instanceId) override {
        cout << "Security: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(WppObject<Security> &object, ID_T instanceId) override {
		cout << "Security: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	virtual void resourceRead(Security &inst, const ResourceID &resId) override {
        cout << "Security: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceWrite(Security &inst, const ResourceID &resId) override {
        cout << "Security: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceExecute(Security &inst, const ResourceID &resId) override {
        cout << "Security: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }
};

#endif // SECURITY_H_