#ifndef CONNECTIVITY_MONITORING_H_
#define CONNECTIVITY_MONITORING_H_

#include "WppRegistry.h"
#include "ObjActObserver.h"


using namespace wpp;

class ConnectivityMonitoringImpl: public wpp::ObjActObserver {
public:
    ConnectivityMonitoringImpl();
    ~ConnectivityMonitoringImpl();

    void init(Object &obj);

private:
	void objectRestore(Object &object) override;
    void getIpAddress(string* ip);
};

#endif // CONNECTIVITY_MONITORING_H_