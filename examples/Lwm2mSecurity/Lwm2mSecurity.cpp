#include "Lwm2mSecurity.h"
#include "Lwm2mServer.h"

Lwm2mSecurityImpl::Lwm2mSecurityImpl() {}

Lwm2mSecurityImpl::~Lwm2mSecurityImpl() {}

void Lwm2mSecurityImpl::init(Object &obj) {
	obj.actSubscribe(this);
	wpp::Instance *inst0 = obj.createInstance(0);

	#ifdef LWM2M_BOOTSTRAP
        string url = "coap://demodm.friendly-tech.com:5681";
        inst0->resource(Lwm2mSecurity::BOOTSTRAP_SERVER_1)->set<BOOL_T>(true);
        inst0->resource(Lwm2mSecurity::CLIENT_HOLD_OFF_TIME_11)->set<INT_T>(10);
        #if DTLS_WITH_PSK
        string pskId = "SINAI_TEST_DEV_ID";
        url = "coap://demodm.friendly-tech.com:5681";
        inst0->resource(Lwm2mSecurity::SECURITY_MODE_2)->set<INT_T>(LWM2M_SECURITY_MODE_PRE_SHARED_KEY);
        inst0->resource(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3)->set(OPAQUE_T(pskId.begin(), pskId.end()));
        inst0->resource(Lwm2mSecurity::SECRET_KEY_5)->set(OPAQUE_T {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44});
        #endif
    #else
        string url = "coap://demodm.friendly-tech.com:";//"coap://eu.iot.avsystem.cloud:"; //"coaps://leshan.eclipseprojects.io:";
        #if DTLS_WITH_PSK
            url += "5684";
            string pskId = "SINAI_TEST_DEV_ID_VS";
            inst0->resource(Lwm2mSecurity::SECURITY_MODE_2)->set<INT_T>(LWM2M_SECURITY_MODE_PRE_SHARED_KEY);
            inst0->resource(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3)->set(OPAQUE_T(pskId.begin(), pskId.end()));
            //00112233445566778899998877665544
            inst0->resource(Lwm2mSecurity::SECRET_KEY_5)->set(OPAQUE_T {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44});
        #elif DTLS_WITH_RPK
            url += "5684";
            inst0->resource(Lwm2mSecurity::SECURITY_MODE_2)->set<INT_T>(LWM2M_SECURITY_MODE_RAW_PUBLIC_KEY);
            inst0->resource(Lwm2mSecurity::PUBLIC_KEY_OR_IDENTITY_3)->set(OPAQUE_T {0x04, 0xba, 0xda, 0x54, 0x75, 0x34, 0x4b, 0xa2, 0x29, 0x61, 0xa7, 0xd9, 0x65, 0xac, 0x51, 0x8e, 0x73, 0x48, 0x1a, 0x5f, 0x77, 0x83, 0x2b, 0xd9, 0x96, 0xc2, 0xfa, 0x35, 0x27, 0xe8, 0xf3, 0xc4, 0x24, 0x8d, 0xda, 0x62, 0x1f, 0xa9, 0xc1, 0x34, 0x8d, 0x13, 0x65, 0xc3, 0x57, 0x35, 0x7c, 0x54, 0x86, 0x94, 0x77, 0xe3, 0x87, 0xfd, 0x2c, 0x26, 0x75, 0xb1, 0xc6, 0xf2, 0x8a, 0xa5, 0x06, 0x67, 0x7b});
            inst0->resource(Lwm2mSecurity::SECRET_KEY_5)->set(OPAQUE_T {0x92, 0x04, 0x53, 0x22, 0xa5, 0xb3, 0x45, 0x62, 0xe1, 0xff, 0xec, 0x4b, 0xcd, 0xcc, 0x25, 0x7b, 0x9e, 0xcf, 0xc3, 0x47, 0x8b, 0xfa, 0xea, 0x4b, 0x6b, 0x07, 0x31, 0x35, 0x02, 0x02, 0xef, 0x2d});
        #else
            url += "5683";
            inst0->resource(Lwm2mSecurity::SECURITY_MODE_2)->set<INT_T>(LWM2M_SECURITY_MODE_NONE);
        #endif
        inst0->resource(Lwm2mSecurity::BOOTSTRAP_SERVER_1)->set<BOOL_T>(false);
    #endif

    inst0->resource(Lwm2mSecurity::LWM2M_SERVER_URI_0)->set<STRING_T>(url);
    inst0->resource(Lwm2mSecurity::SHORT_SERVER_ID_10)->set<INT_T>(TEST_SERVER_SHORT_ID);
}

void Lwm2mSecurityImpl::objectRestore(Object &object) {
	cout << "Lwm2mSecurity: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
	init(object);
}
