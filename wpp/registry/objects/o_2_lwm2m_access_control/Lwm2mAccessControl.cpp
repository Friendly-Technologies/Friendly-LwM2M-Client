/*
 * Lwm2mAccessControl
 * Generated on: 2023-11-03 15:05:47
 * Created by: SinaiR&D
 */

#include "o_2_lwm2m_access_control/Lwm2mAccessControl.h"

#include "Resource.h"
#include "ResOp.h"
#include "WppTypes.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
#include "WppClient.h"

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
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Lwm2mAccessControl::~Lwm2mAccessControl() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

void Lwm2mAccessControl::serverOperationNotifier(Instance *securityInst, ResOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 6 start --------------- */
	/* --------------- Code_cpp block 6 end --------------- */

	operationNotify(*this, resLink, type);

	/* --------------- Code_cpp block 7 start --------------- */
	/* --------------- Code_cpp block 7 end --------------- */
}

void Lwm2mAccessControl::userOperationNotifier(ResOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 8 start --------------- */
	/* --------------- Code_cpp block 8 end --------------- */
}

/* --------------- Code_cpp block 9 start --------------- */
/* --------------- Code_cpp block 9 end --------------- */

void Lwm2mAccessControl::resourcesCreate() {
	std::vector<Resource> resources = {
		{OBJECT_ID_0,            ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
		{OBJECT_INSTANCE_ID_1,   ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
		#if RES_2_2                                                                                                                                                
		{ACL_2,                  ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT }, 
		#endif                                                                                                                                                     
		{ACCESS_CONTROL_OWNER_3, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT }, 
	};
	_resources = std::move(resources);
}

void Lwm2mAccessControl::resourcesInit() {
	/* --------------- Code_cpp block 10 start --------------- */
	
	resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MIN));
	resource(OBJECT_ID_0)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return AC_OBJ_ID_MIN <= value && value <= AC_OBJ_ID_MAX; });

	resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MIN));
	resource(OBJECT_INSTANCE_ID_1)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return AC_OBJ_INST_ID_MIN <= value && value <= AC_OBJ_INST_ID_MAX; });

	#if RES_2_2
	resource(ACL_2)->set(INT_T(ACL_MIN));
	resource(ACL_2)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return ACL_MIN <= value && value < ACL_MAX; });
	#endif

	resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MIN));
	resource(ACCESS_CONTROL_OWNER_3)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return AC_OWNER_MIN <= value && value <= AC_OWNER_MAX; });

	/* --------------- Code_cpp block 10 end --------------- */
}

/* --------------- Code_cpp block 11 start --------------- */

Lwm2mAccessControl * Lwm2mAccessControl::getAcInstForTarget(Object &acObj, ID_T objId, ID_T objInstId) {
	// Find the ac object instance for the target object
	for (auto &inst : acObj.getInstances()) {
		INT_T tmpObjId, tmpObjInstId;
		inst->get(OBJECT_ID_0, tmpObjId);
		inst->get(OBJECT_INSTANCE_ID_1, tmpObjInstId);
		if (tmpObjId == objId && tmpObjInstId == objInstId) return static_cast<Lwm2mAccessControl*>(inst);
	}
	return NULL;
}

bool Lwm2mAccessControl::createInst(Object &targetObj, uint8_t defaultAcl) {
	WppRegistry &registry = targetObj.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();

	// Check if the object instance already exists
	if (getAcInstForTarget(acObj, targetObj.getObjectID(), AC_OBJ_INST_NOT_SET)) return false;

	Lwm2mAccessControl *acInst = acObj.createInstanceSpec();
	acInst->set(OBJECT_ID_0, INT_T(targetObj.getObjectID()));
	acInst->set(OBJECT_INSTANCE_ID_1, INT_T(AC_OBJ_INST_NOT_SET));
	#if RES_2_2
	acInst->set({ACL_2, AC_ACL_DEFAULT_ID}, INT_T(defaultAcl & ALL_OBJ_RIGHTS));
	#endif
	acInst->set(ACCESS_CONTROL_OWNER_3, INT_T(AC_CLIENT_OWNER));

	return true;
}

void Lwm2mAccessControl::deleteInst(Object &targetObj) {
	WppRegistry &registry = targetObj.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetObj.getObjectID(), AC_OBJ_INST_NOT_SET);

	if (acInst) acObj.removeInstance(acInst->getInstanceID());
}

#if RES_2_2
bool Lwm2mAccessControl::addAcl(Object &targetObj, ID_T serverShortId, uint8_t acl) {
	WppRegistry &registry = targetObj.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetObj.getObjectID(), AC_OBJ_INST_NOT_SET);

	if (!acInst) return false;

	acInst->set({ACL_2, serverShortId}, INT_T(acl & ALL_OBJ_RIGHTS));

	return true;
}

void Lwm2mAccessControl::removeAcl(Object &targetObj, ID_T serverShortId) {
	WppRegistry &registry = targetObj.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetObj.getObjectID(), AC_OBJ_INST_NOT_SET);

	if (!acInst) return;

	INT_T acl;
	if (!acInst->get({ACL_2, serverShortId}, acl)) return;
	acInst->remove({ACL_2, serverShortId});
}
#endif

bool Lwm2mAccessControl::createInst(Instance &targetInst, ID_T owner, uint8_t defaultAcl) {
	WppRegistry &registry = targetInst.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();

	// Check if the object instance already exists
	if (getAcInstForTarget(acObj, targetInst.getObjectID(), targetInst.getInstanceID())) return false;

	Lwm2mAccessControl *acInst = acObj.createInstanceSpec();
	acInst->set(OBJECT_ID_0, INT_T(targetInst.getObjectID()));
	acInst->set(OBJECT_INSTANCE_ID_1, INT_T(targetInst.getInstanceID()));
	#if RES_2_2
	acInst->set({ACL_2, AC_ACL_DEFAULT_ID}, INT_T(defaultAcl & ALL_INST_RIGHTS));
	#endif
	acInst->set(ACCESS_CONTROL_OWNER_3, INT_T(owner));

	return true;
}

void Lwm2mAccessControl::deleteInst(Instance &targetInst) {
	WppRegistry &registry = targetInst.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetInst.getObjectID(), targetInst.getInstanceID());

	if (acInst) acObj.removeInstance(acInst->getInstanceID());
}

#if RES_2_2
bool Lwm2mAccessControl::addAcl(Instance &targetInst, ID_T serverShortId, uint8_t acl) {
	WppRegistry &registry = targetInst.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetInst.getObjectID(), targetInst.getInstanceID());

	if (!acInst) return false;
	acInst->set({ACL_2, serverShortId}, INT_T(acl & ALL_INST_RIGHTS));

	return true;
}

void Lwm2mAccessControl::removeAcl(Instance &targetInst, ID_T serverShortId) {
	WppRegistry &registry = targetInst.getRegistry();
	auto &acObj = registry.lwm2mAccessControl();
	Lwm2mAccessControl *acInst = getAcInstForTarget(acObj, targetInst.getObjectID(), targetInst.getInstanceID());

	if (!acInst) return;

	INT_T acl;
	if (!acInst->get({ACL_2, serverShortId}, acl)) return;
	acInst->remove({ACL_2, serverShortId});
}
#endif
/* --------------- Code_cpp block 11 end --------------- */

} /* namespace wpp */
