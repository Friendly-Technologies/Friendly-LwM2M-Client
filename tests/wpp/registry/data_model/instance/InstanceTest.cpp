#include "catch_amalgamated.hpp"
#include "Instance.h"

using namespace wpp;

static lwm2m_context_t mockContext = {};
static const OBJ_LINK_T mockId = {0 , 0};

class InstanceMock : public Instance {
public:
    InstanceMock(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {
        std::vector<Resource> resources = {                     
		{0, ResOp(ResOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::STRING },                                 
		{1, ResOp(ResOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::TIME },                                       
		{2, ResOp(ResOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::INT },                   
		{3, ResOp(ResOp::READ), IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL, TYPE_ID::STRING },                             
		{4, ResOp(ResOp::EXECUTE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE },  
		};
	    _resources = std::move(resources);
    }

    void setDefaultState() override {}

	void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) override {}

	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) override {}
};

TEST_CASE("Instance: info", "[getLink][getObjectID][getInstanceID]") {
    InstanceMock instance(mockContext, mockId);
    
	SECTION("getLink") {
		REQUIRE(instance.getLink().objId == mockId.objId);
		REQUIRE(instance.getLink().objInstId == mockId.objInstId);
	}

	SECTION("getObjectID") {
		REQUIRE(instance.getObjectID() == (OBJ_ID)mockId.objId);
	}

	SECTION("getInstanceID") {
		REQUIRE(instance.getInstanceID() == mockId.objInstId);
	}
}

TEST_CASE("Instance: resource access", "[set][setMove][get][getPtr][clear][remove]") {
    InstanceMock instance(mockContext, mockId);
}