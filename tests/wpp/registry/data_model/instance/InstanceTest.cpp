#include "catch_amalgamated.hpp"
#include "Instance.h"

using namespace wpp;

static lwm2m_context_t mockContext = {};
static const OBJ_LINK_T mockId = {0 , 0};

class InstanceMock : public Instance {
public:
	int userOpReadCnt = 0;
	int userOpWriteUpdCnt = 0;
	int userOpDeleteCnt = 0;

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

	void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) override {
		switch (type) {
		case ResOp::READ: 
			userOpReadCnt++;
			break;
		case ResOp::WRITE_UPD:
			userOpWriteUpdCnt++;
			break;
		case ResOp::DELETE:
			userOpDeleteCnt++;
			break;
		default:
			break;
		}
	}
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

	SECTION("set") {
		REQUIRE(instance.userOpWriteUpdCnt == 0);
		REQUIRE(instance.set(0, (STRING_T)"test1"));
		REQUIRE(instance.userOpWriteUpdCnt == 1);
		REQUIRE(instance.set(1, (TIME_T)123));
		REQUIRE(instance.userOpWriteUpdCnt == 2);
		REQUIRE(instance.set(2, (INT_T)123));
		REQUIRE(instance.userOpWriteUpdCnt == 3);
		REQUIRE(instance.set({3, 0}, (STRING_T)"test2"));
		REQUIRE(instance.userOpWriteUpdCnt == 4);
		REQUIRE(instance.set({3, 1}, (STRING_T)"test3"));
		REQUIRE(instance.userOpWriteUpdCnt == 5);
		EXECUTE_T exec = (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) { return true; };
		REQUIRE(instance.set(4, exec));
		REQUIRE(instance.userOpWriteUpdCnt == 6);
		REQUIRE_FALSE(instance.set(5, (STRING_T)"test4"));
		REQUIRE(instance.userOpWriteUpdCnt == 6);
	}

	SECTION("setMove") {
		STRING_T str = "test";
		TIME_T time = 123;
		INT_T intVal = 123;
		REQUIRE(instance.userOpWriteUpdCnt == 0);
		REQUIRE(instance.setMove(0, str));
		REQUIRE(instance.userOpWriteUpdCnt == 1);
		REQUIRE(instance.setMove(1, time));
		REQUIRE(instance.userOpWriteUpdCnt == 2);
		REQUIRE(instance.setMove(2, intVal));
		REQUIRE(instance.userOpWriteUpdCnt == 3);
		REQUIRE(instance.setMove({3, 0}, str));
		REQUIRE(instance.userOpWriteUpdCnt == 4);
		REQUIRE(instance.setMove({3, 1}, str));
		REQUIRE(instance.userOpWriteUpdCnt == 5);
		EXECUTE_T exec = (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) { return true; };
		REQUIRE(instance.setMove(4, exec));
		REQUIRE(instance.userOpWriteUpdCnt == 6);
		REQUIRE_FALSE(instance.setMove(5, str));
		REQUIRE(instance.userOpWriteUpdCnt == 6);
	}

	SECTION("get") {
		STRING_T str = "test";
		TIME_T time = 123;
		INT_T intVal = 123;
		REQUIRE(instance.set(0, str));
		REQUIRE(instance.set(1, time));
		REQUIRE(instance.set(2, intVal));
		REQUIRE(instance.set({3, 0}, str));
		REQUIRE(instance.set({3, 1}, str));
		EXECUTE_T exec = (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) { return true; };
		REQUIRE(instance.set(4, exec));

		STRING_T strGet;
		TIME_T timeGet;
		INT_T intValGet;
		EXECUTE_T execGet;
		REQUIRE(instance.userOpReadCnt == 0);
		REQUIRE(instance.get(0, strGet));
		REQUIRE(instance.userOpReadCnt == 1);
		REQUIRE(strGet == str);
		REQUIRE(instance.get(1, timeGet));
		REQUIRE(instance.userOpReadCnt == 2);
		REQUIRE(timeGet == time);
		REQUIRE(instance.get(2, intValGet));
		REQUIRE(instance.userOpReadCnt == 3);
		REQUIRE(intValGet == intVal);
		strGet = "";
		REQUIRE(instance.get({3, 0}, strGet));
		REQUIRE(instance.userOpReadCnt == 4);
		REQUIRE(strGet == str);
		strGet = "";
		REQUIRE(instance.get({3, 1}, strGet));
		REQUIRE(instance.userOpReadCnt == 5);
		REQUIRE(strGet == str);
		REQUIRE(instance.get(4, execGet));
		REQUIRE(instance.userOpReadCnt == 6);
		REQUIRE(execGet(instance, 4, OPAQUE_T()));
		REQUIRE_FALSE(instance.get(5, strGet));
		REQUIRE(instance.userOpReadCnt == 6);
	}

	SECTION("getPtr") {
		STRING_T str = "test";
		TIME_T time = 123;
		INT_T intVal = 123;
		REQUIRE(instance.set(0, str));
		REQUIRE(instance.set(1, time));
		REQUIRE(instance.set(2, intVal));
		REQUIRE(instance.set({3, 0}, str));
		REQUIRE(instance.set({3, 1}, str));
		EXECUTE_T exec = (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) { return true; };
		REQUIRE(instance.set(4, exec));

		const STRING_T* strGet;
		const TIME_T* timeGet;
		const INT_T* intValGet;
		const EXECUTE_T* execGet;
		REQUIRE(instance.userOpReadCnt == 0);
		REQUIRE(instance.getPtr(0, &strGet));
		REQUIRE(instance.userOpReadCnt == 1);
		REQUIRE(*strGet == str);
		REQUIRE(instance.getPtr(1, &timeGet));
		REQUIRE(instance.userOpReadCnt == 2);
		REQUIRE(*timeGet == time);
		REQUIRE(instance.getPtr(2, &intValGet));
		REQUIRE(instance.userOpReadCnt == 3);
		REQUIRE(*intValGet == intVal);
		strGet = nullptr;
		REQUIRE(instance.getPtr({3, 0}, &strGet));
		REQUIRE(instance.userOpReadCnt == 4);
		REQUIRE(*strGet == str);
		strGet = nullptr;
		REQUIRE(instance.getPtr({3, 1}, &strGet));
		REQUIRE(instance.userOpReadCnt == 5);
		REQUIRE(*strGet == str);
		REQUIRE(instance.getPtr(4, &execGet));
		REQUIRE(instance.userOpReadCnt == 6);
		REQUIRE((*execGet)(instance, 4, OPAQUE_T()));
		REQUIRE_FALSE(instance.getPtr(5, &strGet));
		REQUIRE(instance.userOpReadCnt == 6);
	}

	SECTION("clear") {
		STRING_T str = "test";
		TIME_T time = 123;
		INT_T intVal = 123;
		REQUIRE(instance.set(0, str));
		REQUIRE(instance.set(1, time));
		REQUIRE(instance.set(2, intVal));
		REQUIRE(instance.set({3, 0}, str));
		REQUIRE(instance.set({3, 1}, str));
		EXECUTE_T exec = (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) { return true; };
		REQUIRE(instance.set(4, exec));

		STRING_T strGet;
		TIME_T timeGet;
		INT_T intValGet;
		EXECUTE_T execGet;
		REQUIRE(instance.userOpDeleteCnt == 0);
		REQUIRE(instance.clear(0));
		REQUIRE(instance.userOpDeleteCnt == 1);
		REQUIRE_FALSE(instance.get(0, strGet));
		REQUIRE(instance.clear(1));
		REQUIRE(instance.userOpDeleteCnt == 2);
		REQUIRE_FALSE(instance.get(1, timeGet));
		REQUIRE(instance.clear(2));
		REQUIRE(instance.userOpDeleteCnt == 3);
		REQUIRE_FALSE(instance.get(2, intValGet));
		REQUIRE(instance.clear(3));
		REQUIRE(instance.userOpDeleteCnt == 4);
		REQUIRE_FALSE(instance.get({3, 0}, strGet));
		REQUIRE_FALSE(instance.get({3, 1}, strGet));
		REQUIRE(instance.clear(4));
		REQUIRE(instance.userOpDeleteCnt == 5);
		REQUIRE_FALSE(instance.get(4, execGet));
		REQUIRE_FALSE(instance.clear(5));
		REQUIRE(instance.userOpDeleteCnt == 5);
	}

	SECTION("remove") {
		STRING_T str = "test";
		TIME_T time = 123;
		INT_T intVal = 123;
		REQUIRE(instance.set(0, str));
		REQUIRE(instance.set(1, time));
		REQUIRE(instance.set(2, intVal));
		REQUIRE(instance.set({3, 0}, str));
		REQUIRE(instance.set({3, 1}, str));
		EXECUTE_T exec = (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) { return true; };
		REQUIRE(instance.set(4, exec));

		STRING_T strGet;
		TIME_T timeGet;
		INT_T intValGet;
		EXECUTE_T execGet;
		REQUIRE(instance.userOpDeleteCnt == 0);
		REQUIRE(instance.get(0, strGet));
		REQUIRE_FALSE(instance.remove({0,}));
		REQUIRE(instance.userOpDeleteCnt == 0);
		REQUIRE(instance.get(0, strGet));

		REQUIRE(instance.get(1, timeGet));
		REQUIRE_FALSE(instance.remove({1,}));
		REQUIRE(instance.userOpDeleteCnt == 0);
		REQUIRE(instance.get(1, timeGet));
		
		REQUIRE(instance.get(2, intValGet));
		REQUIRE_FALSE(instance.remove({2,}));
		REQUIRE(instance.userOpDeleteCnt == 0);
		REQUIRE(instance.get(2, intValGet));
		
		REQUIRE(instance.get({3, 0}, strGet));
		REQUIRE(instance.get({3, 1}, strGet));
		REQUIRE(instance.remove({3, 0}));
		REQUIRE(instance.userOpDeleteCnt == 1);
		REQUIRE_FALSE(instance.get({3, 0}, strGet));
		REQUIRE(instance.get({3, 1}, strGet));
		
		REQUIRE_FALSE(instance.remove({3, 1}));
		REQUIRE(instance.userOpDeleteCnt == 1);
		REQUIRE(instance.get({3, 1}, strGet));

		REQUIRE(instance.get(4, execGet));
		REQUIRE_FALSE(instance.remove({4, 0}));
		REQUIRE(instance.userOpDeleteCnt == 1);
		REQUIRE(instance.get(4, execGet));

		REQUIRE_FALSE(instance.remove({5, 0}));
		REQUIRE(instance.userOpDeleteCnt == 1);
	}	
}