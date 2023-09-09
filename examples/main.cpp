//============================================================================
// Name        : ObjectRegestry.cpp
// Author      : Valentin
// Version     :
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>

#include "WppClient.h"

#include "Connection.h"
#include "Platform.h"

using namespace std;
using namespace wpp;

void memConsumptionCheck() {
//   std::shared_ptr<uint8_t> shared_array(new uint8_t[10], std::default_delete<uint8_t[]>());
//   cout << "Object<Server>: " << sizeof(Object<Server>) << endl;
//   cout << "Object<Server> serverObj: " << sizeof(serverObj) << endl;
//   cout << "Server: " << sizeof(Server(1)) << endl;
//   cout << "Resource: " << sizeof(Resource{}) << endl;
//   cout << "Operation: " << sizeof(Operation{}) << endl;
//   cout << "IS_MANDATORY: " << sizeof(IS_MANDATORY) << endl;
//   cout << "IS_SINGLE: " << sizeof(IS_SINGLE) << endl;
//   cout << "DATA_TYPE: " << sizeof(Resource::DATA_TYPE) << endl;
//   cout << "DATA_T: " << sizeof(Resource::DATA_T) << endl;
//   cout << "mutex: " << sizeof(mutex) << endl;
//   cout << "std::unordered_map<ID_T, DATA_T>: " << sizeof(std::unordered_map<ID_T, Resource::DATA_T>) << endl;
//   cout << "std::vector<DATA_T>: " << sizeof(std::vector<Resource::DATA_T>) << endl;
//   cout << "DATA_VERIFIER_T: " << sizeof(Resource::DATA_VERIFIER_T) << endl;
}

void objRestore(Lwm2mObject &object) {
	cout << "Restore object id: " << (ID_T)object.getObjectId() << ", name: " << object.getObjectInfo().name << endl;
}

int main()
{
	cout << "Test memory consumption:" << endl;
	memConsumptionCheck();

	Platform platform;
	Connection connection;
	WppRegistry registry {objRestore};
	WppClient::create({"Test name", "", ""}, registry, connection, platform);

	cout << endl << "Test WppRegistry:" << endl;
	Server *server = registry.server().createInstance();

	INT_T value;
	server->get(value, Server::SHORT_SERV_ID);
	cout << "Resource Server::SHORT_SERV_ID read: " << value << endl;

	EXECUTE_T execute;
	cout << "Resource Server::DISABLE execute: " << endl;
	server->get(execute, Server::DISABLE);
	execute(Server::SHORT_SERV_ID, OPAQUE_T());



	cout << endl << "Test WppClient:" << endl;
	int iterationCnt = 10;
	while (iterationCnt--) {
		time_t sleepTime = 1;
		WppClient::client()->loop(sleepTime);

		cout << "Iteration: " << iterationCnt << ", sleep: " << sleepTime << endl;
		std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
	}
}

