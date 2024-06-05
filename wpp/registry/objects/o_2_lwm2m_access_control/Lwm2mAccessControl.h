/*
 * Lwm2mAccessControl
 * Generated on: 2023-11-03 15:05:47
 * Created by: Sinai RnD
 */

#ifndef WPP_O_2_LWM2M_ACCESS_CONTROL_H
#define WPP_O_2_LWM2M_ACCESS_CONTROL_H

#include "Lwm2mAccessControlConfig.h"
#include "Lwm2mAccessControlInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
#define AC_CLIENT_OWNER ID_T_MAX_VAL
#define AC_ACL_DEFAULT_ID LWM2M_AC_ACL_DEFAULT_ID
#define AC_OBJ_INST_NOT_SET ID_T_MAX_VAL
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class Lwm2mAccessControl : public Instance {
public:
	enum ID: ID_T {
		OBJECT_ID_0 = 0,
		OBJECT_INSTANCE_ID_1 = 1,
		#if RES_2_2
		ACL_2 = 2,
		#endif
		ACCESS_CONTROL_OWNER_3 = 3,
	};

	/* --------------- Code_h block 2 start --------------- */
	#if RES_2_2
	enum ACL: uint8_t {
		NO_ACCESS = LWM2M_AC_NO_ACCESS,
		R_O_W_ATTR = LWM2M_AC_READ_OP,
		WRITE = LWM2M_AC_WRITE_OP,
		EXECUTE = LWM2M_AC_EXECUTE_OP,
		DELETE = LWM2M_AC_DELETE_OP,
		CREATE = LWM2M_AC_CREATE_OP,
		ALL_OBJ_RIGHTS = CREATE,
		ALL_INST_RIGHTS = R_O_W_ATTR | WRITE | EXECUTE | DELETE,
	};
	#endif
	/* --------------- Code_h block 2 end --------------- */

public:
	Lwm2mAccessControl(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Lwm2mAccessControl();
	
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
	static Lwm2mAccessControl * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static Lwm2mAccessControl * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Removes an instance of the object.
	 * @param ctx - WppClient context.
	 * @param instId The instance ID.
	 * @return True if the instance was successfully removed, false otherwise.
	 */
	static bool removeInst(WppClient &ctx, ID_T instId);

	/* --------------- Code_h block 3 start --------------- */
	/**
	 * @brief Create Lwm2mAccessControl object instance for target object 
	 * @param targetObj Target object
	 * @param defaultAcl Default ACL for target object, can be NO_ACCESS or ALL_OBJ_RIGHTS
	 */
	static Lwm2mAccessControl * create(Object &targetObj, uint8_t defaultAcl = NO_ACCESS);

	/**
	 * @brief Delete Lwm2mAccessControl object instance for target object
	 * @param targetObj Target object
	 */
	static void remove(Object &targetObj);

	/**
	 * @brief Add ACL for target object
	 * @param targetObj Target object
	 * @param serverShortId Server short ID
	 * @param acl ACL for target object, can be NO_ACCESS or ALL_OBJ_RIGHTS
	 */
	static bool addAcl(Object &targetObj, ID_T serverShortId, uint8_t acl = ALL_OBJ_RIGHTS);

	/**
	 * @brief Remove ACL for target object
	 * @param targetObj Target object
	 * @param serverShortId Server short ID
	 */
	static void removeAcl(Object &targetObj, ID_T serverShortId);

	/**
	 * @brief Get Lwm2mAccessControl object instance for target object instance
	 * @param owner Target object instance owner, should be set to AC_CLIENT_OWNER or server short id
	 * @param defaultAcl Default ACL for target object, can be NO_ACCESS or ALL_INST_RIGHTS
	 */
	static Lwm2mAccessControl * create(Instance &targetInst, ID_T owner = AC_CLIENT_OWNER, uint8_t defaultAcl = NO_ACCESS);

	/**
	 * @brief Delete Lwm2mAccessControl object instance for target object instance
	 * @param targetInst Target object instance
	 */
	static void remove(Instance &targetInst);

	/**
	 * @brief Add ACL for target object instance
	 * @param targetInst Target object instance
	 * @param serverShortId Server short ID
	 * @param acl ACL for target object instance, can be NO_ACCESS or ALL_INST_RIGHTS
	 */
	static bool addAcl(Instance &targetInst, ID_T serverShortId, uint8_t acl = ALL_INST_RIGHTS);

	/**
	 * @brief Remove ACL for target object instance
	 * @param targetInst Target object instance
	 * @param serverShortId Server short ID
	 */
	static void removeAcl(Instance &targetInst, ID_T serverShortId);
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
	static Lwm2mAccessControl * getAcInstForTarget(Object &acObj, ID_T objId, ID_T objInstId);
	/* --------------- Code_h block 5 end --------------- */

private:
	/* --------------- Class private properties --------------- */
	/* --------------- Code_h block 6 start --------------- */
	/* --------------- Code_h block 6 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_2_LWM2M_ACCESS_CONTROL_H */
