/*
 * WppConnection.h
 *
 *  Created on: 22 Jul 2023
 *      Author: valentin
 */

#ifndef I_WPP_CONNECTION_H_
#define I_WPP_CONNECTION_H_

#include <string>

#include "liblwm2m.h"
#include "SafeQueue.h"

#define WPP_CONN_I_PACKETS_QUEUE_SIZE 10

/**
 * @brief The WppConnection class represents a connection interface for the Wpp library.
 * 
 * This class provides an abstract interface for establishing and managing connections in the Wpp library.
 * It defines pure virtual methods that must be implemented by the user to establish a connection, disconnect from a connection,
 * compare session identifiers, and send packets over the connection.
 * 
 * The WppConnection class also provides methods for adding packets to a queue, retrieving the size of the packet queue,
 * clearing the packet queue, and setting/getting the COAP block size.
 * 
 * @note This class is meant to be subclassed by the user to provide the implementation for the virtual methods.
 */
namespace wpp {

class WppClient;
class Lwm2mSecurity;

class WppConnection {
public:
	using SESSION_T = void*;

	struct Packet {
		SESSION_T session;
		size_t length;
		uint8_t *buffer;
	};

public:
	WppConnection();
	virtual ~WppConnection();

	WppConnection(const WppConnection&) = delete;
	WppConnection(WppConnection&&) = delete;
	WppConnection& operator=(const WppConnection&) = delete;
	WppConnection& operator=(WppConnection&&) = delete;

	/* ------------- Connection abilities ------------- */
	/**
	 * @brief Establishes a connection using the provided Lwm2mSecurity object.
	 * 
	 * This method must be implemented by the user to establish a connection using the provided Lwm2mSecurity object.
	 * The method should return a session identifier that represents the established connection.
	 * 
	 * @param security The Lwm2mSecurity object used to establish the connection.
	 * @return The session identifier representing the established connection.
	 */
	virtual SESSION_T connect(Lwm2mSecurity& security) = 0;

	/**
	 * @brief Disconnects from the specified session.
	 * 
	 * This method must be implemented by the user to disconnect from the specified session.
	 * 
	 * @param session The session identifier representing the connection to disconnect from.
	 */
	virtual void disconnect(SESSION_T session) = 0;

	/**
	 * @brief Compares two session identifiers.
	 * 
	 * This method must be implemented by the user to compare two session identifiers.
	 * It should return true if the session identifiers are equal, and false otherwise.
	 * 
	 * @param session1 The first session identifier to compare.
	 * @param session2 The second session identifier to compare.
	 * @return true if the session identifiers are equal, false otherwise.
	 */
	virtual bool sessionCmp(SESSION_T session1, SESSION_T session2) = 0;

	/**
	 * @brief Sends a packet over the connection.
	 * 
	 * This method must be implemented by the user to send a packet over the connection.
	 * The method should return true if the packet was successfully sent, and false otherwise.
	 * 
	 * @param packet The packet to be sent over the connection.
	 * @return true if the packet was successfully sent, false otherwise.
	 */
	virtual bool sendPacket(const Packet &packet) = 0;

	/**
	 * @brief Adds a packet to the packet queue.
	 * 
	 * This method adds a packet to the packet queue for processing by the WppClient::loop() method.
	 * The packet will be copied into the queue, so the user is responsible for deleting the memory occupied by the packet.
	 * 
	 * @param packet The packet to be added to the queue.
	 * @return true if the packet was successfully added to the queue, false otherwise.
	 */
	bool addPacketToQueue(const Packet &packet);

	/**
	 * @brief Retrieves the size of the packet queue.
	 * 
	 * This method returns the size of the packet queue.
	 * 
	 * @return The size of the packet queue.
	 */
	uint8_t getPacketQueueSize();

	/**
	 * @brief Clears the packet queue.
	 * 
	 * This method clears the packet queue, deleting all the packets in the queue.
	 */
	void clearPacketQueue();

	/**
	 * @brief Sets the COAP block size.
	 * 
	 * This method sets the COAP block size to the specified value.
	 * 
	 * @param size The COAP block size to set.
	 * @return true if the COAP block size was successfully set, false otherwise.
	 */
	bool setDataBlockSize(uint16_t size);

	/**
	 * @brief Retrieves the COAP block size.
	 * 
	 * This method retrieves the currently set COAP block size.
	 * 
	 * @return The currently set COAP block size.
	 */
	uint16_t getDataBlockSize();

	/**
	 * @brief Processes the packets in the packet queue.
	 * 
	 * This method is called by the WppClient in its loop() method if the packet queue contains at least one packet.
	 * It processes all the packets found in the queue. If an error occurs during the processing of a packet, it is deleted.
	 * But it can be also called by the user if he wants to process immediately.
	 * 
	 * @param client The WppClient object used for packet processing.
	 */
	void handlePacketsInQueue(WppClient &client);

private:
	SafeQueue<Packet, WPP_CONN_I_PACKETS_QUEUE_SIZE> packets;
};

} /* namespace wpp */

#endif /* I_WPP_CONNECTION_H_ */
