//============================================================================
// Name        : ObjectRegestry.cpp
// Author      : Valentin
// Version     :
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>

#include "WppClient.h"
#include "WppRegistry.h"
#include "IObjObserver.h"
#include "IInstObserver.h"

#include "Connection.h"

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
//   cout << "TYPE_ID: " << sizeof(Resource::TYPE_ID) << endl;
//   cout << "DATA_T: " << sizeof(Resource::DATA_T) << endl;
//   cout << "mutex: " << sizeof(mutex) << endl;
//   cout << "std::unordered_map<ID_T, DATA_T>: " << sizeof(std::unordered_map<ID_T, Resource::DATA_T>) << endl;
//   cout << "std::vector<DATA_T>: " << sizeof(std::vector<Resource::DATA_T>) << endl;
//   cout << "DATA_VERIFIER_T: " << sizeof(Resource::DATA_VERIFIER_T) << endl;
}

class ServerObserver: public IObjObserver<Server>, public IInstObserver<Server> {
	public:
	void objectRestore(Object<Server> &object) override {
		cout << "objectRestore: " << (ID_T)object.getObjectID() << endl;
		object.clear();
		object.createInstance();
	}

    void instanceCreated(Object<Server> &object, ID_T instanceId) override {
        cout << "instanceCreated: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
    }

    void instanceDeleting(Object<Server> &object, ID_T instanceId) override {
		cout << "instanceDeleting: " << (ID_T)object.getObjectID() << ":" << instanceId << endl;
	}

	virtual void resourceRead(Server &inst, const ResourceID &resId) override {
        cout << "resourceRead: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceWrite(Server &inst, const ResourceID &resId) override {
        cout << "resourceWrite: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }

    virtual void resourceExecute(Server &inst, const ResourceID &resId) override {
        cout << "resourceExecute: " << (ID_T)inst.getObjectID() << ":" << inst.getInstanceID() << ":" << resId.resId << ":" << resId.resInstId << endl;
    }
};

int main()
{
	cout << "Test memory consumption:" << endl;
	memConsumptionCheck();

	// Client initialization
	Connection connection;
	WppClient::create({"Test name", "", ""}, connection);
	
	WppClient *client = WppClient::takeOwnership();
	if (!client) {
		cout << endl << "ERR: WppClient can not be taken" << endl;
		return -1;
	}

	cout << endl << "Test WppRegistry:" << endl;
	WppRegistry &registry = client->registry();

	Object<Server> &serverObj = registry.server();
	ServerObserver observer;
	serverObj.subscribe(&observer);

	Server *server = serverObj.createInstance();
	server->subscribe(&observer);

	INT_T value;
	server->get(Server::SHORT_SERV_ID, value);
	cout << "Resource Server::SHORT_SERV_ID read: " << value << endl;

	EXECUTE_T execute;
	cout << "Resource Server::DISABLE execute: " << endl;
	server->get(Server::DISABLE, execute);
	execute(Server::SHORT_SERV_ID, OPAQUE_T());

	// Giving ownership to registry
	client->giveOwnership();

	cout << endl << "Test WppClient:" << endl;
	int iterationCnt = 10;
	while (iterationCnt--) {
		time_t sleepTime = 1;

		client = WppClient::takeOwnership();
		if (client) client->loop(sleepTime);
		client->giveOwnership();

		cout << "Iteration: " << iterationCnt << ", sleep: " << sleepTime << endl;
		std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
	}
}

