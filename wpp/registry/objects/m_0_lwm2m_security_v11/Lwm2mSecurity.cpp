/*
 * File type: FILE_TYPE_OBJ_IMPL_CPP
 * Generated on: 2023-10-13 18:39:53
 * Author: valentin
 * Generated by: Vadimzakreva
 */

#include "m_0_lwm2m_security_v11/Lwm2mSecurity.h"

#include <unordered_map>
#include <iostream>

#include "Resource.h"
#include "ResOp.h"
#include "types.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
#define SERVER_URI_MAX_SIZE	255
#if RES_0_7
#define SMS_BIND_KEY_PARAMS_SIZE	6
#endif
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Lwm2mSecurity"

namespace wpp {

Lwm2mSecurity::Lwm2mSecurity(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Lwm2mSecurity::~Lwm2mSecurity() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

void Lwm2mSecurity::setDefaultState() {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	_resources.clear();
	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void Lwm2mSecurity::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
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

void Lwm2mSecurity::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
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

void Lwm2mSecurity::resourcesCreate() {
	std::vector<Resource> resources = {
		// KEY   VALUE
		{LWM2M_SERVER_URI_0,                         ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::MANDATORY,        TYPE_ID::STRING },          
		{BOOTSTRAP_SERVER_1,                         ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::MANDATORY,        TYPE_ID::BOOL },            
		{SECURITY_MODE_2,                            ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::MANDATORY,        TYPE_ID::INT },             
		{PUBLIC_KEY_OR_IDENTITY_3,                   ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::MANDATORY,        TYPE_ID::OPAQUE },          
		{SERVER_PUBLIC_KEY_4,                        ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::MANDATORY,        TYPE_ID::OPAQUE },          
		{SECRET_KEY_5,                               ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::MANDATORY,        TYPE_ID::OPAQUE },          
		#if RES_0_6                                                                                                                                                                                                                
		{SMS_SECURITY_MODE_6,                        {SMS_SECURITY_MODE_6,                        ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::INT }},             
		#endif                                                                                                                                                                                                                     
		#if RES_0_7                                                                                                                                                                                                                
		{SMS_BINDING_KEY_PARAMETERS_7,               {SMS_BINDING_KEY_PARAMETERS_7,               ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::OPAQUE }},          
		#endif                                                                                                                                                                                                                     
		#if RES_0_8                                                                                                                                                                                                                
		{SMS_BINDING_SECRET_KEY_S__8,                {SMS_BINDING_SECRET_KEY_S__8,                ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::OPAQUE }},          
		#endif                                                                                                                                                                                                                     
		#if RES_0_9                                                                                                                                                                                                                
		{LWM2M_SERVER_SMS_NUMBER_9,                  {LWM2M_SERVER_SMS_NUMBER_9,                  ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::STRING }},          
		#endif                                                                                                                                                                                                                     
		#if RES_0_10                                                                                                                                                                                                               
		{SHORT_SERVER_ID_10,                         ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::INT },             
		#endif                                                                                                                                                                                                                     
		#if RES_0_11                                                                                                                                                                                                               
		{CLIENT_HOLD_OFF_TIME_11,                    ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::INT },             
		#endif                                                                                                                                                                                                                     
		#if RES_0_12                                                                                                                                                                                                               
		{BOOTSTRAP_SERVER_ACCOUNT_TIMEOUT_12,        ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::INT },             
		#endif                                                                                                                                                                                                                     
		#if RES_0_13                                                                                                                                                                                                               
		{MATCHING_TYPE_13,                           {MATCHING_TYPE_13,                           ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                     
		#if RES_0_14                                                                                                                                                                                                               
		{SNI_14,                                     {SNI_14,                                     ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::STRING }},          
		#endif                                                                                                                                                                                                                     
		#if RES_0_15                                                                                                                                                                                                               
		{CERTIFICATE_USAGE_15,                       {CERTIFICATE_USAGE_15,                       ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                     
		#if RES_0_16                                                                                                                                                                                                               
		{DTLS_TLS_CIPHERSUITE_16,                    {DTLS_TLS_CIPHERSUITE_16,                    ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::MULTIPLE,        IS_MANDATORY::OPTIONAL,         TYPE_ID::UINT }},            
		#endif                                                                                                                                                                                                                     
		#if RES_0_17                                                                                                                                                                                                               
		{OSCORE_SECURITY_MODE_17,                    {OSCORE_SECURITY_MODE_17,                    ResOp(ResOp::READ|ResOp::WRITE),        IS_SINGLE::SINGLE,          IS_MANDATORY::OPTIONAL,         TYPE_ID::OBJ_LINK }},        
		#endif                                                                                                                                                                                                                     
	};
	_resources = std::move(resources);
}

void Lwm2mSecurity::resourcesInit() {
	/* --------------- Code_cpp block 9 start --------------- */
	getResIter(LWM2M_SERVER_URI_0)->set(STRING_T(""));
	getResIter(LWM2M_SERVER_URI_0)->setDataVerifier((VERIFY_STRING_T)[](const STRING_T& value) { return value.size() < SERVER_URI_MAX_SIZE; });

	getResIter(BOOTSTRAP_SERVER_1)->set(false);

	getResIter(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_NONE));
	getResIter(SECURITY_MODE_2)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return LWM2M_SECURITY_MODE_PRE_SHARED_KEY <= value && value <= LWM2M_SECURITY_MODE_NONE; });

	getResIter(PUBLIC_KEY_OR_IDENTITY_3)->set(OPAQUE_T());

	getResIter(SERVER_PUBLIC_KEY_4)->set(OPAQUE_T());
	
	getResIter(SECRET_KEY_5)->set(OPAQUE_T());

	#if RES_0_6
	getResIter(SMS_SECURITY_MODE_6)->set(INT_T(SMS_SEC_MODE_MAX));
	getResIter(SMS_SECURITY_MODE_6)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return SMS_SEC_MODE_MIN <= value && value <= SMS_SEC_MODE_MAX; });
	#endif
	
	#if RES_0_7
	getResIter(SMS_BINDING_KEY_PARAMETERS_7)->set(OPAQUE_T());
	getResIter(SMS_BINDING_KEY_PARAMETERS_7)->setDataVerifier((VERIFY_OPAQUE_T)[](const OPAQUE_T& value) { return value.size() == SMS_BIND_KEY_PARAMS_SIZE; });
	#endif

	#if RES_0_8_
	getResIter(SMS_BINDING_SECRET_KEY_S__8)->set(OPAQUE_T());
	getResIter(SMS_BINDING_SECRET_KEY_S__8)->setDataVerifier((VERIFY_OPAQUE_T)[](const OPAQUE_T& value) { return MIN_SMS_KEY_LEN <= value.size() && value.size() <= MAX_SMS_KEY_LEN; });
	#endif

	#if RES_0_9
	getResIter(LWM2M_SERVER_SMS_NUMBER_9)->set(STRING_T(""));                                                                                                                                                                                                                        
	#endif 

	#if RES_0_10
	getResIter(SHORT_SERVER_ID_10)->set(INT_T(0));
	getResIter(SHORT_SERVER_ID_10)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return SINGLE_INSTANCE_ID < value && value < ID_T_MAX_VAL; });
	#endif
	
	#if RES_0_11
	getResIter(CLIENT_HOLD_OFF_TIME_11)->set(INT_T(0));
	#endif
	
	#if RES_0_12
	getResIter(BOOTSTRAP_SERVER_ACCOUNT_TIMEOUT_12)->set(INT_T(0));                                                                                                                                                                                                             
	#endif

	#if RES_0_13
	getResIter(MATCHING_TYPE_13)->set(UINT_T(MAX_MATCH_TYPE));
	getResIter(MATCHING_TYPE_13)->setDataVerifier((VERIFY_UINT_T)[](const UINT_T& value) { return EXACT_MATCH <= value && value < MAX_MATCH_TYPE; });
	#endif

	#if RES_0_14
	getResIter(SNI_14)->set(STRING_T(""));                                                                                                                                                                                                                                     
	#endif

	#if RES_0_15
	getResIter(CERTIFICATE_USAGE_15)->set(UINT_T(MAX_MATCH_TYMAX_CERT_USAGEPE));
	getResIter(CERTIFICATE_USAGE_15)->setDataVerifier((VERIFY_UINT_T)[](const UINT_T& value) { return CA_CONSTRAINT <= value && value < MAX_CERT_USAGE; });
	#endif

	#if RES_0_16                                                                                                                                                                                                                            
	getResIter(DTLS_TLS_CIPHERSUITE_16)->set(UINT_T(0));
	#endif   

	#if RES_0_17
	getResIter(OSCORE_SECURITY_MODE_17)->set(OBJ_LINK_T());                                                                                                                                                                                                                        
	#endif  
	/* --------------- Code_cpp block 9 end --------------- */
}

/* --------------- Code_cpp block 10 start --------------- */
/* --------------- Code_cpp block 10 end --------------- */

} /* namespace wpp */
