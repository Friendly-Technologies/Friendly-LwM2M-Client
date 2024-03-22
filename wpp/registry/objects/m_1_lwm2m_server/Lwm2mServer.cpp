/*
 * Lwm2mServer
 * Generated on: 2023-11-02 16:41:01
 * Created by: SinaiR&D
 */

#include "m_1_lwm2m_server/Lwm2mServer.h"

#include "Resource.h"
#include "ResOp.h"
#include "WppTypes.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
#if RES_1_11
#define TLS_DTLS_ALERT_CODE_MIN	0
#define TLS_DTLS_ALERT_CODE_MAX	255
#endif
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Lwm2mServer"

namespace wpp {

Lwm2mServer::Lwm2mServer(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	_requestBootstrapTaskId = WPP_ERR_TASK_ID;
	_requestDeregistrationTaskId = WPP_ERR_TASK_ID;
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Lwm2mServer::~Lwm2mServer() {
	/* --------------- Code_cpp block 3 start --------------- */
	WppTaskQueue::requestToRemoveTask(_requestBootstrapTaskId);
	WppTaskQueue::requestToRemoveTask(_requestDeregistrationTaskId);
	
	/* --------------- Code_cpp block 3 end --------------- */
}

void Lwm2mServer::setDefaultState() {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	_resources.clear();
	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void Lwm2mServer::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 6 start --------------- */
	if (type == ResOp::WRITE_REPLACE_INST || ((type & ResOp::WRITE) && resId.resId == LIFETIME_1)) {
		INT_T serverId;
		resource(SHORT_SERVER_ID_0)->get(serverId);
		lwm2m_update_registration(&getContext(), serverId, true, false);
	}
	/* --------------- Code_cpp block 6 end --------------- */

	operationNotify(*this, resId, type);

	/* --------------- Code_cpp block 7 start --------------- */
	/* --------------- Code_cpp block 7 end --------------- */
}

void Lwm2mServer::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 8 start --------------- */
	if (type == ResOp::WRITE && resId.resId == LIFETIME_1 && getContext().state == STATE_READY){
		INT_T serverId, lifetime;
		resource(SHORT_SERVER_ID_0)->get(serverId);
		resource(LIFETIME_1)->get(lifetime);
		lwm2m_update_server_lifetime(&getContext(), serverId, lifetime);
	}
	/* --------------- Code_cpp block 8 end --------------- */
}

/* --------------- Code_cpp block 9 start --------------- */
/* --------------- Code_cpp block 9 end --------------- */

void Lwm2mServer::resourcesCreate() {
	std::vector<Resource> resources = {
		{SHORT_SERVER_ID_0,                               ResOp(ResOp::READ),              IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT },      
		{LIFETIME_1,                                      ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT },      
		#if RES_1_2                                                                                                                                                                            
		{DEFAULT_MINIMUM_PERIOD_2,                        ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                                 
		#if RES_1_3                                                                                                                                                                            
		{DEFAULT_MAXIMUM_PERIOD_3,                        ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                                 
		#if RES_1_4                                                                                                                                                                            
		{DISABLE_4,                                       ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE },  
		#endif                                                                                                                                                                                 
		#if RES_1_5                                                                                                                                                                            
		{DISABLE_TIMEOUT_5,                               ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                                 
		{NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL },     
		{BINDING_7,                                       ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING },   
		{REGISTRATION_UPDATE_TRIGGER_8,                   ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE },  
		#if RES_1_9                                                                                                                                                                            
		{BOOTSTRAP_REQUEST_TRIGGER_9,                     ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE },  
		#endif                                                                                                                                                                                 
		#if RES_1_10                                                                                                                                                                           
		{APN_LINK_10,                                     ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::OBJ_LINK }, 
		#endif                                                                                                                                                                                 
		#if RES_1_11                                                                                                                                                                           
		{TLS_DTLS_ALERT_CODE_11,                          ResOp(ResOp::READ),              IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                                 
		#if RES_1_12                                                                                                                                                                           
		{LAST_BOOTSTRAPPED_12,                            ResOp(ResOp::READ),              IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },     
		#endif                                                                                                                                                                                 
		#if RES_1_13                                                                                                                                                                           
		{REGISTRATION_PRIORITY_ORDER_13,                  ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                                 
		#if RES_1_14                                                                                                                                                                           
		{INITIAL_REGISTRATION_DELAY_TIMER_14,             ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                                 
		#if RES_1_15                                                                                                                                                                           
		{REGISTRATION_FAILURE_BLOCK_15,                   ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL },     
		#endif                                                                                                                                                                                 
		#if RES_1_16                                                                                                                                                                           
		{BOOTSTRAP_ON_REGISTRATION_FAILURE_16,            ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL },     
		#endif                                                                                                                                                                                 
		#if RES_1_17                                                                                                                                                                           
		{COMMUNICATION_RETRY_COUNT_17,                    ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                                 
		#if RES_1_18                                                                                                                                                                           
		{COMMUNICATION_RETRY_TIMER_18,                    ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                                 
		#if RES_1_19                                                                                                                                                                           
		{COMMUNICATION_SEQUENCE_DELAY_TIMER_19,           ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                                 
		#if RES_1_20                                                                                                                                                                           
		{COMMUNICATION_SEQUENCE_RETRY_COUNT_20,           ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::UINT },     
		#endif                                                                                                                                                                                 
		#if RES_1_21                                                                                                                                                                           
		{TRIGGER_21,                                      ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL },     
		#endif                                                                                                                                                                                 
		#if RES_1_22                                                                                                                                                                           
		{PREFERRED_TRANSPORT_22,                          ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                                 
		#if RES_1_23                                                                                                                                                                           
		{MUTE_SEND_23,                                    ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::BOOL },     
		#endif                                                                                                                                                                                 
	};
	_resources = std::move(resources);
}

void Lwm2mServer::resourcesInit() {
	/* --------------- Code_cpp block 10 start --------------- */
	// TODO: The most part of the server resources logic must be implemented
	// on wakaama core level, because the Server is only a state holder and
	// at this level, it does not have the required information for doing
	// sings described in the documentation.

	resource(SHORT_SERVER_ID_0)->set(INT_T(0));
	resource(SHORT_SERVER_ID_0)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return SINGLE_INSTANCE_ID < value && value < ID_T_MAX_VAL; });

	resource(LIFETIME_1)->set(INT_T(0));

	#if RES_1_2    
	resource(DEFAULT_MINIMUM_PERIOD_2)->set(INT_T(0));                                                                                                                                                                                                             
	#endif    

	#if RES_1_3       
	resource(DEFAULT_MAXIMUM_PERIOD_3)->set(INT_T(0));                                                                                                                                                                                                          
	#endif                       

	// TODO: Disable (Res id 4) must be implemented by wakaama core or WppClient
	#if RES_1_4
	// Resource starts the separated task to deregistration from all currently registered servers. 
	// The registration proccess performs after deregistration immediatelly. If the object delete
	// before task execution, this task will be deleted at destructor.
	resource(DISABLE_4)->set((EXECUTE_T)[this](Instance& inst, ID_T resId, const OPAQUE_T& data) {
		if (!WppTaskQueue::isTaskExist(_requestDeregistrationTaskId)) {
			WPP_LOGI(TAG, "Deregistration Request Trigger: Deregistration is started");
			_requestDeregistrationTaskId = WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, [this](WppClient &client, void *ctx) -> bool {
				lwm2m_deregister(&getContext());
				return true;
			});
		} else {
			WPP_LOGI(TAG, "Deregistration Request Trigger: Deregistration is already in the progress");
		}
		return true;
	});
	#endif

	#if RES_1_5                                                                                                                                                                                                                        
	resource(DISABLE_TIMEOUT_5)->set(INT_T(0));
	#endif 

	// TODO: Notification Storing (Res id 6) must be implemented by wakaama core
	resource(NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6)->set(false);
	
	resource(BINDING_7)->set(STRING_T(""));
	resource(BINDING_7)->setDataVerifier((VERIFY_STRING_T)[](const STRING_T& value) { return wppBindingValidate(value); });

	// TODO: Registration Update (Res id 8) must be implemented by wakaama core
	resource(REGISTRATION_UPDATE_TRIGGER_8)->set((EXECUTE_T)[this](Instance& inst, ID_T resId, const OPAQUE_T& data) {
		INT_T serverId;
		resource(SHORT_SERVER_ID_0)->get(serverId);
		WPP_LOGI(TAG, "Registration Update Trigger: serverId -> %d", serverId);
		lwm2m_update_registration(&getContext(), serverId, false, true);
		return true; 
	});

	// TODO: Bootstrap Request (Res id 9) must be implemented by wakaama core or WppClient
	#if RES_1_9
	resource(BOOTSTRAP_REQUEST_TRIGGER_9)->set((EXECUTE_T)[this](Instance& inst, ID_T resId, const OPAQUE_T& data) {
		if (!WppTaskQueue::isTaskExist(_requestBootstrapTaskId)) {
			WPP_LOGI(TAG, "Bootstrap Request Trigger: Bootstrap request is started");
			_requestBootstrapTaskId = WppTaskQueue::addTask(WPP_TASK_MIN_DELAY_S, [this](WppClient &client, void *ctx) -> bool {
				getContext().state = STATE_BOOTSTRAP_REQUIRED;
				return true;
			});
		} else {
			WPP_LOGI(TAG, "Bootstrap Request Trigger: Bootstrap request is already in progress");
		}
		return true; 
	});
	#endif

	#if RES_1_10    
	resource(APN_LINK_10)->set(OBJ_LINK_T());                                                                                                                                                                                                                          
	#endif 

	#if RES_1_11
	resource(TLS_DTLS_ALERT_CODE_11)->set(UINT_T(TLS_DTLS_ALERT_CODE_MIN));
	resource(TLS_DTLS_ALERT_CODE_11)->setDataVerifier((VERIFY_UINT_T)[](const UINT_T& value) { return TLS_DTLS_ALERT_CODE_MIN <= value && value <= TLS_DTLS_ALERT_CODE_MAX; });
	#endif
	                                                                                                                                                                                                                                       
	#if RES_1_12
	resource(LAST_BOOTSTRAPPED_12)->set(TIME_T(0));                                                                                                                                                                                                                    
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_13
	resource(REGISTRATION_PRIORITY_ORDER_13)->set(UINT_T(0));                                                                                                                                                                                                           
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_14
	resource(INITIAL_REGISTRATION_DELAY_TIMER_14)->set(UINT_T(0));                                                                                                                                                                                                      
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_15  
	resource(REGISTRATION_FAILURE_BLOCK_15)->set(false);                                                                                                                                                                                                             
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_16
	resource(BOOTSTRAP_ON_REGISTRATION_FAILURE_16)->set(false);                                                                                                                                                                                                    
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_17                                                                                                                                                                                                             
	resource(COMMUNICATION_RETRY_COUNT_17)->set(UINT_T(0));
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_18                                                                                                                                                                                                             
	resource(COMMUNICATION_RETRY_TIMER_18)->set(UINT_T(0));
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_19                                                                                                                                                                                                    
	resource(COMMUNICATION_SEQUENCE_DELAY_TIMER_19)->set(UINT_T(0));
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_20                                                                                                                                                                                                    
	resource(COMMUNICATION_SEQUENCE_RETRY_COUNT_20)->set(UINT_T(0));
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_21
	resource(TRIGGER_21)->set(false);                                                                                                                                                                                                                               
	#endif 

	#if RES_1_22
	resource(PREFERRED_TRANSPORT_22)->set(STRING_T(""));
	#endif

	#if RES_1_23                                                                                                                                                                                                                             
	resource(MUTE_SEND_23)->set(false);
	#endif 
	/* --------------- Code_cpp block 10 end --------------- */
}

/* --------------- Code_cpp block 11 start --------------- */
/* --------------- Code_cpp block 11 end --------------- */

} /* namespace wpp */
