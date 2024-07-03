/*
 * Accelerometer
 * Generated on: 2024-06-27 12:57:44
 * Created by: Sinai RnD
 */

#ifndef WPP_O_3313_ACCELEROMETER_H
#define WPP_O_3313_ACCELEROMETER_H

#include "AccelerometerConfig.h"
#include "AccelerometerInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class Accelerometer : public Instance {
public:
	enum ID: ID_T {
		X_VALUE_5702 = 5702,
		#if RES_3313_5703
		Y_VALUE_5703 = 5703,
		#endif
		#if RES_3313_5704
		Z_VALUE_5704 = 5704,
		#endif
		#if RES_3313_5701
		SENSOR_UNITS_5701 = 5701,
		#endif
		#if RES_3313_5603
		MIN_RANGE_VALUE_5603 = 5603,
		#endif
		#if RES_3313_5604
		MAX_RANGE_VALUE_5604 = 5604,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

public:
	Accelerometer(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Accelerometer();

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
	static Accelerometer * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static Accelerometer * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

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
	/* --------------- Code_h block 6 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_3313_ACCELEROMETER_H */
