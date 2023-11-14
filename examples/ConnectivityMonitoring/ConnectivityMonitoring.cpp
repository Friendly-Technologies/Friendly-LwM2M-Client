#include "ConnectivityMonitoring.h"

ConnectivityMonitoringImpl::ConnectivityMonitoringImpl() {}

ConnectivityMonitoringImpl::~ConnectivityMonitoringImpl() {}

void ConnectivityMonitoringImpl::init(Object &obj) {
	obj.subscribe(this);
	wpp::Instance *inst0 = obj.createInstance(0);
	inst0->set(ConnectivityMonitoring::NETWORK_BEARER_0,
               INT_T(ConnectivityMonitoring::NtwrkBrr::ETHERNET));
	inst0->set(ConnectivityMonitoring::RADIO_SIGNAL_STRENGTH_2, INT_T(-20));
}


void ConnectivityMonitoringImpl::objectRestore(Object &object) {
	cout << "CnnctvtMonnImpl: objectRestore: " << (ID_T)object.getObjectID() << endl;
	object.clear();
	init(object);
}

void ConnectivityMonitoringImpl::getIpAddress(string* ip) {
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
