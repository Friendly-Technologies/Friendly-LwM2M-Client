/*
 * ConnectivityMonitoring
 * Generated on: 2023-11-02 16:57:15
 * Created by: Sinai RnD
 */

#ifndef WPP_O_4_CONNECTIVITY_MONITORING_H
#define WPP_O_4_CONNECTIVITY_MONITORING_H

#include "ConnectivityMonitoringConfig.h"
#include "ConnectivityMonitoringInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class ConnectivityMonitoring : public Instance {
public:
	enum ID: ID_T {
		NETWORK_BEARER_0 = 0,
		AVAILABLE_NETWORK_BEARER_1 = 1,
		RADIO_SIGNAL_STRENGTH_2 = 2,
		#if RES_4_3
		LINK_QUALITY_3 = 3,
		#endif
		IP_ADDRESSES_4 = 4,
		#if RES_4_5
		ROUTER_IP_ADDRESSES_5 = 5,
		#endif
		#if RES_4_6
		LINK_UTILIZATION_6 = 6,
		#endif
		#if RES_4_7
		APN_7 = 7,
		#endif
		#if RES_4_8
		CELL_ID_8 = 8,
		#endif
		#if RES_4_9
		SMNC_9 = 9,
		#endif
		#if RES_4_10
		SMCC_10 = 10,
		#endif
		#if RES_4_11
		SIGNALSNR_11 = 11,
		#endif
		#if RES_4_12
		LAC_12 = 12,
		#endif
		#if RES_4_13
		COVERAGE_ENHANCEMENT_LEVEL_13 = 13,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	enum NtwrkBrr: uint8_t {
		GSM = 0,
		TD_SCDMA = 1,
		WCDMA = 2,
		CDMA2000 = 3,
		WIMAX = 4,
		LTE_TDD = 5,
		LTE_FDD = 6,
		NB_IOT = 7,
		NR_TDD_5G = 8,
		NR_FDD_5G = 9,
		CLLR_OTHER_BEGIN = 10,
		CLLR_OTHER_END = 20,
		WLAN = 21,
		BLUETOOTH = 22,
		IEEE_802_15_4 = 23,
		WRLSS_OTHER_BEGIN = 24,
		WRLSS_OTHER_END = 40,
		ETHERNET = 41,
		DSL = 42,
		PLC = 43,
		WRLN_OTHER_BEGIN = 44,
		WRLN_OTHER_END = 50,
		NTWRK_BRR_MAX
	};

	#if RES_4_13
	enum CvrgEnhncmntLvl: uint8_t {
		MISSING = 0,
		LEVEL_0 = 1,
		LEVEL_1 = 2,
		LEVEL_2 = 3,
		LEVEL_3 = 4,
		CVRG_ENHNCMNT_LVL_MAX
	};
	#endif
	/* --------------- Code_h block 2 end --------------- */

public:
	ConnectivityMonitoring(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~ConnectivityMonitoring();
	
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
	static ConnectivityMonitoring * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static ConnectivityMonitoring * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Removes an instance of the object.
	 * @param ctx - WppClient context.
	 * @param instId The instance ID.
	 * @return True if the instance was successfully removed, false otherwise.
	 */
	static bool removeInst(WppClient &ctx, ID_T instId);

	/* --------------- Code_h block 3 start --------------- */

	bool checkLinkQuality(uint8_t linkQuality);
	bool checkCellId(uint32_t cellId);

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
	/* --------------- Code_h block 6 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_4_CONNECTIVITY_MONITORING_H */
