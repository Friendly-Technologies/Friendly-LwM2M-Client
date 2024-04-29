#include "catch_amalgamated.hpp"
#include "Instance.h"

using namespace wpp;

static lwm2m_context_t mockContext = {};
static const OBJ_LINK_T mockId = {0, 0};

class InstanceMock : public Instance {
public:
	int setDefaultStateCnt = 0;

	int userOpReadCnt = 0;
	int userOpWriteUpdCnt = 0;
	int userOpDeleteCnt = 0;

	int serverOpReadCnt = 0;
	int serverOpWriteUpdCnt = 0;
	int serverOpWriteReplaceResCnt = 0;
	int serverOpWriteReplaceInstCnt = 0;
	int serverOpExecuteCnt = 0;
	int serverOpDiscoverCnt = 0;

    InstanceMock(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {
       init();
    }

	void init() {
		std::vector<Resource> resources = {                     
			{0, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::STRING},                                 
			{1, ResOp(ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::TIME},                                       
			{2, ResOp(ResOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::TIME},                   
			{3, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL, TYPE_ID::STRING},                             
			{4, ResOp(ResOp::EXECUTE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE},
			{5, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::UINT},   
			{6, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::BOOL},
			{7, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::FLOAT},
			{8, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::OBJ_LINK},
			{9, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::OPAQUE},
			{10, ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::CORE_LINK},
		};
	    setupResources(std::move(resources));

		REQUIRE(resource(0)->set((STRING_T)"test1"));
		REQUIRE(resource(1)->set((TIME_T)123));
		REQUIRE(resource(2)->set((INT_T)12));
		REQUIRE(resource(3)->set((STRING_T)"test2", 0));
		REQUIRE(resource(3)->set((STRING_T)"test3", 1));
		EXECUTE_T exec = (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) { return true; };
		REQUIRE(resource(4)->set(exec));
		REQUIRE(resource(5)->set((UINT_T)12));
		REQUIRE(resource(6)->set((BOOL_T)true));
		REQUIRE(resource(7)->set((FLOAT_T)12.3));
		REQUIRE(resource(8)->set(OBJ_LINK_T()));
		REQUIRE(resource(9)->set(OPAQUE_T()));
		REQUIRE(resource(10)->set((CORE_LINK_T)"</3/0>"));
	}

    void setDefaultState() override {
		_resources.clear();
		init();
		setDefaultStateCnt++;
	}

	void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) override {
		switch (type) {
		case ResOp::READ:
			serverOpReadCnt++;
			break;
		case ResOp::WRITE_UPD:
			serverOpWriteUpdCnt++;
			break;
		case ResOp::WRITE_REPLACE_RES:
			serverOpWriteReplaceResCnt++;
			break;
		case ResOp::WRITE_REPLACE_INST:
			serverOpWriteReplaceInstCnt++;
			break;
		case ResOp::EXECUTE:
			serverOpExecuteCnt++;
			break;
		case ResOp::DISCOVER:
			serverOpDiscoverCnt++;
			break;
		default:
			break;
		}
	}

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
		REQUIRE_FALSE(instance.clear(100));
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

TEST_CASE("Instance: server operations", "[readAsServer][writeAsServer][executeAsServer][discoverAsServer]") {
	InstanceMock instance(mockContext, mockId);
	int numData = 0;
	lwm2m_data_t *dataArray = NULL;

	SECTION("readAsServer: Incorrect parameters") {
		REQUIRE(instance.readAsServer(NULL, &dataArray) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(instance.readAsServer(&numData, NULL) == COAP_500_INTERNAL_SERVER_ERROR);
	}

	SECTION("readAsServer: Read all resources when no one resource is set") {
		instance.clear(0);
		instance.clear(1);
		instance.clear(2);
		instance.clear(3);
		instance.clear(4);
		instance.clear(5);
		instance.clear(6);
		instance.clear(7);
		instance.clear(8);
		instance.clear(9);
		instance.clear(10);

		REQUIRE(numData == 0);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(instance.readAsServer(&numData, &dataArray) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(numData == 0);
		REQUIRE(dataArray == NULL);
	}

	SECTION("readAsServer: Read one resource when no one resource is set") {
		instance.clear(0);
		instance.clear(1);
		instance.clear(2);
		instance.clear(3);
		instance.clear(4);
		instance.clear(5);
		instance.clear(6);
		instance.clear(7);
		instance.clear(8);
		instance.clear(9);
		instance.clear(10);

		numData = 1;
		dataArray = new lwm2m_data_t;
		dataArray->id = 0;

		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(instance.readAsServer(&numData, &dataArray) == COAP_404_NOT_FOUND);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);
		
		delete dataArray;
	}
	
	SECTION("readAsServer: Read one resource when read is not supported for this resource") {
		numData = 1;
		dataArray = new lwm2m_data_t;
		dataArray->id = 1;

		REQUIRE(instance.set(1, (TIME_T)123));
		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(instance.readAsServer(&numData, &dataArray) == COAP_405_METHOD_NOT_ALLOWED);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);

		delete dataArray;
	}
	
	SECTION("readAsServer: Read one resource when it is not multiple") {
		numData = 1;
		dataArray = new lwm2m_data_t;
		dataArray->id = 0;
		dataArray->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
		
		REQUIRE(instance.set(0, (STRING_T)"test1"));

		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(instance.readAsServer(&numData, &dataArray) == COAP_405_METHOD_NOT_ALLOWED);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);

		delete dataArray;
	}
	
	SECTION("readAsServer: Read two resource when one is not set") {	
		numData = 2;
		dataArray = new lwm2m_data_t[2];
		dataArray[0].id = 0;
		dataArray[1].id = 1;
		dataArray[0].type = LWM2M_TYPE_UNDEFINED;
		dataArray[1].type = LWM2M_TYPE_UNDEFINED;

		REQUIRE(instance.set(0, (STRING_T)"test1"));

		REQUIRE(numData == 2);
		REQUIRE(dataArray != NULL);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE((int)instance.readAsServer(&numData, &dataArray) == (int)COAP_205_CONTENT);
		REQUIRE(instance.serverOpReadCnt == 1);
		REQUIRE(numData == 2);
		REQUIRE(dataArray != NULL);

		REQUIRE(dataArray[0].id == 0);
		REQUIRE(dataArray[0].type == LWM2M_TYPE_STRING);
		REQUIRE(dataArray[0].value.asBuffer.length == 5);
		REQUIRE(dataArray[0].value.asBuffer.buffer != NULL);
		REQUIRE(memcmp(dataArray[0].value.asBuffer.buffer, "test1", 5) == 0);

		REQUIRE(dataArray[1].id == 1);
		REQUIRE(dataArray[1].type == LWM2M_TYPE_UNDEFINED);

		delete dataArray[0].value.asBuffer.buffer;
		delete dataArray;
	}
	
	SECTION("readAsServer: Read multiple resource that is not set") {
		instance.clear(0);
		instance.clear(1);
		instance.clear(2);
		instance.clear(3);
		instance.clear(4);
		instance.clear(5);
		instance.clear(6);
		instance.clear(7);
		instance.clear(8);
		instance.clear(9);
		instance.clear(10);

		numData = 1;
		dataArray = new lwm2m_data_t[1];
		dataArray->id = 3;
		dataArray->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
		dataArray->value.asChildren.count = 2;
		dataArray->value.asChildren.array = new lwm2m_data_t[2];
		dataArray->value.asChildren.array[0].id = 0;
		dataArray->value.asChildren.array[0].type = LWM2M_TYPE_UNDEFINED;
		dataArray->value.asChildren.array[1].id = 1;
		dataArray->value.asChildren.array[1].type = LWM2M_TYPE_UNDEFINED;

		REQUIRE(instance.set({3, 0}, (STRING_T)"test2"));

		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE((int)instance.readAsServer(&numData, &dataArray) == (int)COAP_404_NOT_FOUND);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);

		REQUIRE(dataArray->id == 3);
		REQUIRE(dataArray->type == LWM2M_TYPE_MULTIPLE_RESOURCE);
		REQUIRE(dataArray->value.asChildren.count == 2);
		REQUIRE(dataArray->value.asChildren.array != NULL);
		REQUIRE(dataArray->value.asChildren.array[1].id == 1);
		REQUIRE(dataArray->value.asChildren.array[1].type == LWM2M_TYPE_UNDEFINED);

		delete dataArray->value.asChildren.array;
		delete dataArray;
	}
	
	SECTION("readAsServer: Read one resource") {
		numData = 1;
		dataArray = new lwm2m_data_t;
		dataArray->id = 0;
		dataArray->type = LWM2M_TYPE_UNDEFINED;

		REQUIRE(instance.set(0, (STRING_T)"test1"));
		
		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE((int)instance.readAsServer(&numData, &dataArray) == (int)COAP_205_CONTENT);
		REQUIRE(instance.serverOpReadCnt == 1);
		REQUIRE(numData == 1);
		REQUIRE(dataArray != NULL);

		REQUIRE(dataArray->id == 0);
		REQUIRE(dataArray->type == LWM2M_TYPE_STRING);
		REQUIRE(dataArray->value.asBuffer.length == 5);
		REQUIRE(dataArray->value.asBuffer.buffer != NULL);
		REQUIRE(memcmp(dataArray->value.asBuffer.buffer, "test1", 5) == 0);

		delete dataArray->value.asBuffer.buffer;
		delete dataArray;
	}
	
	SECTION("readAsServer: Read all resources") {
		numData = 0;
		dataArray = NULL;

		REQUIRE(numData == 0);
		REQUIRE(dataArray == NULL);
		REQUIRE(instance.serverOpReadCnt == 0);
		REQUIRE((int)instance.readAsServer(&numData, &dataArray) == (int)COAP_205_CONTENT);
		REQUIRE(instance.serverOpReadCnt == 10);
		REQUIRE(numData == 9);
		REQUIRE(dataArray != NULL);

		REQUIRE(dataArray[0].id == 0);
		REQUIRE(dataArray[0].type == LWM2M_TYPE_STRING);
		REQUIRE(dataArray[0].value.asBuffer.length == 5);
		REQUIRE(dataArray[0].value.asBuffer.buffer != NULL);
		REQUIRE(memcmp(dataArray[0].value.asBuffer.buffer, "test1", 5) == 0);
		REQUIRE(dataArray[1].id == 2);
		REQUIRE(dataArray[1].type == LWM2M_TYPE_INTEGER);
		REQUIRE(dataArray[1].value.asInteger == 12);
		REQUIRE(dataArray[2].id == 3);
		REQUIRE(dataArray[2].type == LWM2M_TYPE_MULTIPLE_RESOURCE);
		REQUIRE(dataArray[2].value.asChildren.count == 2);
		REQUIRE(dataArray[2].value.asChildren.array != NULL);
		REQUIRE(dataArray[2].value.asChildren.array[0].id == 0);
		REQUIRE(dataArray[2].value.asChildren.array[0].type == LWM2M_TYPE_STRING);
		REQUIRE(dataArray[2].value.asChildren.array[0].value.asBuffer.length == 5);
		REQUIRE(dataArray[2].value.asChildren.array[0].value.asBuffer.buffer != NULL);
		REQUIRE(memcmp(dataArray[2].value.asChildren.array[0].value.asBuffer.buffer, "test2", 5) == 0);
		REQUIRE(dataArray[2].value.asChildren.array[1].id == 1);
		REQUIRE(dataArray[2].value.asChildren.array[1].type == LWM2M_TYPE_STRING);
		REQUIRE(dataArray[2].value.asChildren.array[1].value.asBuffer.length == 5);
		REQUIRE(dataArray[2].value.asChildren.array[1].value.asBuffer.buffer != NULL);
		REQUIRE(memcmp(dataArray[2].value.asChildren.array[1].value.asBuffer.buffer, "test3", 5) == 0);
		REQUIRE(dataArray[3].id == 5);
		REQUIRE(dataArray[3].type == LWM2M_TYPE_UNSIGNED_INTEGER);
		REQUIRE(dataArray[3].value.asInteger == 12);
		REQUIRE(dataArray[4].id == 6);
		REQUIRE(dataArray[4].type == LWM2M_TYPE_BOOLEAN);
		REQUIRE(dataArray[4].value.asBoolean == true);
		REQUIRE(dataArray[5].id == 7);
		REQUIRE(dataArray[5].type == LWM2M_TYPE_FLOAT);
		REQUIRE((int)(dataArray[5].value.asFloat*10) == (int)123);
		REQUIRE(dataArray[6].id == 8);
		REQUIRE(dataArray[6].type == LWM2M_TYPE_OBJECT_LINK);
		REQUIRE(dataArray[6].value.asObjLink.objectId == ID_T_MAX_VAL);
		REQUIRE(dataArray[6].value.asObjLink.objectInstanceId == ID_T_MAX_VAL);
		REQUIRE(dataArray[7].id == 9);
		REQUIRE(dataArray[7].type == LWM2M_TYPE_OPAQUE);
		REQUIRE(dataArray[7].value.asBuffer.length == 0);
		REQUIRE(dataArray[7].value.asBuffer.buffer == NULL);
		REQUIRE(dataArray[8].id == 10);
		REQUIRE(dataArray[8].type == LWM2M_TYPE_CORE_LINK);
		REQUIRE(dataArray[8].value.asBuffer.length == 6);
		REQUIRE(dataArray[8].value.asBuffer.buffer != NULL);
		REQUIRE(memcmp(dataArray[8].value.asBuffer.buffer, "</3/0>", 6) == 0);
		
		delete dataArray[0].value.asBuffer.buffer;
		delete dataArray[2].value.asChildren.array[0].value.asBuffer.buffer;
		delete dataArray[2].value.asChildren.array[1].value.asBuffer.buffer;
		delete dataArray[2].value.asChildren.array;
		delete dataArray[8].value.asBuffer.buffer;
		delete dataArray;
	}

	SECTION("writeAsServer: Incorrect parameters") {
		REQUIRE(instance.writeAsServer(0, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == COAP_204_CHANGED);
		REQUIRE(instance.writeAsServer(1, NULL, LWM2M_WRITE_PARTIAL_UPDATE) == COAP_500_INTERNAL_SERVER_ERROR);
	}

	SECTION("writeAsServer: Write one resource that not set") {
		instance.clear(0);
		dataArray = new lwm2m_data_t;
		dataArray->id = 0;
		dataArray->type = LWM2M_TYPE_STRING;
		REQUIRE((int)instance.writeAsServer(1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_404_NOT_FOUND);
		delete dataArray;
	}

	SECTION("writeAsServer: Write one resource that is read only") {
		dataArray = new lwm2m_data_t;
		dataArray->id = 2;
		dataArray->type = LWM2M_TYPE_INTEGER;
		REQUIRE(instance.set(2, (INT_T)12));
		REQUIRE((int)instance.writeAsServer(1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_405_METHOD_NOT_ALLOWED);
		delete dataArray;
	}

	SECTION("writeAsServer: Write single resource as multiple") {
		dataArray = new lwm2m_data_t;
		dataArray->id = 0;
		dataArray->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
		REQUIRE(instance.set(0, (STRING_T)"test1"));
		REQUIRE((int)instance.writeAsServer(1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_405_METHOD_NOT_ALLOWED);
		delete dataArray;
	}

	SECTION("writeAsServer: Write multiple resource as single") {
		dataArray = new lwm2m_data_t;
		dataArray->id = 3;
		dataArray->type = LWM2M_TYPE_STRING;
		REQUIRE(instance.set({3, 0}, (STRING_T)"test1"));
		REQUIRE((int)instance.writeAsServer(1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_405_METHOD_NOT_ALLOWED);
		delete dataArray;
	}

	SECTION("writeAsServer: Write several resource with one incorrect") {
		dataArray = new lwm2m_data_t[3];
		dataArray->id = 0;
		dataArray->type = LWM2M_TYPE_STRING;
		dataArray->value.asBuffer.length = 5;
		dataArray->value.asBuffer.buffer = new uint8_t[5];
		memcpy(dataArray->value.asBuffer.buffer, "test1", 5);
		dataArray[1].id = 1;
		dataArray[1].type = LWM2M_TYPE_INTEGER;
		dataArray[1].value.asInteger = 12;
		dataArray[2].id = 100;
		dataArray[2].type = LWM2M_TYPE_INTEGER;
		dataArray[2].value.asInteger = 123;


		REQUIRE(instance.set(0, (STRING_T)""));
		REQUIRE(instance.set(1, (INT_T)0));

		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE((int)instance.writeAsServer(3, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_204_CHANGED);
		REQUIRE(instance.serverOpWriteUpdCnt == 2);
		
		STRING_T testStr;
		REQUIRE(instance.get(0, testStr));
		REQUIRE(testStr == "test1");
		INT_T testInt;
		REQUIRE(instance.get(1, testInt));
		REQUIRE(testInt == 12);
		REQUIRE_FALSE(instance.get(100, testInt));

		delete dataArray[0].value.asBuffer.buffer;
		delete dataArray;
	}

	SECTION("writeAsServer: Write upd one resource with incorrect data") {
		dataArray = new lwm2m_data_t;
		dataArray->id = 1;
		dataArray->type = LWM2M_TYPE_STRING;
		dataArray->value.asBuffer.length = 5;
		dataArray->value.asBuffer.buffer = new uint8_t[5];
		memcpy(dataArray->value.asBuffer.buffer, "test1", 5);

		REQUIRE(instance.set(1, (INT_T)0));
		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE((int)instance.writeAsServer(1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_400_BAD_REQUEST);
		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		INT_T testInt;
		REQUIRE(instance.get(1, testInt));
		REQUIRE(testInt == 0);

		delete dataArray->value.asBuffer.buffer;
		delete dataArray;
	}

	SECTION("writeAsServer: Write replace one resource with incorrect data") {
		dataArray = new lwm2m_data_t;
		dataArray->id = 1;
		dataArray->type = LWM2M_TYPE_STRING;
		dataArray->value.asBuffer.length = 5;
		dataArray->value.asBuffer.buffer = new uint8_t[5];
		memcpy(dataArray->value.asBuffer.buffer, "test1", 5);

		REQUIRE(instance.set(1, (INT_T)0));
		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE((int)instance.writeAsServer(1, dataArray, LWM2M_WRITE_REPLACE_RESOURCES) == (int)COAP_400_BAD_REQUEST);
		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		INT_T testInt;
		REQUIRE(instance.get(1, testInt));
		REQUIRE(testInt == 0);

		delete dataArray->value.asBuffer.buffer;
		delete dataArray;
	}

	SECTION("writeAsServer: Write one resource") {
		dataArray = new lwm2m_data_t;
		dataArray->id = 0;
		dataArray->type = LWM2M_TYPE_STRING;
		dataArray->value.asBuffer.length = 5;
		dataArray->value.asBuffer.buffer = new uint8_t[5];
		memcpy(dataArray->value.asBuffer.buffer, "test1", 5);

		REQUIRE(instance.set(0, (STRING_T)""));
		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE((int)instance.writeAsServer(1, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_204_CHANGED);
		REQUIRE(instance.serverOpWriteUpdCnt == 1);
		STRING_T testStr;
		REQUIRE(instance.get(0, testStr));
		REQUIRE(testStr == "test1");

		delete dataArray;
	}

	SECTION("writeAsServer: Write one resource replace") {
		dataArray = new lwm2m_data_t;
		dataArray->id = 0;
		dataArray->type = LWM2M_TYPE_STRING;
		dataArray->value.asBuffer.length = 5;
		dataArray->value.asBuffer.buffer = new uint8_t[5];
		memcpy(dataArray->value.asBuffer.buffer, "test1", 5);

		REQUIRE(instance.set(0, (STRING_T)""));
		REQUIRE(instance.serverOpWriteReplaceResCnt == 0);
		REQUIRE((int)instance.writeAsServer(1, dataArray, LWM2M_WRITE_REPLACE_RESOURCES) == (int)COAP_204_CHANGED);
		REQUIRE(instance.serverOpWriteReplaceResCnt == 1);
		STRING_T testStr;
		REQUIRE(instance.get(0, testStr));
		REQUIRE(testStr == "test1");

		delete dataArray;
	}

	SECTION("writeAsServer: Write several resource") {
		dataArray = new lwm2m_data_t[2];
		dataArray->id = 0;
		dataArray->type = LWM2M_TYPE_STRING;
		dataArray->value.asBuffer.length = 5;
		dataArray->value.asBuffer.buffer = new uint8_t[5];
		memcpy(dataArray->value.asBuffer.buffer, "test1", 5);
		dataArray[1].id = 3;
		dataArray[1].type = LWM2M_TYPE_MULTIPLE_RESOURCE;
		dataArray[1].value.asChildren.count = 2;
		dataArray[1].value.asChildren.array = new lwm2m_data_t[2];
		dataArray[1].value.asChildren.array[0].id = 0;
		dataArray[1].value.asChildren.array[0].type = LWM2M_TYPE_STRING;
		dataArray[1].value.asChildren.array[0].value.asBuffer.length = 5;
		dataArray[1].value.asChildren.array[0].value.asBuffer.buffer = new uint8_t[5];
		memcpy(dataArray[1].value.asChildren.array[0].value.asBuffer.buffer, "test2", 5);
		dataArray[1].value.asChildren.array[1].id = 1;
		dataArray[1].value.asChildren.array[1].type = LWM2M_TYPE_STRING;
		dataArray[1].value.asChildren.array[1].value.asBuffer.length = 5;
		dataArray[1].value.asChildren.array[1].value.asBuffer.buffer = new uint8_t[5];
		memcpy(dataArray[1].value.asChildren.array[1].value.asBuffer.buffer, "test3", 5);

		REQUIRE(instance.set(0, (STRING_T)""));
		REQUIRE(instance.set({3, 0}, (STRING_T)""));
		REQUIRE(instance.set({3, 1}, (STRING_T)""));

		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE((int)instance.writeAsServer(2, dataArray, LWM2M_WRITE_PARTIAL_UPDATE) == (int)COAP_204_CHANGED);
		REQUIRE(instance.serverOpWriteUpdCnt == 3);
		STRING_T testStr;
		REQUIRE(instance.get(0, testStr));
		REQUIRE(testStr == "test1");
		REQUIRE(instance.get({3, 0}, testStr));
		REQUIRE(testStr == "test2");
		REQUIRE(instance.get({3, 1}, testStr));
		REQUIRE(testStr == "test3");

		delete dataArray[0].value.asBuffer.buffer;
		delete dataArray[1].value.asChildren.array[0].value.asBuffer.buffer;
		delete dataArray[1].value.asChildren.array[1].value.asBuffer.buffer;
		delete dataArray[1].value.asChildren.array;
		delete dataArray;
	}

	SECTION("writeAsServer: Write resource with replace instance") {		
		dataArray = new lwm2m_data_t[4];
		dataArray[0].id = 0;
		dataArray[0].type = LWM2M_TYPE_STRING;
		dataArray[0].value.asBuffer.length = 4;
		dataArray[0].value.asBuffer.buffer = new uint8_t[4];
		memcpy(dataArray->value.asBuffer.buffer, "test", 4);
		dataArray[1].id = 1;
		dataArray[1].type = LWM2M_TYPE_INTEGER;
		dataArray[1].value.asInteger = 1;
		dataArray[2].id = 2;
		dataArray[2].type = LWM2M_TYPE_INTEGER;
		dataArray[2].value.asInteger = 2;
		dataArray[3].id = 3;
		dataArray[3].type = LWM2M_TYPE_MULTIPLE_RESOURCE;
		dataArray[3].value.asChildren.count = 1;
		dataArray[3].value.asChildren.array = new lwm2m_data_t[1];
		dataArray[3].value.asChildren.array[0].id = 0;
		dataArray[3].value.asChildren.array[0].type = LWM2M_TYPE_STRING;
		dataArray[3].value.asChildren.array[0].value.asBuffer.length = 4;
		dataArray[3].value.asChildren.array[0].value.asBuffer.buffer = new uint8_t[4];
		memcpy(dataArray[3].value.asChildren.array[0].value.asBuffer.buffer, "test", 4);

		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE(instance.serverOpWriteReplaceResCnt == 0);
		REQUIRE(instance.serverOpWriteReplaceInstCnt == 0);
		REQUIRE((int)instance.writeAsServer(4, dataArray, LWM2M_WRITE_REPLACE_INSTANCE) == (int)COAP_204_CHANGED);
		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE(instance.serverOpWriteReplaceResCnt == 0);
		REQUIRE(instance.serverOpWriteReplaceInstCnt == 1);
		STRING_T testStr;
		REQUIRE(instance.get(0, testStr));
		TIME_T testTime;
		REQUIRE(instance.get(1, testTime));
		REQUIRE(testTime == 1);
		REQUIRE(instance.get(2, testTime));
		REQUIRE(testTime == 2);
		REQUIRE(instance.get({3, 0}, testStr));
		REQUIRE(testStr == "test");
		REQUIRE_FALSE(instance.get({3, 1}, testStr));
		EXECUTE_T execGet;
		REQUIRE(instance.get(4, execGet));

		delete dataArray[0].value.asBuffer.buffer;
		delete dataArray[3].value.asChildren.array[0].value.asBuffer.buffer;
		delete dataArray[3].value.asChildren.array;
		delete dataArray;
	}

	SECTION("writeAsServer: Write all resources with replace") {		
		dataArray = new lwm2m_data_t[10];
		dataArray[0].id = 0;
		dataArray[0].type = LWM2M_TYPE_STRING;
		dataArray[0].value.asBuffer.length = 4;
		dataArray[0].value.asBuffer.buffer = new uint8_t[4];
		memcpy(dataArray->value.asBuffer.buffer, "test", 4);
		dataArray[1].id = 1;
		dataArray[1].type = LWM2M_TYPE_INTEGER;
		dataArray[1].value.asInteger = 1;
		dataArray[2].id = 2;
		dataArray[2].type = LWM2M_TYPE_INTEGER;
		dataArray[2].value.asInteger = 2;
		dataArray[3].id = 3;
		dataArray[3].type = LWM2M_TYPE_MULTIPLE_RESOURCE;
		dataArray[3].value.asChildren.count = 1;
		dataArray[3].value.asChildren.array = new lwm2m_data_t[1];
		dataArray[3].value.asChildren.array[0].id = 0;
		dataArray[3].value.asChildren.array[0].type = LWM2M_TYPE_STRING;
		dataArray[3].value.asChildren.array[0].value.asBuffer.length = 4;
		dataArray[3].value.asChildren.array[0].value.asBuffer.buffer = new uint8_t[4];
		memcpy(dataArray[3].value.asChildren.array[0].value.asBuffer.buffer, "test", 4);
		dataArray[4].id = 5;
		dataArray[4].type = LWM2M_TYPE_UNSIGNED_INTEGER;
		dataArray[4].value.asInteger = 178;
		dataArray[5].id = 6;
		dataArray[5].type = LWM2M_TYPE_BOOLEAN;
		dataArray[5].value.asBoolean = false;
		dataArray[6].id = 7;
		dataArray[6].type = LWM2M_TYPE_FLOAT;
		dataArray[6].value.asFloat = 11.3;
		dataArray[7].id = 8;
		dataArray[7].type = LWM2M_TYPE_OBJECT_LINK;
		dataArray[7].value.asObjLink.objectId = 123;
		dataArray[7].value.asObjLink.objectInstanceId = 456;
		dataArray[8].id = 9;
		dataArray[8].type = LWM2M_TYPE_STRING;
		dataArray[8].value.asBuffer.length = 4;
		dataArray[8].value.asBuffer.buffer = new uint8_t[4];
		memcpy(dataArray[8].value.asBuffer.buffer, "test", 4);
		dataArray[9].id = 10;
		dataArray[9].type = LWM2M_TYPE_CORE_LINK;
		dataArray[9].value.asBuffer.length = 6;
		dataArray[9].value.asBuffer.buffer = new uint8_t[6];
		memcpy(dataArray[9].value.asBuffer.buffer, "</1/0>", 6);

		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE(instance.serverOpWriteReplaceResCnt == 0);
		REQUIRE(instance.serverOpWriteReplaceInstCnt == 0);
		REQUIRE((int)instance.writeAsServer(10, dataArray, LWM2M_WRITE_REPLACE_RESOURCES) == (int)COAP_204_CHANGED);
		REQUIRE(instance.serverOpWriteUpdCnt == 0);
		REQUIRE(instance.serverOpWriteReplaceResCnt == 9);
		REQUIRE(instance.serverOpWriteReplaceInstCnt == 0);
		STRING_T testStr;
		REQUIRE(instance.get(0, testStr));
		TIME_T testTime;
		REQUIRE(instance.get(1, testTime));
		REQUIRE(testTime == 1);
		REQUIRE(instance.get(2, testTime));
		REQUIRE(testTime == 12);
		REQUIRE(instance.get({3, 0}, testStr));
		REQUIRE(testStr == "test");
		REQUIRE_FALSE(instance.get({3, 1}, testStr));
		EXECUTE_T execGet;
		REQUIRE(instance.get(4, execGet));
		UINT_T testUInt;
		REQUIRE(instance.get(5, testUInt));
		REQUIRE(testUInt == 178);
		BOOL_T testBool;
		REQUIRE(instance.get(6, testBool));
		REQUIRE(testBool == false);
		FLOAT_T testFloat;
		REQUIRE(instance.get(7, testFloat));
		REQUIRE((int)(testFloat*10) == (int)113);
		OBJ_LINK_T testObjLink;
		REQUIRE(instance.get(8, testObjLink));
		REQUIRE(testObjLink.objId == 123);
		REQUIRE(testObjLink.objInstId == 456);
		OPAQUE_T testOpaque;
		REQUIRE(instance.get(9, testOpaque));
		REQUIRE(testOpaque.size() == 4);
		REQUIRE(memcmp(testOpaque.data(), "test", 4) == 0);
		REQUIRE(instance.get(10, testStr));
		REQUIRE(testStr == "</1/0>");

		delete dataArray[0].value.asBuffer.buffer;
		delete dataArray[3].value.asChildren.array[0].value.asBuffer.buffer;
		delete dataArray[3].value.asChildren.array;
		delete dataArray[8].value.asBuffer.buffer;
		delete dataArray[9].value.asBuffer.buffer;
		delete dataArray;
	}

	SECTION("executeAsServer") {
		// Resource does not exist
		REQUIRE(instance.serverOpExecuteCnt == 0);
		REQUIRE(instance.executeAsServer(100, NULL, 0) == COAP_404_NOT_FOUND);
		REQUIRE(instance.serverOpExecuteCnt == 0);

		// Resource is not executable
		REQUIRE(instance.serverOpExecuteCnt == 0);
		REQUIRE((int)instance.executeAsServer(0, NULL, 0) == (int)COAP_405_METHOD_NOT_ALLOWED);
		REQUIRE(instance.serverOpExecuteCnt == 0);

		// Resource is executable but not set
		instance.clear(4);
		REQUIRE(instance.set(4, (EXECUTE_T)NULL));
		REQUIRE(instance.serverOpExecuteCnt == 0);
		REQUIRE(instance.executeAsServer(4, NULL, 0) == COAP_404_NOT_FOUND);
		REQUIRE(instance.serverOpExecuteCnt == 0);

		bool executed = false;
		bool returnValue = false;
		EXECUTE_T exec = [&executed, &returnValue](Instance& inst, ID_T id, const OPAQUE_T& data) { 
			executed = true;
			return returnValue; 
		};
		REQUIRE(instance.set(4, exec));

		// Resource is executable and set but execution failed
		REQUIRE_FALSE(returnValue);
		REQUIRE_FALSE(executed);
		REQUIRE(instance.serverOpExecuteCnt == 0);
		REQUIRE(instance.executeAsServer(4, NULL, 0) == COAP_405_METHOD_NOT_ALLOWED);
		REQUIRE(executed);
		REQUIRE(instance.serverOpExecuteCnt == 1);

		// Resource is executable and set and execution succeeded
		returnValue = true;
		executed = false;
		REQUIRE(returnValue);
		REQUIRE_FALSE(executed);
		REQUIRE(instance.serverOpExecuteCnt == 1);
		REQUIRE(instance.executeAsServer(4, NULL, 0) == COAP_204_CHANGED);
		REQUIRE(executed);
		REQUIRE(instance.serverOpExecuteCnt == 2);

		// Check that data is passed to the execute function
		OPAQUE_T dataTest = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
		EXECUTE_T exec2 = [&instance, &dataTest](Instance& inst, ID_T id, const OPAQUE_T& data) { 
			REQUIRE(&instance == &inst);
			REQUIRE(id == 4);
			REQUIRE(dataTest == data);
			return true; 
		};
		REQUIRE(instance.set(4, exec));
		REQUIRE(instance.executeAsServer(4, dataTest.data(), dataTest.size()) == COAP_204_CHANGED);
	}

	SECTION("discoverAsServer") {
		// Incorrect parameters
		REQUIRE(instance.discoverAsServer(NULL, &dataArray) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(instance.discoverAsServer(&numData, NULL) == COAP_500_INTERNAL_SERVER_ERROR);

		// Resource does not exist
		instance.clear(0);
		instance.clear(1);
		instance.clear(2);
		instance.clear(3);
		instance.clear(4);
		instance.clear(5);
		instance.clear(6);
		instance.clear(7);
		instance.clear(8);
		instance.clear(9);
		instance.clear(10);
		REQUIRE(numData == 0);
		REQUIRE(instance.serverOpDiscoverCnt == 0);
		REQUIRE(instance.discoverAsServer(&numData, &dataArray) == COAP_500_INTERNAL_SERVER_ERROR);
		REQUIRE(instance.serverOpDiscoverCnt == 0);
		REQUIRE(numData == 0);
		REQUIRE(dataArray == NULL);
		
		// Creating resources
		REQUIRE(instance.set(0, (STRING_T)"test1"));
		REQUIRE(instance.set(1, (TIME_T)123));
		REQUIRE(instance.set(2, (INT_T)12));
		REQUIRE(instance.set({3, 0}, (STRING_T)"test2"));
		REQUIRE(instance.set({3, 1}, (STRING_T)"test3"));
		EXECUTE_T exec = (EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) { return true; };
		REQUIRE(instance.set(4, exec));
		
		// Discover all resources
		REQUIRE(numData == 0);
		REQUIRE(instance.serverOpDiscoverCnt == 0);
		REQUIRE(instance.discoverAsServer(&numData, &dataArray) == COAP_205_CONTENT);
		REQUIRE(instance.serverOpDiscoverCnt == 6);
		REQUIRE(numData == 5);
		REQUIRE(dataArray != NULL);
		REQUIRE(dataArray[0].id == 0);
		REQUIRE(dataArray[1].id == 1);
		REQUIRE(dataArray[2].id == 2);
		REQUIRE(dataArray[3].id == 3);
		REQUIRE(dataArray[3].type == LWM2M_TYPE_MULTIPLE_RESOURCE);
		REQUIRE(dataArray[3].value.asChildren.count == 2);
		REQUIRE(dataArray[3].value.asChildren.array != NULL);
		REQUIRE(dataArray[3].value.asChildren.array[0].id == 0);
		REQUIRE(dataArray[3].value.asChildren.array[1].id == 1);
		REQUIRE(dataArray[4].id == 4);
		delete dataArray;

		// Discover one defined resources
		dataArray = new lwm2m_data_t[1];
		dataArray[0].id = 0;
		numData = 1;
		REQUIRE(numData == 1);
		REQUIRE(instance.serverOpDiscoverCnt == 6);
		REQUIRE(instance.discoverAsServer(&numData, &dataArray) == COAP_205_CONTENT);
		REQUIRE(instance.serverOpDiscoverCnt == 7);
		REQUIRE(numData == 1);
		delete dataArray;

		// Discover one undefined resources
		dataArray = new lwm2m_data_t[1];
		dataArray[0].id = 100;
		numData = 1;
		REQUIRE(numData == 1);
		REQUIRE(instance.serverOpDiscoverCnt == 7);
		REQUIRE(instance.discoverAsServer(&numData, &dataArray) == COAP_404_NOT_FOUND);
		REQUIRE(instance.serverOpDiscoverCnt == 7);
		REQUIRE(numData == 1);
		delete dataArray;
	}	
}