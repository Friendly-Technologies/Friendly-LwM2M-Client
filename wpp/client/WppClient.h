/*
 * WppManager.h
 *
 *  Created on: 20 Jul 2023
 *      Author: valentin
 */

#ifndef WPP_CLIENT_H_
#define WPP_CLIENT_H_

#include <string>
#include <functional>

#include "liblwm2m.h"
#include "WppTypes.h"
#include "WppRegistry.h"
#include "WppConnection.h"
#include "WppGuard.h"

#define WPP_CLIENT_MAX_SLEEP_TIME_S	60

namespace wpp {

/**
 * @class WppClient
 * @brief Represents a client interface for Wpp library.
 *
 * The WppClient class provides methods for managing the Wpp library, accessing its components,
 * and processing the state of the Wakaama core. It allows users to create, remove, and check
 * if the WppClient is created. Users can also take ownership of the client, interact with its
 * connection and registry, and perform server registration and deregistration.
 *
 * @note To interact with the WppClient, it is necessary to take ownership of it. Only one actor
 * can have access to the client at a time to synchronize access to the registry and avoid
 * simultaneous writing, reading, or deleting of Instance or Resource.
 */
class WppClient {
public:
	/**
	 * @struct ClientInfo
	 * @brief Represents the information required to create a WppClient.
	 */
	struct ClientInfo {
		std::string endpointName; /**< The endpoint name of the client. */
		std::string msisdn; /**< The MSISDN of the client. */
		std::string altPath; /**< The alternative path of the client. */
	};

	using WppErrHandler = std::function<void(WppClient &client, int errCode)>;

private:
	/**
	 * @brief Constructs a WppClient object with the given connection and maximum sleep time.
	 * @param connection The WppConnection object to be associated with the client.
	 */
	WppClient(WppConnection &connection, WppErrHandler errHandler = NULL);
	
	/**
	 * @brief Unused constructors of WppClient.
	 */
	WppClient(const WppClient&) = delete;
	WppClient(WppClient&&) = delete;
	WppClient& operator=(const WppClient&) = delete;
	WppClient& operator=(WppClient&&) = delete;

public:
	~WppClient();

	/* ------------- WppClient management ------------- */

	/**
	 * @brief Creates a WppClient with the specified client information, connection, and maximum sleep time.
	 * @param info The client information required to create the WppClient.
	 * @param connection The WppConnection object to be associated with the client.
	 * @return True if the WppClient is created successfully, false otherwise.
	 */
	static bool create(const ClientInfo &info, WppConnection &connection, WppErrHandler errHandler = NULL);

	/**
	 * @brief Removes the WppClient.
	 */
	static void remove();

	/**
	 * @brief Checks if the WppClient is created.
	 * @return True if the WppClient is created, false otherwise.
	 */
	static bool isCreated();

	/**
	 * @brief Takes ownership of the WppClient.
	 * @return A pointer to the WppClient if available, or NULL if the client is occupied by another actor or uninitialized.
	 */
	static WppClient* takeOwnership();

	/**
	 * @brief Takes ownership of the WppClient, blocking until it becomes available.
	 * @return A pointer to the WppClient if available, or NULL if the client is uninitialized.
	 */
	static WppClient* takeOwnershipBlocking();

	/**
	 * @brief Gives up ownership of the WppClient.
	 */
	void giveOwnership();

	/* ------------- WppClient components ------------- */

	/**
	 * @brief Gets the WppConnection associated with the WppClient.
	 * @return The WppConnection object.
	 */
	WppConnection & connection();

	/**
	 * @brief Gets the WppRegistry associated with the WppClient.
	 * @return The WppRegistry object.
	 */
	WppRegistry & registry();

	/* ------------- Wakaama core state processing ------------- */

	/**
	 * @brief Gets the state of the Wakaama client.
	 * @return The state of the client as lwm2m_client_state_t.
	 */
	lwm2m_client_state_t getState();

	/**
	 * @brief Gets the LwM2M context associated with the WppClient.
	 * @return The LwM2M context object.
	 */
	lwm2m_context_t & getContext();

	/**
	 * @brief Processes the state of the Wakaama core.
	 * This function performs the necessary work by the Wakaama core, such as (re)sending packets,
	 * handles received packets from servers, and created tasks.
	 * @return The time interval after which the function should be called again.
	 */
	time_t loop();

	/* ------------- WppClient server operations ------------- */
	
	/**
	 * @brief Deregisters the client from the servers.
	 */
	void deregister();

	#if defined(LWM2M_SUPPORT_SENML_JSON) && RES_1_23
	/**
	 * @brief Sending occurs immediately, without checking the response from the server,
	 * checking only the parameters. Data is sent to all servers with which a successful
	 * connection is established, the exception serves only to explicitly disable the send
	 * operation through resource 23 in the object 1.
	 * @param link The link to object/instance/resource to send.
	 * @return True if the data link is correct and sent successfully, false otherwise.
	 */
	bool send(const DataLink &link);
	#endif // LWM2M_SUPPORT_SENML_JSON && RES_1_23

private:
	/* ------------- Wakaama client initialisation ------------- */

	/**
	 * @brief Opens the LwM2M context.
	 * @return True if the context is opened successfully, false otherwise.
	 */
	bool lwm2mContextOpen();

	/**
	 * @brief Closes the LwM2M context.
	 */
	void lwm2mContextClose();

	/**
	 * @brief Configures the LwM2M client with the specified endpoint name, MSISDN, and alternative path.
	 * @param endpointName The endpoint name of the client.
	 * @param msisdn The MSISDN of the client.
	 * @param altPath The alternative path of the client.
	 * @return True if the client is configured successfully, false otherwise.
	 */
	bool lwm2mConfigure(const std::string &endpointName, const std::string &msisdn, const std::string &altPath);

private:
	static WppGuard _clientGuard;
	static WppClient *_client;

	WppConnection &_connection;
	WppRegistry *_registry;
	WppErrHandler _errHandler;
	lwm2m_context_t *_lwm2m_context;
};

} /* namespace wpp */
#endif /* WPP_CLIENT_H_ */
