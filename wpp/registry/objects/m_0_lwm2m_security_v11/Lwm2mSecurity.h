/*
 * Lwm2mSecurity
 * Generated on: 2023-11-02 16:43:32
 * Created by: SinaiR&D
 */

#ifndef WPP_M_0_LWM2M_SECURITY_V11_H
#define WPP_M_0_LWM2M_SECURITY_V11_H

#include "Lwm2mSecurityConfig.h"
#include "Lwm2mSecurityInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class Lwm2mSecurity : public Instance {
public:
	enum ID: ID_T {
		LWM2M_SERVER_URI_0 = 0,
		BOOTSTRAP_SERVER_1 = 1,
		SECURITY_MODE_2 = 2,
		PUBLIC_KEY_OR_IDENTITY_3 = 3,
		SERVER_PUBLIC_KEY_4 = 4,
		SECRET_KEY_5 = 5,
		#if RES_0_6
		SMS_SECURITY_MODE_6 = 6,
		#endif
		#if RES_0_7
		SMS_BINDING_KEY_PARAMETERS_7 = 7,
		#endif
		#if RES_0_8
		SMS_BINDING_SECRET_KEY_S__8 = 8,
		#endif
		#if RES_0_9
		LWM2M_SERVER_SMS_NUMBER_9 = 9,
		#endif
		#if RES_0_10
		SHORT_SERVER_ID_10 = 10,
		#endif
		#if RES_0_11
		CLIENT_HOLD_OFF_TIME_11 = 11,
		#endif
		#if RES_0_12
		BOOTSTRAP_SERVER_ACCOUNT_TIMEOUT_12 = 12,
		#endif
		#if RES_0_13
		MATCHING_TYPE_13 = 13,
		#endif
		#if RES_0_14
		SNI_14 = 14,
		#endif
		#if RES_0_15
		CERTIFICATE_USAGE_15 = 15,
		#endif
		#if RES_0_16
		DTLS_TLS_CIPHERSUITE_16 = 16,
		#endif
		#if RES_0_17
		OSCORE_SECURITY_MODE_17 = 17,
		#endif
	};

	/* --------------- Code_h block 1 start --------------- */
	#if RES_0_6
	enum SmsSecMode: uint8_t {
		SMS_SEC_MODE_MIN = 0,
		SMS_SEC_DTLS_PSK = 1,
		SMS_SEC_PACK_STRUCT = 2,
		SMS_NO_SEC = 3,
		SMS_SEC_MODE_MAX = 255
	};
	#endif

	#if RES_0_8
	enum SmsKeyLen: uint8_t {
		MIN_SMS_KEY_LEN = 16,
		MAX_SMS_KEY_LEN = 48
	};
	#endif

	#if RES_0_13
	enum MatchType: uint8_t {
		EXACT_MATCH = 0,
		SHA_256 = 1,
		SHA_384 = 2,
		SHA_512 = 3,
		MAX_MATCH_TYPE
	};
	#endif

	#if RES_0_15
	enum CertUsage: uint8_t {
		CA_CONSTRAINT = 0,
		SERVICE_CERT_CONSTRAINT = 1,
		TRUST_ANCHOR_ASSERTION = 2,
		DOMAIN_ISSUED_CERT = 3,
		MAX_CERT_USAGE
	};
	#endif
	/* --------------- Code_h block 1 end --------------- */

public:
	Lwm2mSecurity(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Lwm2mSecurity();

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
	
	/* --------------- Code_h block 3 start --------------- */
	/* --------------- Code_h block 3 end --------------- */

private:
	/* --------------- Code_h block 4 start --------------- */
	/* --------------- Code_h block 4 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_M_0_LWM2M_SECURITY_V11_H */
