/*
 * Lwm2mServer
 * Generated on: 2023-11-02 16:41:01
 * Created by: Sinai RnD
 */

#ifndef WPP_M_1_LWM2M_SERVER_H
#define WPP_M_1_LWM2M_SERVER_H

#include "Lwm2mServerConfig.h"
#include "Lwm2mServerInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
#include "WppTaskQueue.h"
#include "WppBindings.h"
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class Lwm2mServer : public Instance {
public:
	enum ID: ID_T {
		SHORT_SERVER_ID_0 = 0,
		LIFETIME_1 = 1,
		#if RES_1_2
		DEFAULT_MINIMUM_PERIOD_2 = 2,
		#endif
		#if RES_1_3
		DEFAULT_MAXIMUM_PERIOD_3 = 3,
		#endif
		#if RES_1_4
		DISABLE_4 = 4,
		#endif
		#if RES_1_5
		DISABLE_TIMEOUT_5 = 5,
		#endif
		NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6 = 6,
		BINDING_7 = 7,
		REGISTRATION_UPDATE_TRIGGER_8 = 8,
		#if RES_1_9
		BOOTSTRAP_REQUEST_TRIGGER_9 = 9,
		#endif
		#if RES_1_10
		APN_LINK_10 = 10,
		#endif
		#if RES_1_11
		TLS_DTLS_ALERT_CODE_11 = 11,
		#endif
		#if RES_1_12
		LAST_BOOTSTRAPPED_12 = 12,
		#endif
		#if RES_1_13
		REGISTRATION_PRIORITY_ORDER_13 = 13,
		#endif
		#if RES_1_14
		INITIAL_REGISTRATION_DELAY_TIMER_14 = 14,
		#endif
		#if RES_1_15
		REGISTRATION_FAILURE_BLOCK_15 = 15,
		#endif
		#if RES_1_16
		BOOTSTRAP_ON_REGISTRATION_FAILURE_16 = 16,
		#endif
		#if RES_1_17
		COMMUNICATION_RETRY_COUNT_17 = 17,
		#endif
		#if RES_1_18
		COMMUNICATION_RETRY_TIMER_18 = 18,
		#endif
		#if RES_1_19
		COMMUNICATION_SEQUENCE_DELAY_TIMER_19 = 19,
		#endif
		#if RES_1_20
		COMMUNICATION_SEQUENCE_RETRY_COUNT_20 = 20,
		#endif
		#if RES_1_21
		TRIGGER_21 = 21,
		#endif
		#if RES_1_22
		PREFERRED_TRANSPORT_22 = 22,
		#endif
		#if RES_1_23
		MUTE_SEND_23 = 23,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

public:
	Lwm2mServer(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Lwm2mServer();

	/* --------------- Helpful methods --------------- */
	/**
	 * @brief Gets the Object reference.
	 * @param ctx - WppClient context.
	 * @return A reference to the Object.
	 */
	static Object & object(WppClient &ctx);

	/**
	 * @brief Gets an instance of the object.
	 * @param ctx - WppClient context.
	 * @param instId The instance ID. If not provided, the first available instance is returned.
	 * @return A pointer to the Instance object or NULL.
	 */
	static Lwm2mServer * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static Lwm2mServer * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Removes an instance of the object.
	 * @param ctx - WppClient context.
	 * @param instId The instance ID.
	 * @return True if the instance was successfully removed, false otherwise.
	 */
	static bool removeInst(WppClient &ctx, ID_T instId);

	/* --------------- Code_h block 3 start --------------- */
	/* --------------- Code_h block 3 end --------------- */

protected:
	/* --------------- Instance implementation part --------------- */
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) override;

	/* --------------- Code_h block 4 start --------------- */
	/* --------------- Code_h block 4 end --------------- */

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Creates resources without initializing.
	 */
	void resourcesCreate();
	/*
	 * Initialize resources with default values
	 * SINGLE resource always must have at least one instance.
	 */	
	void resourcesInit();
	
	/* --------------- Code_h block 5 start --------------- */
	/* --------------- Code_h block 5 end --------------- */

private:
	/* --------------- Class private properties --------------- */
	/* --------------- Code_h block 6 start --------------- */
	WppTaskQueue::task_id_t _requestBootstrapTaskId;
	WppTaskQueue::task_id_t _requestDeregistrationTaskId;
	/* --------------- Code_h block 6 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_M_1_LWM2M_SERVER_H */
