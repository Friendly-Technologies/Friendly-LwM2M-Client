//============================================================================
// Name        : ObjectRegestry.cpp
// Author      : Valentin
// Version     :
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>

#include "Server.h"
#include "Security.h"
#include "Device.h"
#include "Connection.h"
#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
#include "ConnectivityMonitoring.h"
#endif
#if OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
#include "AccessControl.h"
#endif
#if OBJ_O_5_FIRMWARE_UPDATE_V11
#include "FwUpdate.h"
#endif

#include "WppClient.h"
#include "WppRegistry.h"

using namespace std;
using namespace wpp;

void socketPolling(Connection *connection, DeviceImpl *device) {
	while (!device->isNeededReboot()) {
		connection->loop();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main() {
	cout << endl << "---- Creating requiered components ----" << endl;
	Connection connection("56830", AF_INET);
	ServerImpl server;
	SecurityImpl security;
	DeviceImpl device;
	#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
	CnnctvtMonnImpl conn_mon;
	#endif
	#if OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
	AccessControl accessCtrl;
	#endif
	#if OBJ_O_5_FIRMWARE_UPDATE_V11
	FwUpdateImpl fwUpd;
	#endif

	// Client initialization
	cout << endl << "---- Creating WppClient ----" << endl;
	string clientName = "SinaiTestLwm2m";
	#if DTLS_WITH_PSK
	clientName += "PSK";
	#elif DTLS_WITH_RPK
	clientName += "RPK";
	#endif
	cout << "WppClient name: " << clientName << endl;
	WppClient::create({clientName, "", ""}, connection);
	WppClient *client = WppClient::takeOwnership();
	WppRegistry &registry = client->registry();

	// Initialize wpp objects
	cout << endl << "---- Initialization wpp Server ----" << endl;
	server.init(registry.lwm2mServer());
	cout << endl << "---- Initialization wpp Security ----" << endl;
	security.init(registry.lwm2mSecurity());
	cout << endl << "---- Initialization wpp Device ----" << endl;
	device.init(registry.device());
	#if OBJ_O_4_CONNECTIVITY_MONITORING_V13
	cout << endl << "---- Initialization wpp ConnectivityMonitoring ----" << endl;
	conn_mon.init(registry.connectivityMonitoring());
	registry.registerObj(registry.connectivityMonitoring());
	#endif
	#if OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
	cout << endl << "---- Initialization wpp AccessControl ----" << endl;
	accessCtrl.init(registry.lwm2mAccessControl());
	registry.registerObj(registry.lwm2mAccessControl());
	#endif
	#if OBJ_O_5_FIRMWARE_UPDATE_V11
	cout << endl << "---- Initialization wpp FirmwareUpdate ----" << endl;
	fwUpd.init(registry.firmwareUpdate());
	registry.registerObj(registry.firmwareUpdate());
	#endif
	
	// Giving ownership to registry
	client->giveOwnership();

	cout << endl << "---- Starting Connection thread ----" << endl;
	thread my_thread(socketPolling, &connection, &device);

	time_t callTime = 0;
	for (int iterationCnt = 0; !device.isNeededReboot(); iterationCnt++) {
		time_t currTime = time(NULL);

		cout << endl << "---- iteration:" << iterationCnt << " ----" << endl;
		if (currTime >= callTime || connection.getPacketQueueSize()) {
			// Handle client state and process packets from the server
			client = WppClient::takeOwnership();
			if (client) { 
				callTime = currTime + client->loop();
				client->giveOwnership();
				cout << "Sleep time: " << callTime - time(NULL) << endl;
			}
		}
		this_thread::sleep_for(chrono::seconds(1));
	}

	cout << endl << "---- Closing example ----" << endl;
	my_thread.join();
	WppClient::remove();
	
	return 0;
}

