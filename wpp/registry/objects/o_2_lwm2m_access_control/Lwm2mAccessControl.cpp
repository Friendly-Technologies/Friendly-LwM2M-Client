/*
 * Lwm2mAccessControl
 * Generated on: 2023-11-03 15:05:47
 * Created by: Sinai RnD
 */

#include "o_2_lwm2m_access_control/Lwm2mAccessControl.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
#define AC_OBJ_ID_MIN 0
#define AC_OBJ_ID_MAX (ID_T_MAX_VAL-1)
#define AC_OBJ_INST_ID_MIN 0
#define AC_OBJ_INST_ID_MAX ID_T_MAX_VAL
#if RES_2_2
#define ACL_MIN 0
#define ACL_MAX 0x32
#endif
#define AC_OWNER_MIN 0
#define AC_OWNER_MAX ID_T_MAX_VAL
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Lwm2mAccessControl"

namespace wpp {

Lwm2mAccessControl::Lwm2mAccessControl(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	lwm2m_ac_request_update_policy(&context, false);
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Lwm2mAccessControl::~Lwm2mAccessControl() {
	/* --------------- Code_cpp block 3 start --------------- */
	lwm2m_ac_request_update_policy(&getContext(), false);
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & Lwm2mAccessControl::object(WppClient &ctx) {
	return ctx.registry().lwm2mAccessControl();
}

Lwm2mAccessControl * Lwm2mAccessControl::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().lwm2mAccessControl().instance(instId);
	if (!inst) return NULL;
	return static_cast<Lwm2mAccessControl*>(inst);
}

Lwm2mAccessControl * Lwm2mAccessControl::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().lwm2mAccessControl().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<Lwm2mAccessControl*>(inst);
}

bool Lwm2mAccessControl::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().lwm2mAccessControl().remove(instId);
}

void Lwm2mAccessControl::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 4 start --------------- */
	lwm2m_ac_request_update_policy(&getContext(), false);
	/* --------------- Code_cpp block 4 end --------------- */

	operationNotify(*this, resLink, type);

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void Lwm2mAccessControl::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
	if (type == ItemOp::WRITE || type == ItemOp::DELETE) notifyResChanged(resLink.resId, resLink.resInstId);

	/* --------------- Code_cpp block 6 start --------------- */
	lwm2m_ac_request_update_policy(&getContext(), false);
	/* --------------- Code_cpp block 6 end --------------- */
}

void Lwm2mAccessControl::resourcesCreate() {
	std::vector<Resource> resources = {
		{OBJECT_ID_0,            ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
		{OBJECT_INSTANCE_ID_1,   ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
		#if RES_2_2                                                                                                                                                
		{ACL_2,                  ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT }, 
		#endif                                                                                                                                                     
		{ACCESS_CONTROL_OWNER_3, ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
	};
	setupResources(std::move(resources));
}

void Lwm2mAccessControl::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	
	resource(OBJECT_ID_0)->set<INT_T>(AC_OBJ_ID_MIN);
	resource(OBJECT_ID_0)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return AC_OBJ_ID_MIN <= value && value <= AC_OBJ_ID_MAX; });

	resource(OBJECT_INSTANCE_ID_1)->set<INT_T>(AC_OBJ_INST_ID_MIN);
	resource(OBJECT_INSTANCE_ID_1)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return AC_OBJ_INST_ID_MIN <= value && value <= AC_OBJ_INST_ID_MAX; });

	#if RES_2_2
	resource(ACL_2)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return ACL_MIN <= value && value < ACL_MAX; });
	#endif

	resource(ACCESS_CONTROL_OWNER_3)->set<INT_T>(AC_OWNER_MIN);
	resource(ACCESS_CONTROL_OWNER_3)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return AC_OWNER_MIN <= value && value <= AC_OWNER_MAX; });

	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
Lwm2mAccessControl * Lwm2mAccessControl::getAcInstForTarget(Object &acObj, ID_T objId, ID_T objInstId) {
	// Find the ac object instance for the target object
	for (auto &inst : acObj.instances()) {
		INT_T tmpObjId = inst->get<INT_T>(OBJECT_ID_0);
		INT_T tmpObjInstId = inst->get<INT_T>(OBJECT_INSTANCE_ID_1);
		if (tmpObjId == objId && tmpObjInstId == objInstId) return static_cast<Lwm2mAccessControl*>(inst);
	}
	return NULL;
}

Lwm2mAccessControl * Lwm2mAccessControl::create(Object &targetObj, uint8_t defaultAcl) {
	WppRegistry &registry = targetObj.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();

	// Check if the object instance already exists
	if (getAcInstForTarget(acObj, targetObj.getObjectID(), AC_OBJ_INST_NOT_SET)) return NULL;

	Lwm2mAccessControl *acInst = static_cast<Lwm2mAccessControl *>(acObj.createInstance());
	acInst->set<INT_T>(OBJECT_ID_0, targetObj.getObjectID());
	acInst->set<INT_T>(OBJECT_INSTANCE_ID_1, AC_OBJ_INST_NOT_SET);
	#if RES_2_2
	acInst->set<INT_T>(ACL_2, AC_ACL_DEFAULT_ID, defaultAcl & ALL_OBJ_RIGHTS);
	#endif
	acInst->set<INT_T>(ACCESS_CONTROL_OWNER_3, AC_CLIENT_OWNER);

	return acInst;
}

void Lwm2mAccessControl::remove(Object &targetObj) {
	WppRegistry &registry = targetObj.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetObj.getObjectID(), AC_OBJ_INST_NOT_SET);

	if (acInst) acObj.remove(acInst->getInstanceID());
}

#if RES_2_2
bool Lwm2mAccessControl::addAcl(Object &targetObj, ID_T serverShortId, uint8_t acl) {
	WppRegistry &registry = targetObj.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetObj.getObjectID(), AC_OBJ_INST_NOT_SET);

	if (!acInst) return false;

	acInst->set<INT_T>(ACL_2, serverShortId, acl & ALL_OBJ_RIGHTS);

	return true;
}

void Lwm2mAccessControl::removeAcl(Object &targetObj, ID_T serverShortId) {
	WppRegistry &registry = targetObj.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetObj.getObjectID(), AC_OBJ_INST_NOT_SET);

	if (!acInst || !acInst->isExist(ACL_2, serverShortId)) return;
	acInst->removeRes(ACL_2, serverShortId);
}
#endif

Lwm2mAccessControl * Lwm2mAccessControl::create(Instance &targetInst, ID_T owner, uint8_t defaultAcl) {
	WppRegistry &registry = targetInst.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();

	// Check if the object instance already exists
	if (getAcInstForTarget(acObj, targetInst.getObjectID(), targetInst.getInstanceID())) return NULL;

	Lwm2mAccessControl *acInst = static_cast<Lwm2mAccessControl *>(acObj.createInstance());
	acInst->set<INT_T>(OBJECT_ID_0, targetInst.getObjectID());
	acInst->set<INT_T>(OBJECT_INSTANCE_ID_1, targetInst.getInstanceID());
	#if RES_2_2
	acInst->set<INT_T>(ACL_2, AC_ACL_DEFAULT_ID, defaultAcl & ALL_INST_RIGHTS);
	#endif
	acInst->set<INT_T>(ACCESS_CONTROL_OWNER_3, owner);

	return acInst;
}

void Lwm2mAccessControl::remove(Instance &targetInst) {
	WppRegistry &registry = targetInst.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetInst.getObjectID(), targetInst.getInstanceID());

	if (acInst) acObj.remove(acInst->getInstanceID());
}

#if RES_2_2
bool Lwm2mAccessControl::addAcl(Instance &targetInst, ID_T serverShortId, uint8_t acl) {
	WppRegistry &registry = targetInst.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetInst.getObjectID(), targetInst.getInstanceID());

	if (!acInst) return false;
	acInst->set<INT_T>(ACL_2, serverShortId, acl & ALL_INST_RIGHTS);

	return true;
}

void Lwm2mAccessControl::removeAcl(Instance &targetInst, ID_T serverShortId) {
	WppRegistry &registry = targetInst.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetInst.getObjectID(), targetInst.getInstanceID());

	if (!acInst || !acInst->isExist(ACL_2, serverShortId)) return;
	acInst->removeRes(ACL_2, serverShortId);
}
#endif
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
