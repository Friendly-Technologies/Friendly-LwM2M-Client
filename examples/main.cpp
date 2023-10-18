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
using namespace wpp;

void* operator new(std::size_t sz) {
	void *ptr = std::malloc(sz);
	cout << endl << "[_NEW_:" << hex << ptr << dec << ":" << sz << "]" << endl;
	return ptr;
}

void* operator new[](std::size_t sz) {
  	void *ptr = std::malloc(sz);
	cout << endl << "[_NEW_:" << hex << ptr << dec << ":" << sz << "]" << dec << endl;
	return ptr;
}

void operator delete(void* ptr) noexcept {
	cout << endl << "[_DELETE_:" << hex << ptr << "]" << dec << endl;
  	std::free(ptr);
}

void operator delete[](void* ptr) noexcept {
	cout << endl << "[_DELETE_:" << hex << ptr << "]" <<  dec << endl;
  	std::free(ptr);
}

#include <map>
#include <list>

void mem_consumption_test() {
	cout << endl << "Memory consumption test start" << endl;

	cout << "Resource size = " << sizeof(Resource) << endl;
	cout << "ID_T size = " << sizeof(ID_T) << endl;
	cout << "Resource::DATA_T size = " << sizeof(Resource::DATA_T) << endl;
	cout << "Resource::DATA_VERIFIER_T size = " << sizeof(Resource::DATA_VERIFIER_T) << endl;
	cout << "ResInstance size = " << sizeof(Resource::ResInst) << endl;

	lwm2m_context_t ctx;
	// cout << "Lwm2mSecurity size = " << sizeof(wpp::Lwm2mSecurity) << endl;
	// Lwm2mSecurity sec = Lwm2mSecurity(ctx, {});
	// cout << "Lwm2mServer size = " << sizeof(wpp::Lwm2mServer) << endl;
	// Lwm2mServer serv = Lwm2mServer(ctx, {});
	// cout << "Device size = " << sizeof(wpp::Device) << endl;
	// Device dev = Device(ctx, {});

	cout << "Object<Lwm2mSecurity> size = " << sizeof(Object<Lwm2mSecurity>) << endl;
	Object<Lwm2mSecurity>::create(ctx, LWM2M_SECURITY_OBJ_INFO);
	Object<Lwm2mSecurity>::object()->createInstance();
	// Connection connection("56830", AF_INET);
	// cout << "WppClient size = " << sizeof(wpp::WppClient) << endl;
	// WppClient::create({"asdasd", "", ""}, connection);

	// cout << "vector<ResInstance> size = " << sizeof(vector<ResInstance>) << endl;
	// vector<ResInstance> _resInstances;
	// _resInstances.push_back({0, true});
	// _resInstances.push_back({1, true});
	// _resInstances.push_back({2, true});

	// cout << "list<ResInstance> size = " << sizeof(list<ResInstance>) << endl;
	// list<ResInstance> _resInstances;
	// _resInstances.push_back({0, true});
	// _resInstances.push_back({1, true});
	// _resInstances.push_back({2, true});

	// cout << "std::map<ID_T, Resource::DATA_T> size = " << sizeof(std::map<ID_T, Resource::DATA_T>) << endl;
	// std::map<ID_T, Resource::DATA_T> _instances;
	// _instances[0] = true;
	// _instances[1] = true;
	// _instances[2] = true;

	// cout << "std::unordered_map<ID_T, Resource::DATA_T> size = " << sizeof(std::unordered_map<ID_T, Resource::DATA_T>) << endl;
	// std::unordered_map<ID_T, Resource::DATA_T> _instances;
	// _instances[0] = true;
	// _instances[1] = true;
	// _instances[2] = true;

	// cout << "Resource BOOL" << endl;
	// Resource resBool = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::BOOL};
	// resBool.set(true,0);
	// resBool.set(true,1);

	// cout << "Resource INT" << endl;
	// Resource resInt = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::INT};
	// resInt.setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return -1 <= value && value <= 10; });
	// resInt.set((INT_T)0,0);
	// resInt.set((INT_T)0,1);
	
	// cout << "Resource UINT" << endl;
	// Resource resUint = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::UINT};
	// resUint.set((UINT_T)0,0);
	// resUint.set((UINT_T)0,1);
	
	// cout << "Resource FLOAT" << endl;
	// Resource resFloat = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::FLOAT};
	// resFloat.set((FLOAT_T)0,0);
	// resFloat.set((FLOAT_T)0,1);

	// cout << "Resource OBJ_LINK" << endl;
	// Resource resObjLink = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::OBJ_LINK};
	// resObjLink.set(OBJ_LINK_T(),0);
	// resObjLink.set(OBJ_LINK_T(),1);
	
	// cout << "Resource TIME" << endl;
	// Resource resTime = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::TIME};
	// resTime.set(TIME_T(0),0);
	// resTime.set(TIME_T(0),1);

	// cout << "Resource OPAQUE" << endl;
	// Resource resOpaque = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::OPAQUE};
	// resOpaque.set(OPAQUE_T(),0);
	// resOpaque.set(OPAQUE_T(),1);
	
	// cout << "Resource STRING" << endl;
	// Resource resString = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::STRING};
	// resString.set(STRING_T(),0);
	// resString.set(STRING_T(),1);
	
	// cout << "Resource CORE_LINK" << endl;
	// Resource resCoreLink = {0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY,TYPE_ID::CORE_LINK};
	// resCoreLink.set(CORE_LINK_T(),0);
	// resCoreLink.set(CORE_LINK_T(),1);

	cout << "Memory consumption test end" << endl << endl;
}

void socketPolling(Connection *connection, DeviceImpl *device) {
	while (!device->isNeededReboot()) {
		connection->loop();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main() {
	// mem_consumption_test();
	// return 0;

	cout << endl << "---- Creating requiered components ----" << endl;
	Connection connection("56830", AF_INET);
	ServerImpl server;
	SecurityImpl security;
	DeviceImpl device;

	// Client initialization
	cout << endl << "---- Creating WppClient ----" << endl;
	string clientName = "SinaiTestLwm2m";
	#if DTLS_WITH_PSK
	clientName += "PSK";
	#elif DTLS_WITH_RPK
	clientName += "RPK";
	#endif

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

