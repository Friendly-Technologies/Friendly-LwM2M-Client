#include "catch_amalgamated.hpp"
#include "ObjectSpec.h"
#include "Instance.h"

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

TEST_CASE("Object constructing/destructing", "[Object][ObjectSpec]") {
    SECTION("Constructor") {
        ObjectSpecMock obj(mockContext, mockInfo);
    }
}