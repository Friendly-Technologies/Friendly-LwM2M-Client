#ifndef SECURITY_H_
#define SECURITY_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"
#include "InstObserver.h"

using namespace wpp;
using namespace std;

class SecurityImpl: public ObjObserver<WppLWM2MSecurity>, public InstObserver<WppLWM2MSecurity> {
	public:
    void init(Object<WppLWM2MSecurity> &securityObj) {
        securityObj.subscribe(this);
        WppLWM2MSecurity *security = securityObj.createInstance();
        security->subscribe(this);
        
        #if LWM2M_BOOTSTRAP
            string url = "coap://demodm.friendly-tech.com:5680";
            security->set(WppLWM2MSecurity::BOOTSTRAP_SERVER_1, true);
            security->set(WppLWM2MSecurity::CLIENT_HOLD_OFF_TIME_11, (INT_T)10);
        #else
            string url = "coap://demodm.friendly-tech.com:"; //"coaps://leshan.eclipseprojects.io:";
            #if DTLS_WITH_PSK
                url += "5684";
                string pskId = "SINAI_TEST_DEV_ID";
                security->set(WppLWM2MSecurity::SECURITY_MODE_2, (INT_T)LWM2M_SECURITY_MODE_PRE_SHARED_KEY);
                security->set(WppLWM2MSecurity::PUBLIC_KEY_OR_IDENTITY_3, OPAQUE_T(pskId.begin(), pskId.end()));
                security->set(WppLWM2MSecurity::SECRET_KEY_5, OPAQUE_T {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44});
            #elif DTLS_WITH_RPK
                url += "5684";
                security->set(WppLWM2MSecurity::SECURITY_MODE_2, (INT_T)LWM2M_SECURITY_MODE_RAW_PUBLIC_KEY);
                security->set(WppLWM2MSecurity::PUBLIC_KEY_OR_IDENTITY_3, OPAQUE_T {0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xff, 0xd8, 0x5d, 0xeb, 0x4a, 0x66, 0x40, 0xf4, 0x30, 0x0d, 0x9f, 0x04, 0x6f, 0xe3, 0x42, 0xa9, 0x95, 0xf3, 0xa8, 0xa3, 0x20, 0x3f, 0xe7, 0xd2, 0xc3, 0x2a, 0x9b, 0xe1, 0x78, 0x87, 0x75, 0x2e, 0x6d, 0x6c, 0x58, 0x5b, 0x8d, 0x8a, 0xb8, 0x48, 0x97, 0x90, 0x07, 0xf7, 0x75, 0xfd, 0x15, 0x3c, 0x5a, 0x45, 0xf2, 0xa8, 0xf1, 0xe5, 0x56, 0x25, 0x86, 0x8d, 0x6a, 0x92, 0x8d, 0xbc, 0x77, 0xd5});
                security->set(WppLWM2MSecurity::SECRET_KEY_5, OPAQUE_T {0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0x42, 0x60, 0x1d, 0xb0, 0xb3, 0xa9, 0xa1, 0xa0, 0xb6, 0xdc, 0xab, 0x94, 0xf7, 0x1c, 0x2c, 0x44, 0x38, 0x3d, 0x6a, 0x2a, 0x61, 0x9b, 0xac, 0x33, 0xea, 0x20, 0x90, 0x83, 0x4f, 0x4e, 0xd5, 0xf9, 0xa0, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0xff, 0xd8, 0x5d, 0xeb, 0x4a, 0x66, 0x40, 0xf4, 0x30, 0x0d, 0x9f, 0x04, 0x6f, 0xe3, 0x42, 0xa9, 0x95, 0xf3, 0xa8, 0xa3, 0x20, 0x3f, 0xe7, 0xd2, 0xc3, 0x2a, 0x9b, 0xe1, 0x78, 0x87, 0x75, 0x2e, 0x6d, 0x6c, 0x58, 0x5b, 0x8d, 0x8a, 0xb8, 0x48, 0x97, 0x90, 0x07, 0xf7, 0x75, 0xfd, 0x15, 0x3c, 0x5a, 0x45, 0xf2, 0xa8, 0xf1, 0xe5, 0x56, 0x25, 0x86, 0x8d, 0x6a, 0x92, 0x8d, 0xbc, 0x77, 0xd5});
            #else
                url += "5683";
                security->set(WppLWM2MSecurity::SECURITY_MODE_2, (INT_T)LWM2M_SECURITY_MODE_NONE);
            #endif
            security->set(WppLWM2MSecurity::BOOTSTRAP_SERVER_ACCOUNT_TIMEOUT_12, false);
        #endif

        security->set(WppLWM2MSecurity::LWM2M_SERVER_URI_0, (STRING_T)url);
        security->set(WppLWM2MSecurity::SHORT_SERVER_ID_10, (INT_T)123);
    }

	void objectRestore(Object<WppLWM2MSecurity> &object) override {
		cout << "WppLWM2MSecurity: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object<WppLWM2MSecurity> &object, ID_T instanceId) override {
        cout << "WppLWM2MSecurity: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
        object.instance(instanceId)->subscribe(this);
    }

    void instanceDeleting(Object<WppLWM2MSecurity> &object, ID_T instanceId) override {
		cout << "WppLWM2MSecurity: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(WppLWM2MSecurity &inst, const ResLink &resId) override {
        cout << "WppLWM2MSecurity: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(WppLWM2MSecurity &inst, const ResLink &resId) override {
        cout << "WppLWM2MSecurity: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(WppLWM2MSecurity &inst, const ResLink &resId) override {
        cout << "WppLWM2MSecurity: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(WppLWM2MSecurity &inst) override {
        cout << "WppLWM2MSecurity: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }
};

#endif // SECURITY_H_