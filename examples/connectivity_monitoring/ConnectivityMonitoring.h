#ifndef CONNECTIVITY_MONITORING_H_
#define CONNECTIVITY_MONITORING_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjObserver.h"

#include <ifaddrs.h>

using namespace wpp;
using namespace std;

class CnnctvtMonnImpl: public ObjObserver, public InstOpObserver {
	public:
	void init(Object &connMonObj) {
		connMonObj.subscribe(this);
        Instance *connMon = connMonObj.createInstance();
        connMon->subscribe(this);

        STRING_T ip;
        getIpAddress(&ip);
        connMon->set(wpp::ConnectivityMonitoring::NETWORK_BEARER_0,
                     INT_T(wpp::ConnectivityMonitoring::NtwrkBrr::ETHERNET));
        connMon->set(wpp::ConnectivityMonitoring::RADIO_SIGNAL_STRENGTH_2, INT_T(-20));
        connMon->set(wpp::ConnectivityMonitoring::IP_ADDRESSES_4, ip);
	}

	void objectRestore(Object &object) override {
		cout << "CnnctvtMonnImpl: objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
        init(object);
	}

    void instanceCreated(Object &object, ID_T instanceId) override {
        cout << "CnnctvtMonnImpl: instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object &object, ID_T instanceId) override {
		cout << "CnnctvtMonnImpl: instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	void resourceRead(Instance &inst, const ResLink &resId) override {
        cout << "CnnctvtMonnImpl: resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceWrite(Instance &inst, const ResLink &resId) override {
        cout << "CnnctvtMonnImpl: resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourceExecute(Instance &inst, const ResLink &resId) override {
        cout << "CnnctvtMonnImpl: resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    void resourcesReplaced(Instance &inst) override {
        cout << "CnnctvtMonnImpl: resourcesReplaced: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << endl;
    }

    void getIpAddress(string* ip) {
        struct ifaddrs *interfaces = NULL;
        struct ifaddrs *temp_addr = NULL;
        // retrieve the current interfaces - returns 0 on success
        if (getifaddrs(&interfaces) == 0) {
            // Loop through linked list of interfaces
            temp_addr = interfaces;
            while (temp_addr != NULL) {
                if (temp_addr->ifa_addr->sa_family == AF_INET && strcmp(temp_addr->ifa_name, "en0")) {
                    *ip = inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                }
                temp_addr = temp_addr->ifa_next;
            }
        }
        freeifaddrs(interfaces);  // free memory
    }
};

#endif // CONNECTIVITY_MONITORING_H_