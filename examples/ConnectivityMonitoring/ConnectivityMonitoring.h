#ifndef CONNECTIVITY_MONITORING_H_
#define CONNECTIVITY_MONITORING_H_

#include <iostream>
#include "WppRegistry.h"
#include "ObjActObserver.h"

using namespace wpp;
using namespace std;

class ConnectivityMonitoringImpl: public wpp::ObjActObserver {
public:
    ConnectivityMonitoringImpl();
    ~ConnectivityMonitoringImpl();

    void init(Object &obj);

	void objectRestore(Object &object) override;
    void getIpAddress(string* ip);
};

#endif // CONNECTIVITY_MONITORING_H_