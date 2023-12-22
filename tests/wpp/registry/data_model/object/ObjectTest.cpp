#include "catch_amalgamated.hpp"
#include "ObjectSpec.h"
#include "Instance.h"
#include <cstring>

using namespace wpp;

lwm2m_context_t mockContext = {};
const ObjectInfo mockInfo = {
    "Test Object",
	OBJ_ID::MAX_ID,
	"urn:oma:lwm2m:oma:123:1",
	Version {1, 0},
	Version {1, 0},
	IS_SINGLE::MULTIPLE,
	IS_MANDATORY::OPTIONAL,
	InstOp(InstOp::CREATE | InstOp::DELETE),
	ResOp(ResOp::READ|
			ResOp::WRITE|
			ResOp::DISCOVER|
			ResOp::EXECUTE|
			ResOp::DELETE),
};

class InstanceMock : public Instance {
public:
    InstanceMock(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {}
    
    void setDefaultState() override {}
    
	void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) override {}
	
	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) override {}
};

class ObjectSpecMock : public ObjectSpec<InstanceMock> {
public:
	ObjectSpecMock(lwm2m_context_t &context, const ObjectInfo &info): ObjectSpec(context, info) {}
	~ObjectSpecMock() {};

	Instance* createInstance(ID_T instanceID = ID_T_MAX_VAL) override {
        return ObjectSpec::createInstance(instanceID);
    }

    InstanceMock* createInstanceSpec(ID_T instanceID = ID_T_MAX_VAL) {
        return ObjectSpec::createInstanceSpec(instanceID);
    }

    InstanceMock* instanceSpec(ID_T instanceID = 0) {
        return ObjectSpec::instanceSpec(instanceID);
    }

	lwm2m_context_t *getContext() {
		return &_context;
	}
};

class ObjOpObserverTest : public ObjOpObserver {
public:
    int instanceCreatedCount = 0;
    int instanceDeletingCount = 0;

    void instanceCreated(Object &object, ID_T instanceId) override {
        ObjOpObserver::instanceCreated(object, instanceId);
        instanceCreatedCount++;
    }

    void instanceDeleting(Object &object, ID_T instanceId) override {
        ObjOpObserver::instanceDeleting(object, instanceId);
        instanceDeletingCount++;
    }
};

class ObjActObserverTest : public ObjActObserver {
public:
    int objectRestoreCount = 0;

    void objectRestore(Object &object) override {
        objectRestoreCount++;
    }
};

TEST_CASE("Object: constructing/destucting and getting infor", "[Object][ObjectSpec][getObjectID][getLwm2mObject][getObjectInfo]") {
	ObjectSpecMock obj(mockContext, mockInfo);

    SECTION("Constructor") {
		REQUIRE(obj.getObjectID() == OBJ_ID::MAX_ID);

		REQUIRE(std::strcmp(obj.getObjectInfo().name, "Test Object") == 0);
		REQUIRE(obj.getObjectInfo().objID == OBJ_ID::MAX_ID);
		REQUIRE(std::strcmp(obj.getObjectInfo().urn, "urn:oma:lwm2m:oma:123:1") == 0);
		REQUIRE(obj.getObjectInfo().lwm2mVersion.minor == 0);
		REQUIRE(obj.getObjectInfo().lwm2mVersion.major == 1);
		REQUIRE(obj.getObjectInfo().objVersion.minor == 0);
		REQUIRE(obj.getObjectInfo().objVersion.major == 1);
		REQUIRE(obj.getObjectInfo().isSingle == IS_SINGLE::MULTIPLE);
		REQUIRE(obj.getObjectInfo().isMandatory == IS_MANDATORY::OPTIONAL);
		REQUIRE(obj.getObjectInfo().instOperation.getFlags() == InstOp(InstOp::CREATE | InstOp::DELETE).getFlags());
		REQUIRE(obj.getObjectInfo().resOperation.getFlags() == ResOp(ResOp::READ|
													ResOp::WRITE|
													ResOp::DISCOVER|
													ResOp::EXECUTE|
													ResOp::DELETE).getFlags());

		lwm2m_object_t lwm2mObj = obj.getLwm2mObject();
		REQUIRE(lwm2mObj.objID == (ID_T)OBJ_ID::MAX_ID);
		REQUIRE(lwm2mObj.instanceList == NULL);
		REQUIRE(lwm2mObj.versionMajor == 1);
		REQUIRE(lwm2mObj.versionMinor == 0);
		REQUIRE(lwm2mObj.userData == &obj);
		REQUIRE(lwm2mObj.readFunc != NULL);
		REQUIRE(lwm2mObj.discoverFunc != NULL);
		REQUIRE(lwm2mObj.writeFunc != NULL);
		REQUIRE(lwm2mObj.executeFunc != NULL);
		REQUIRE(lwm2mObj.createFunc != NULL);
		REQUIRE(lwm2mObj.deleteFunc != NULL);

		ObjectInfo constrainedObjInfo = mockInfo;
		constrainedObjInfo.resOperation = ResOp(ResOp::NONE);
		constrainedObjInfo.instOperation = InstOp(InstOp::NONE);
		ObjectSpecMock constrainedObj(mockContext, constrainedObjInfo);
		lwm2m_object_t constrainedLwm2mObj = constrainedObj.getLwm2mObject();
		REQUIRE(constrainedLwm2mObj.userData == &constrainedObj);
		REQUIRE(constrainedLwm2mObj.readFunc == NULL);
		REQUIRE(constrainedLwm2mObj.discoverFunc == NULL);
		REQUIRE(constrainedLwm2mObj.writeFunc == NULL);
		REQUIRE(constrainedLwm2mObj.executeFunc == NULL);
		REQUIRE(constrainedLwm2mObj.createFunc == NULL);
		REQUIRE(constrainedLwm2mObj.deleteFunc == NULL);
    }

	SECTION("Object: Destructor") {
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.instanceCnt() == 1);
	}
}

TEST_CASE("Object: instance creating/removing", "[createInstance][createInstanceSpec][removeInstance][clear][instanceCnt][isInstanceExist][restore]") {
	ObjectSpecMock obj(mockContext, mockInfo);
	ObjActObserverTest actObserver;
	obj.actSubscribe(&actObserver);

	SECTION("createInstance") {
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.instanceCnt() == 1);
		REQUIRE(obj.isInstanceExist(0));
		REQUIRE(obj.createInstance(0) == NULL);
		REQUIRE(obj.instanceCnt() == 1);
		REQUIRE(obj.createInstance(1) != NULL);
		REQUIRE(obj.instanceCnt() == 2);
		REQUIRE(obj.isInstanceExist(1));
		REQUIRE(obj.createInstance(1) == NULL);
		REQUIRE(obj.instanceCnt() == 2);

		Instance *instance = obj.createInstance();
		REQUIRE(instance != NULL);
		REQUIRE(obj.isInstanceExist(2));
		REQUIRE(obj.instanceCnt() == 3);
		REQUIRE(instance->getInstanceID() == 2);

		REQUIRE(obj.createInstance(4) != NULL);
		REQUIRE(obj.instanceCnt() == 4);
		instance = obj.createInstance();
		REQUIRE(instance != NULL);
		REQUIRE(obj.instanceCnt() == 5);
		REQUIRE(instance->getInstanceID() == 5);

		ObjectInfo constrainedObjInfo = mockInfo;
		constrainedObjInfo.isSingle = IS_SINGLE::SINGLE;
		ObjectSpecMock constrainedObj(mockContext, constrainedObjInfo);
		REQUIRE(constrainedObj.createInstance(0) != NULL);
		REQUIRE(constrainedObj.instanceCnt() == 1);
		REQUIRE(constrainedObj.createInstance(2) == NULL);
		REQUIRE(constrainedObj.instanceCnt() == 1);
		REQUIRE(constrainedObj.createInstance() == NULL);
		REQUIRE(constrainedObj.instanceCnt() == 1);
	}

	SECTION("createInstanceSpec") {
		InstanceMock *instance = obj.createInstanceSpec(0);
		REQUIRE(instance != NULL);
		REQUIRE(obj.instanceCnt() == 1);
		REQUIRE(instance->getInstanceID() == 0);
		REQUIRE(obj.createInstanceSpec(0) == NULL);
	}

	SECTION("removeInstance") {
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.createInstance(1) != NULL);
		REQUIRE(obj.createInstance(2) != NULL);
		REQUIRE(obj.createInstance(3) != NULL);
		REQUIRE(obj.createInstance(4) != NULL);

		REQUIRE_FALSE(obj.removeInstance(5));
		REQUIRE(obj.instanceCnt() == 5);
		REQUIRE_FALSE(obj.removeInstance(ID_T_MAX_VAL));
		REQUIRE(obj.instanceCnt() == 5);

		REQUIRE(obj.removeInstance(0));
		REQUIRE_FALSE(obj.isInstanceExist(0));
		REQUIRE(obj.instanceCnt() == 4);
		REQUIRE_FALSE(obj.removeInstance(0));
		REQUIRE(obj.instanceCnt() == 4);
		REQUIRE(obj.removeInstance(1));
		REQUIRE_FALSE(obj.isInstanceExist(1));
		REQUIRE(obj.instanceCnt() == 3);
		REQUIRE(obj.removeInstance(2));
		REQUIRE_FALSE(obj.isInstanceExist(2));
		REQUIRE(obj.instanceCnt() == 2);
		REQUIRE(obj.removeInstance(3));
		REQUIRE_FALSE(obj.isInstanceExist(3));
		REQUIRE(obj.instanceCnt() == 1);
		REQUIRE(obj.removeInstance(4));
		REQUIRE_FALSE(obj.isInstanceExist(4));
		REQUIRE(obj.instanceCnt() == 0);
	}

	SECTION("clear") {
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.createInstance(1) != NULL);
		REQUIRE(obj.createInstance(2) != NULL);
		REQUIRE(obj.createInstance(3) != NULL);
		REQUIRE(obj.createInstance(4) != NULL);

		obj.clear();
		REQUIRE(obj.instanceCnt() == 0);
	}

	SECTION("restore") {
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.instanceCnt() == 1);
		obj.restore();
		REQUIRE(obj.instanceCnt() == 1);
		REQUIRE(actObserver.objectRestoreCount == 1);
	}
}

TEST_CASE("Object: instance accessing", "[instance][instanceSpec][getInstances]") {
	ObjectSpecMock obj(mockContext, mockInfo);
	ObjActObserverTest actObserver;
	obj.actSubscribe(&actObserver);

	SECTION("instance") {
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.createInstance(1) != NULL);
		REQUIRE(obj.createInstance(2) != NULL);
		REQUIRE(obj.createInstance(3) != NULL);
		REQUIRE(obj.createInstance(4) != NULL);

		REQUIRE(obj.instance(0) != NULL);
		REQUIRE(obj.instance(1) != NULL);
		REQUIRE(obj.instance(2) != NULL);
		REQUIRE(obj.instance(3) != NULL);
		REQUIRE(obj.instance(4) != NULL);
		REQUIRE(obj.instance(5) == NULL);
		REQUIRE(obj.instance(ID_T_MAX_VAL) == NULL);
	}

	SECTION("instanceSpec") {
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.createInstance(1) != NULL);
		REQUIRE(obj.createInstance(2) != NULL);
		REQUIRE(obj.createInstance(3) != NULL);
		REQUIRE(obj.createInstance(4) != NULL);

		REQUIRE(obj.instanceSpec(0) != NULL);
		REQUIRE(obj.instanceSpec(1) != NULL);
		REQUIRE(obj.instanceSpec(2) != NULL);
		REQUIRE(obj.instanceSpec(3) != NULL);
		REQUIRE(obj.instanceSpec(4) != NULL);
		REQUIRE(obj.instanceSpec(5) == NULL);
		REQUIRE(obj.instanceSpec(ID_T_MAX_VAL) == NULL);
	}

	SECTION("getInstances") {
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.createInstance(1) != NULL);
		REQUIRE(obj.createInstance(2) != NULL);
		REQUIRE(obj.createInstance(3) != NULL);
		REQUIRE(obj.createInstance(4) != NULL);

		const std::vector<Instance*> &instances = obj.getInstances();
		REQUIRE(instances.size() == 5);
		for (int i = 0; i < 5; i++) {
			REQUIRE(instances[i] != NULL);
			REQUIRE(instances[i]->getInstanceID() == i);
		}
	}
}

TEST_CASE("Object: internals", "[getFirstAvailableInstanceID][serverRead_clb][serverWrite_clb][serverExecute_clb][serverDiscover_clb][serverCreate_clb][serverDelete_clb]") {
	ObjectSpecMock obj(mockContext, mockInfo);
	ObjOpObserverTest opObserver;
	obj.opSubscribe(&opObserver);

	SECTION("getFirstAvailableInstanceID") {
		ObjectSpecMock obj1(mockContext, mockInfo);
		for (int i = 0; i < ID_T_MAX_VAL; i++) {
			REQUIRE(obj1.createInstance(i) != NULL);
		}
		REQUIRE(obj1.instanceCnt() == ID_T_MAX_VAL);
		REQUIRE(obj1.createInstance() == NULL);
		REQUIRE(obj1.instanceCnt() == ID_T_MAX_VAL);

		ObjectSpecMock obj2(mockContext, mockInfo);
		REQUIRE(obj2.createInstance(0) != NULL);
		REQUIRE(obj2.createInstance(4) != NULL);
		for (int i = 5; i < ID_T_MAX_VAL; i++) {
			REQUIRE(obj2.createInstance(i) != NULL);
		}
		REQUIRE(obj2.instanceCnt() == (ID_T_MAX_VAL - 3));

		Instance *inst = obj2.createInstance();
		REQUIRE(inst != NULL);
		REQUIRE(inst->getInstanceID() == 1);
		inst = obj2.createInstance();
		REQUIRE(inst != NULL);
		REQUIRE(inst->getInstanceID() == 2);
		inst = obj2.createInstance();
		REQUIRE(inst != NULL);
		REQUIRE(inst->getInstanceID() == 3);

		REQUIRE(obj2.instanceCnt() == ID_T_MAX_VAL);
		REQUIRE(obj2.createInstance() == NULL);
		REQUIRE(obj2.instanceCnt() == ID_T_MAX_VAL);
	}

	SECTION("serverRead_clb") {
		lwm2m_object_t &lwm2mObj = obj.getLwm2mObject();
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(lwm2mObj.readFunc(&mockContext, 0, NULL, NULL, &lwm2mObj) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(lwm2mObj.readFunc(&mockContext, 1, NULL, NULL, &lwm2mObj) == COAP_404_NOT_FOUND);
	}

	SECTION("serverWrite_clb") {
		lwm2m_object_t &lwm2mObj = obj.getLwm2mObject();
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(lwm2mObj.writeFunc(&mockContext, 0, 0, NULL, &lwm2mObj, LWM2M_WRITE_PARTIAL_UPDATE) == COAP_204_CHANGED);
		REQUIRE(lwm2mObj.writeFunc(&mockContext, 0, 1, NULL, &lwm2mObj, LWM2M_WRITE_PARTIAL_UPDATE) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(lwm2mObj.writeFunc(&mockContext, 1, 0, NULL, &lwm2mObj, LWM2M_WRITE_PARTIAL_UPDATE) == COAP_404_NOT_FOUND);
	}

	SECTION("serverExecute_clb") {
		lwm2m_object_t &lwm2mObj = obj.getLwm2mObject();
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(lwm2mObj.executeFunc(&mockContext, 0, 0, NULL, 0, &lwm2mObj) == COAP_404_NOT_FOUND);
		REQUIRE(lwm2mObj.executeFunc(&mockContext, 1, 0, NULL, 0, &lwm2mObj) == COAP_404_NOT_FOUND);
	}

	SECTION("serverDiscover_clb") {
		lwm2m_object_t &lwm2mObj = obj.getLwm2mObject();
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(lwm2mObj.discoverFunc(&mockContext, 0, NULL, NULL, &lwm2mObj) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(lwm2mObj.discoverFunc(&mockContext, 1, NULL, NULL, &lwm2mObj) == COAP_404_NOT_FOUND);
	}

	SECTION("serverCreate_clb") {
		REQUIRE(obj.instanceCnt() == 0);
		lwm2m_object_t &lwm2mObj = obj.getLwm2mObject();
		REQUIRE(lwm2mObj.createFunc(&mockContext, 0, 0, NULL, &lwm2mObj) == COAP_201_CREATED);
		REQUIRE(obj.instanceCnt() == 1);
		REQUIRE(obj.isInstanceExist(0));
		REQUIRE(opObserver.instanceCreatedCount == 1);
		
		REQUIRE(lwm2mObj.createFunc(&mockContext, 1, 0, NULL, &lwm2mObj) == COAP_201_CREATED);
		REQUIRE(obj.instanceCnt() == 2);
		REQUIRE(obj.isInstanceExist(1));
		REQUIRE(opObserver.instanceCreatedCount == 2);

		REQUIRE(lwm2mObj.createFunc(&mockContext, 1, 0, NULL, &lwm2mObj) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(opObserver.instanceCreatedCount == 2);

		REQUIRE(opObserver.instanceDeletingCount == 0);
		REQUIRE(lwm2mObj.createFunc(&mockContext, 2, 1, NULL, &lwm2mObj) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(obj.instanceCnt() == 2);
		REQUIRE_FALSE(obj.isInstanceExist(2));
		REQUIRE(opObserver.instanceCreatedCount == 3);
		REQUIRE(opObserver.instanceDeletingCount == 1);
	}

	SECTION("serverDelete_clb") {
		lwm2m_object_t &lwm2mObj = obj.getLwm2mObject();
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.createInstance(1) != NULL);
		REQUIRE(obj.createInstance(2) != NULL);
		REQUIRE(obj.createInstance(3) != NULL);
		REQUIRE(obj.createInstance(4) != NULL);
		REQUIRE(obj.instanceCnt() == 5);

		REQUIRE(lwm2mObj.deleteFunc(&mockContext, 0, &lwm2mObj) == COAP_202_DELETED);
		REQUIRE(obj.instanceCnt() == 4);
		REQUIRE_FALSE(obj.isInstanceExist(0));
		REQUIRE(opObserver.instanceDeletingCount == 1);

		REQUIRE(lwm2mObj.deleteFunc(&mockContext, 0, &lwm2mObj) == COAP_404_NOT_FOUND);
		REQUIRE(obj.instanceCnt() == 4);
		REQUIRE(opObserver.instanceDeletingCount == 1);

		REQUIRE(lwm2mObj.deleteFunc(&mockContext, 1, &lwm2mObj) == COAP_202_DELETED);
		REQUIRE(obj.instanceCnt() == 3);
		REQUIRE_FALSE(obj.isInstanceExist(1));
		REQUIRE(opObserver.instanceDeletingCount == 2);

		REQUIRE(lwm2mObj.deleteFunc(&mockContext, 2, &lwm2mObj) == COAP_202_DELETED);
		REQUIRE(obj.instanceCnt() == 2);
		REQUIRE_FALSE(obj.isInstanceExist(2));
		REQUIRE(opObserver.instanceDeletingCount == 3);

		REQUIRE(lwm2mObj.deleteFunc(&mockContext, 3, &lwm2mObj) == COAP_202_DELETED);
		REQUIRE(obj.instanceCnt() == 1);
		REQUIRE_FALSE(obj.isInstanceExist(3));
		REQUIRE(opObserver.instanceDeletingCount == 4);

		REQUIRE(lwm2mObj.deleteFunc(&mockContext, 4, &lwm2mObj) == COAP_202_DELETED);
		REQUIRE(obj.instanceCnt() == 0);
		REQUIRE_FALSE(obj.isInstanceExist(4));
		REQUIRE(opObserver.instanceDeletingCount == 5);

		REQUIRE(lwm2mObj.deleteFunc(&mockContext, 5, &lwm2mObj) == COAP_404_NOT_FOUND);
		REQUIRE(obj.instanceCnt() == 0);
		REQUIRE(opObserver.instanceDeletingCount == 5);
	}
}