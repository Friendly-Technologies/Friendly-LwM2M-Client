#ifndef SECURITY_H_
#define SECURITY_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class SecurityImpl: public ObjObserver<Security>, public InstObserver<Security> {
	public:
    void init(Object<Security> &securityObj) {
        securityObj.subscribe(this);
        Security *security = securityObj.createInstance();
        security->subscribe(this);
        
        #if WITH_UDP_DTLS
        security->set(Security::SERVER_URI, (STRING_T)"coaps://leshan.eclipseprojects.io:5684");
        security->set(Security::SECURITY_MODE, (INT_T)LWM2M_SECURITY_MODE_PRE_SHARED_KEY);
        string pskId = "SINAI_TEST_DEV_ID";
        security->set(Security::PUBLIC_KEY, OPAQUE_T(pskId.begin(), pskId.end()));
        security->set(Security::SECRET_KEY, OPAQUE_T {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44});
        #else
        security->set(Security::SERVER_URI, (STRING_T)"coap://leshan.eclipseprojects.io:5683");
        #endif
        security->set(Security::BOOTSTRAP_SERVER, false);
        security->set(Security::SERVER_ID, (INT_T)123);
    }

	void objectRestore(Object<Security> &object) override {
		cout << "Security: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object<Security> &object, ID_T instanceId) override {
        cout << "Security: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object<Security> &object, ID_T instanceId) override {
		cout << "Security: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	virtual void resourceRead(Security &inst, const ResLink &resId) override {
        cout << "Security: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceWrite(Security &inst, const ResLink &resId) override {
        cout << "Security: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceExecute(Security &inst, const ResLink &resId) override {
        cout << "Security: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }
};

#endif // SECURITY_H_