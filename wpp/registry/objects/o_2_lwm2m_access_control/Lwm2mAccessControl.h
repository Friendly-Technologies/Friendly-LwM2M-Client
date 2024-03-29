/*
 * Lwm2mAccessControl
 * Generated on: 2023-11-03 15:05:47
 * Created by: SinaiR&D
 */

#ifndef WPP_O_2_LWM2M_ACCESS_CONTROL_H
#define WPP_O_2_LWM2M_ACCESS_CONTROL_H

#include "Lwm2mAccessControlConfig.h"
#include "Lwm2mAccessControlInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

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

	/* --------------- Code_h block 1 start --------------- */

	#if RES_2_2
	enum ACL: uint8_t {
		R_O_W_ATTR = 1,
		WRITE = 2,
		EXECUTE = 4,
		DELETE = 8,
		CREATE = 16,
		ALL_RIGHTS = R_O_W_ATTR | WRITE | EXECUTE | DELETE | CREATE,
		ACL_MAX,
	};
	#endif

	/* --------------- Code_h block 1 end --------------- */

public:
	Lwm2mAccessControl(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Lwm2mAccessControl();

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

protected:
	/* --------------- Instance implementation part --------------- */
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Instance *securityInst, ResOp::TYPE type, const ResLink &resLink) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ResOp::TYPE type, const ResLink &resLink) override;

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
	/* --------------- Code_h block 5 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_O_2_LWM2M_ACCESS_CONTROL_H */
