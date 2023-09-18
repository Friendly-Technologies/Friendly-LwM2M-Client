/*
 * IWppConnection.h
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#ifndef IWppConnection_H_
#define IWppConnection_H_

#include <string>

//TODO: #include "liblwm2m.h"
#include "dep.h"
#include "mandatory/security/Security.h"
#include "SafeQueue.h"

#define WPP_CONN_I_PACKETS_QUEUE_SIZE 10

namespace wpp {

class WppClient;

class IWppConnection {
	friend class WppClient;

public:
	using SESSION_T = void*;

	struct Packet {
		SESSION_T session;
		size_t length;
		uint8_t *buffer;
	};

public:
	IWppConnection();
	virtual ~IWppConnection();

	IWppConnection(const IWppConnection&) = delete;
	IWppConnection(IWppConnection&&) = delete;
	IWppConnection& operator=(const IWppConnection&) = delete;
	IWppConnection& operator=(IWppConnection&&) = delete;

	/* ------------- Connection abilities ------------- */
	/*
	 * Virtual methods must be implemented by user and
	 * will be called by wpp core.
	 */
	virtual SESSION_T connect(Security& security) = 0;
	virtual void disconnect(SESSION_T session) = 0;
	virtual bool sessionCmp(SESSION_T session1, SESSION_T session2) = 0;
	/*
	 * Deleting the memory occupied by the package will be done in wpp core.
	 */
	virtual bool sendPacket(Packet packet) = 0;

	/*
	 * Interface for add packets to queue and get information about queue size.
	 * Packets that were added to queue will be processed by wpp core in WppClient::loop().
	 * Since messages from the server are mostly related to changes in the internal state
	 * of the registry, their processing can be started only when the registry is owned by
	 * WppClient. This approach will allow the user to use this interface from interrupts
	 * or other streams without losing messages and with ownershiped WppRegistry.
	 * 
	 * The memory occupied by the package must be deleted by the user, because the method
	 * does not save a pointer to the package data, but copies it.
	 */
	bool addPacketToQueue(Packet packet);
	uint8_t getPacketQueueSize();

	/*
	 * TODO: It can lead to memory double-free if
	 * simultaneously called with handlePacketsInQueue().
	 */
	void clearPacketQueue();

	/*
	 * Interface for setup and get information about COAP block size
	 */
	bool setDataBlockSize(uint16_t size);
	uint16_t getDataBlockSize();

private:
	/*
	* This method is called by AppClient in loop() if the queue contains at least one message.
	* And processes all the messages found in the queue. If an error occurred during the
	* processing of the package, it is deleted.
	*/
	void handlePacketsInQueue(lwm2m_context_t *context);

private:
	SafeQueue<Packet, WPP_CONN_I_PACKETS_QUEUE_SIZE> packets;
};

} /* namespace wpp */

#endif /* IWppConnection_H_ */
