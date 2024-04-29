//============================================================================
// Name        : main.cpp
// Author      : Valentin
// Version     :
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>

#include "Lwm2mServer.h"
#include "Lwm2mSecurity.h"
#include "Device.h"
#include "Connection.h"
#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
#include "ConnectivityMonitoring.h"
#endif
#ifdef OBJ_O_5_FIRMWARE_UPDATE
#include "FirmwareUpdate.h"
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

void initAudioClipObj(WppClient &client) {
	#ifdef OBJ_O_3339_AUDIO_CLIP
	AudioClip::createInst(client);
	client.registry().registerObj(AudioClip::object(client));
	#if OBJ_O_2_LWM2M_ACCESS_CONTROL
	Lwm2mAccessControl::create(AudioClip::object(client), Lwm2mAccessControl::ALL_OBJ_RIGHTS);
	Lwm2mAccessControl::create(*AudioClip::instance(client), TEST_SERVER_SHORT_ID);
	#endif
	#endif
}

// Found Wakaama bugs:
// TODO: Device work with NON confirmation messages

int main() {
	cout << endl << "---- Creating requiered components ----" << endl;
	Connection connection("56830", AF_INET);
	Lwm2mServerImpl server;
	Lwm2mSecurityImpl security;
	DeviceImpl device;
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
	ConnectivityMonitoringImpl conn_mon;
	#endif
	#ifdef OBJ_O_5_FIRMWARE_UPDATE
	FirmwareUpdateImpl fwUpd;
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
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
	cout << endl << "---- Initialization wpp ConnectivityMonitoring ----" << endl;
	conn_mon.init(registry.connectivityMonitoring());
	registry.registerObj(registry.connectivityMonitoring());
	#endif
	#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
	cout << endl << "---- Initialization wpp AccessControl ----" << endl;
	registry.registerObj(registry.lwm2mAccessControl());
	#endif
	#ifdef OBJ_O_5_FIRMWARE_UPDATE
	cout << endl << "---- Initialization wpp FirmwareUpdate ----" << endl;
	fwUpd.init(registry.firmwareUpdate());
	registry.registerObj(registry.firmwareUpdate());
	#endif
	#ifdef OBJ_O_3339_AUDIO_CLIP
	cout << endl << "---- Initialization wpp AudioClip ----" << endl;
	initAudioClipObj(*client);
	#endif
	
	// Giving ownership to registry
	client->giveOwnership();

	// Add tasks with send operation
	#if defined(LWM2M_SUPPORT_SENML_JSON) && RES_1_23 && RES_3_13
	WppTaskQueue::addTask(5, [](WppClient &client, void *ctx) {
		WPP_LOGD(TAG_WPP_TASK, "Task: Send operation, sending current time to the server");
		DataLink dataLink = {{OBJ_ID::DEVICE, 0}, {Device::CURRENT_TIME_13,}};
		client.send(dataLink);
		return false;
	});
	#endif

	cout << endl << "---- Starting Connection thread ----" << endl;
	thread my_thread(socketPolling, &connection, &device);

	time_t callTime = 0;
	for (int iterationCnt = 0; !device.isNeededReboot(); iterationCnt++) {
		time_t currTime = time(NULL);

		cout << endl << "---- iteration:" << iterationCnt << ", time: " << time(NULL) << " ----" << endl;
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

