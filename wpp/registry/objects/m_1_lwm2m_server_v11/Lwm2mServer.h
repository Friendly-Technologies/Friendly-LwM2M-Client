/*
 * Lwm2mServer
 * Generated on: 2023-11-02 16:41:01
 * Created by: SinaiR&D
 */

#ifndef WPP_M_1_LWM2M_SERVER_V11_H
#define WPP_M_1_LWM2M_SERVER_V11_H

#include "Lwm2mServerConfig.h"
#include "Lwm2mServerInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
#include "WppTaskQueue.h"
#include "WppBindings.h"
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

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

	/* --------------- Code_h block 1 start --------------- */
	/* --------------- Code_h block 1 end --------------- */

public:
	Lwm2mServer(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Lwm2mServer();

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

protected:
	/* --------------- Instance implementation part --------------- */
	/*
	 * Reset all resources values and internal state to default.
	 */
	void setDefaultState() override;
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) override;

	/* --------------- Code_h block 3 start --------------- */
	/* --------------- Code_h block 3 end --------------- */

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Creates resources without initializing.
	 */
	void resourcesCreate();
	/*
	 * Initialize resources with default values
	 * Resource always must have at least one instance.
	 * Note: From server side, empty resource == undefined resource.
	 */	
	void resourcesInit();
	
	/* --------------- Code_h block 4 start --------------- */
	/* --------------- Code_h block 4 end --------------- */

private:
	/* --------------- Class private properties --------------- */
	/* --------------- Code_h block 5 start --------------- */
	WppTaskQueue::task_id_t _requestBootstrapTaskId;
	WppTaskQueue::task_id_t _requestDeregistrationTaskId;
	/* --------------- Code_h block 5 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_M_1_LWM2M_SERVER_V11_H */
