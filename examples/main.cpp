//============================================================================
// Name        : WppObjectRegestry.cpp
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

#include "WppClient.h"
#include "WppRegistry.h"

using namespace std;

void memConsumptionCheck() {
//   std::shared_ptr<uint8_t> shared_array(new uint8_t[10], std::default_delete<uint8_t[]>());
//   cout << "WppWppObject<Server>: " << sizeof(WppWppObject<Server>) << endl;
//   cout << "WppWppObject<Server> serverObj: " << sizeof(serverObj) << endl;
//   cout << "Server: " << sizeof(Server(1)) << endl;
//   cout << "Resource: " << sizeof(Resource{}) << endl;
//   cout << "Operation: " << sizeof(Operation{}) << endl;
//   cout << "IS_MANDATORY: " << sizeof(IS_MANDATORY) << endl;
//   cout << "IS_SINGLE: " << sizeof(IS_SINGLE) << endl;
//   cout << "TYPE_ID: " << sizeof(Resource::TYPE_ID) << endl;
//   cout << "DATA_T: " << sizeof(Resource::DATA_T) << endl;
//   cout << "mutex: " << sizeof(mutex) << endl;
//   cout << "std::unordered_map<ID_T, DATA_T>: " << sizeof(std::unordered_map<ID_T, Resource::DATA_T>) << endl;
//   cout << "std::vector<DATA_T>: " << sizeof(std::vector<Resource::DATA_T>) << endl;
//   cout << "DATA_VERIFIER_T: " << sizeof(Resource::DATA_VERIFIER_T) << endl;
}

void socketPolling(Connection *connection) {
	while (true) {
		connection->loop();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main()
{
	// cout << "Test memory consumption:" << endl;
	// memConsumptionCheck();

	cout << endl << "------------------- Creating requiered components -------------------" << endl;
	Connection connection ("56830", AF_INET);
	ServerImpl server;
	SecurityImpl security;
	DeviceImpl device;

	// Client initialization
	cout << endl << "------------------- Creating WppClient -------------------" << endl;
	wpp::WppClient::create({"Test name", "", ""}, connection);
	wpp::WppClient *client = wpp::WppClient::takeOwnership();
	if (!client) {
		cout << endl << "ERR: WppClient can not be taken" << endl;
		return -1;
	}
	wpp::WppRegistry &registry = client->registry();

	// Initialize wpp objects
	cout << endl << "------------------- Initialization wpp Server -------------------" << endl;
	server.init(registry.server());
	cout << endl << "------------------- Initialization wpp Security -------------------" << endl;
	security.init(registry.security());
	cout << endl << "------------------- Initialization wpp Device -------------------" << endl;
	device.init(registry.device());

	// Giving ownership to registry
	client->giveOwnership();

	cout << endl << "------------------- Starting Connection thread -------------------" << endl;
	std::thread my_thread(socketPolling, &connection);

	time_t callTime = 0;
	for (int iterationCnt = 0; true; iterationCnt++) {
		time_t currTime = std::time(NULL);

		cout << endl << "------------------- iteration:" << iterationCnt << " -------------------" << endl;
		if (currTime >= callTime || connection.getPacketQueueSize()) {
			// Handle client state and process packets from the server
			client = wpp::WppClient::takeOwnership();
			if (client) { 
				callTime = currTime + client->loop();
				client->giveOwnership();
				cout << "Sleep time: " << callTime - std::time(NULL) << endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

