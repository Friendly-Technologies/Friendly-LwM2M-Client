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

	Instance* createInstance(ID_T instanceID) override {
        return ObjectSpec::createInstance(instanceID);
    }

    InstanceMock* createInstanceSpec(ID_T instanceID) {
        return ObjectSpec::createInstanceSpec(instanceID);
    }

    InstanceMock* instanceSpec(ID_T instanceID) {
        return ObjectSpec::instanceSpec(instanceID);
    }
};

TEST_CASE("Object: constructing/destucting and getting infor", "[Object][ObjectSpec][getObjectID][getLwm2mObject][getObjectInfo][instanceCnt]") {
    SECTION("Constructor") {
        ObjectSpecMock obj(mockContext, mockInfo);

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
		ObjectSpecMock obj(mockContext, mockInfo);
		REQUIRE(obj.createInstance(0) != NULL);
		REQUIRE(obj.instanceCnt() == 1);
	}
}