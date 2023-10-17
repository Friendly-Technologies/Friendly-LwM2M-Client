/*
 * File type: FILE_TYPE_OBJ_IMPL_CPP
 * Generated on: 2023-10-14 19:46:46
 * Author: valentin
 * Generated by: Vadimzakreva
 */

#include "m_1_lwm2m_server_v11/Lwm2mServer.h"

#include <unordered_map>
#include <iostream>

#include "Resource.h"
#include "ResOp.h"
#include "types.h"
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
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Lwm2mServer::~Lwm2mServer() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Resource * Lwm2mServer::getResource(ID_T id) {
	auto res = getResIter(id);
	// Check if resource ID is valid
	if (res == _resources.end()) return NULL;
	return &(*res);
}

std::vector<Resource *> Lwm2mServer::getResourcesList() {
	std::vector<Resource *> list;
	for (auto &res : _resources) {
		list.push_back(&res);
	}
	return list;
}

std::vector<Resource *> Lwm2mServer::getResourcesList(const ResOp& filter) {
	std::vector<Resource *> list;
	for (auto &res : _resources) {
		if (filter.isCompatible(res.getOperation())) list.push_back(&res);
	}
	return list;
}

std::vector<Resource *> Lwm2mServer::getInstantiatedResourcesList() {
	std::vector<Resource *> list;
	for (auto &res : _resources) {
		if (!res.isEmpty()) list.push_back(&res);
	}
	return list;
}

std::vector<Resource *> Lwm2mServer::getInstantiatedResourcesList(const ResOp& filter) {
	std::vector<Resource *> list;
	for (auto &res : _resources) {
		if (!res.isEmpty() && filter.isCompatible(res.getOperation())) list.push_back(&res);
	}
	return list;
}

void Lwm2mServer::setDefaultState() {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	for (auto &res : _resources) res.clear();
	resourcesInit();

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void Lwm2mServer::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 6 start --------------- */
	/* --------------- Code_cpp block 6 end --------------- */

	observerNotify(*this, resId, type);

	/* --------------- Code_cpp block 7 start --------------- */
	switch (type) {
	case ResOp::READ:
		WPP_LOGD_ARG(TAG, "Server READ -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE:
		WPP_LOGD_ARG(TAG, "Server WRITE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::EXECUTE:
		WPP_LOGD_ARG(TAG, "Server EXECUTE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::DISCOVER:
		WPP_LOGD_ARG(TAG, "Server DISCOVER -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::DELETE:
		WPP_LOGD_ARG(TAG, "Server DELETE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 7 end --------------- */
}

void Lwm2mServer::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 8 start --------------- */
	switch (type) {
	case ResOp::READ:
		WPP_LOGD_ARG(TAG, "User READ -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::WRITE:
		WPP_LOGD_ARG(TAG, "User WRITE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	case ResOp::DELETE:
		WPP_LOGD_ARG(TAG, "User DELETE -> resId: %d, resInstId: %d", resId.resId, resId.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 8 end --------------- */
}

void Lwm2mServer::resourcesInit() {
	/* --------------- Code_cpp block 9 start --------------- */
	// TODO: The most part of the server resources logic must be implemented
	// on wakaama core level, because the Server is only a state holder and
	// at this level, it does not have the required information for doing
	// sings described in the documentation.

	getResIter(SHORT_SERVER_ID_0)->set(INT_T(0));
	getResIter(SHORT_SERVER_ID_0)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return SINGLE_INSTANCE_ID < value && value < ID_T_MAX_VAL; });

	getResIter(LIFETIME_1)->set(INT_T(0));

	#if RES_1_2    
	_resources[DEFAULT_MINIMUM_PERIOD_2].set(INT_T(0));                                                                                                                                                                                                             
	#endif    

	#if RES_1_3       
	_resources[DEFAULT_MAXIMUM_PERIOD_3].set(INT_T(0));                                                                                                                                                                                                          
	#endif                       

	// TODO: Disable (Res id 4) must be implemented by wakaama core or WppClient

	#if RES_1_5                                                                                                                                                                                                                        
	_resources[DISABLE_TIMEOUT_5].set(INT_T(0));
	#endif 

	// TODO: Notification Storing (Res id 6) must be implemented by wakaama core
	getResIter(NOTIFICATION_STORING_WHEN_DISABLED_OR_OFFLINE_6)->set(false);
	
	getResIter(BINDING_7)->set(STRING_T(""));
	getResIter(BINDING_7)->setDataVerifier((VERIFY_STRING_T)[](const STRING_T& value) { return wppBindingValidate(value); });

	// TODO: Registration Update (Res id 8) must be implemented by wakaama core or WppClient

	// TODO: Bootstrap Request (Res id 9) must be implemented by wakaama core or WppClient

	#if RES_1_10    
	_resources[APN_LINK_10].set(OBJ_LINK_T());                                                                                                                                                                                                                          
	#endif 

	#if RES_1_11
	_resources[TLS_DTLS_ALERT_CODE_11].set(UINT_T(TLS_DTLS_ALERT_CODE_MIN));
	_resources[TLS_DTLS_ALERT_CODE_11].setDataVerifier((VERIFY_UINT_T)[](const UINT_T& value) { return TLS_DTLS_ALERT_CODE_MIN <= value && value <= TLS_DTLS_ALERT_CODE_MAX; });
	#endif
	                                                                                                                                                                                                                                       
	#if RES_1_12
	_resources[LAST_BOOTSTRAPPED_12].set(TIME_T(0));                                                                                                                                                                                                                    
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_13
	_resources[REGISTRATION_PRIORITY_ORDER_13].set(UINT_T(0));                                                                                                                                                                                                           
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_14
	_resources[INITIAL_REGISTRATION_DELAY_TIMER_14].set(UINT_T(0));                                                                                                                                                                                                      
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_15  
	_resources[REGISTRATION_FAILURE_BLOCK_15].set(false);                                                                                                                                                                                                             
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_16
	_resources[BOOTSTRAP_ON_REGISTRATION_FAILURE_16].set(false);                                                                                                                                                                                                    
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_17                                                                                                                                                                                                             
	_resources[COMMUNICATION_RETRY_COUNT_17].set(UINT_T(0));
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_18                                                                                                                                                                                                             
	_resources[COMMUNICATION_RETRY_TIMER_18].set(UINT_T(0));
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_19                                                                                                                                                                                                    
	_resources[COMMUNICATION_SEQUENCE_DELAY_TIMER_19].set(UINT_T(0));
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_20                                                                                                                                                                                                    
	_resources[COMMUNICATION_SEQUENCE_RETRY_COUNT_20].set(UINT_T(0));
	#endif                                                                                                                                                                                                                                             
	
	#if RES_1_21
	_resources[TRIGGER_21].set(false);                                                                                                                                                                                                                               
	#endif 

	#if RES_1_22
	_resources[PREFERRED_TRANSPORT_22].set(STRING_T(""));
	#endif

	#if RES_1_23                                                                                                                                                                                                                             
	resources[MUTE_SEND_23].set(false);
	#endif 
	/* --------------- Code_cpp block 9 end --------------- */
}

std::vector<Resource>::iterator Lwm2mServer::getResIter(ID_T resId) {
	auto finder = [&resId](const Resource &res) -> bool { return res.getId() == resId; };
	return std::find_if(_resources.begin(), _resources.end(), finder);
}

/* --------------- Code_cpp block 10 start --------------- */
/* --------------- Code_cpp block 10 end --------------- */

} /* namespace wpp */
