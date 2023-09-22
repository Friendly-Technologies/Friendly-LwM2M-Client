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

#include "WppClient.h"
#include "WppRegistry.h"

using namespace std;

void memConsumptionCheck() {
//   std::shared_ptr<uint8_t> shared_array(new uint8_t[10], std::default_delete<uint8_t[]>());
//   cout << "Object<Server>: " << sizeof(Object<Server>) << endl;
//   cout << "Object<Server> serverObj: " << sizeof(serverObj) << endl;
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

int main()
{
	cout << "Test memory consumption:" << endl;
	memConsumptionCheck();

	Connection connection;
	ServerImpl server;
	SecurityImpl security;
	DeviceImpl device;

	// Client initialization
	wpp::WppClient::create({"Test name", "", ""}, connection);
	wpp::WppClient *client = wpp::WppClient::takeOwnership();
	if (!client) {
		cout << endl << "ERR: WppClient can not be taken" << endl;
		return -1;
	}
	wpp::WppRegistry &registry = client->registry();

	// Initialize wpp objects
	server.init(registry.server());
	security.init(registry.security());
	device.init(registry.device());

	// Giving ownership to registry
	client->giveOwnership();

	int iterationCnt = 10;
	while (iterationCnt--) {
		time_t sleepTime = 1;

		client = wpp::WppClient::takeOwnership();
		if (client) client->loop(sleepTime);
		client->giveOwnership();

		cout << "Iteration: " << iterationCnt << ", sleep: " << sleepTime << endl;
		std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
	}
}

