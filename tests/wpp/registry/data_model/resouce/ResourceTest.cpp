// #include "catch_amalgamated.hpp"
// #include "Resource.h"
// #include <iostream>

// using namespace wpp;

// namespace wpp {
//   class Instance {
//   public:
//     Instance() {}
//     ~Instance() {}
//   };
// }

// TEST_CASE("Resources", "[resources]")
// {
// #define MIN_SIZE 10
// #define MAX_SIZE 50
// #define INT_MIN_VALUE 1
// #define INT_MAX_VALUE 30
// #define FLOAT_MIN_VALUE 1.1
// #define FLOAT_MAX_VALUE 4.25
// #define STRING_MAX_SIZE 10

//   bool check_value;
//   bool method_call;
//   std::vector<ID_T> ids;

//   enum ID : uint8_t
//   {
//     R_BOOL = 0,      // bool
//     R_INT = 1,       // int64_t
//     R_UINT = 2,      // uint64_t
//     R_FLOAT = 3,     // double
//     R_OBJ_LINK = 4,  // {object ID, instance ID}
//     R_TIME = 5,      // Derived from INT
//     R_OPAQUE = 6,    // vector<uint8_t>
//     R_STRING = 7,    // string
//     R_CORE_LINK = 8, // Derived from STRING
//     R_EXECUTE = 9,   // Type of executable resources
//     R_UNDEFINED = 10 // Undefined type
//   };

//   Resource resources;
//   Resource resources_0 = {R_BOOL, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL};
//   Resource resources_1 = {R_INT, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT};
//   Resource resources_2 = {R_UINT, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::UINT};
//   Resource resources_3 = {R_FLOAT, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT};
//   Resource resources_4 = {R_OBJ_LINK, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OBJ_LINK};
//   Resource resources_5 = {R_TIME, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::TIME};
//   Resource resources_6 = {R_OPAQUE, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE};
//   Resource resources_7 = {R_STRING, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING};
//   Resource resources_8 = {R_CORE_LINK, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::CORE_LINK};
//   Resource resources_9 = {R_EXECUTE, ItemOp(ItemOp::EXECUTE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE};
//   Resource resources_10 = {R_UNDEFINED, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::UNDEFINED};

//   SECTION("default_resources")
//   {
//     REQUIRE(resources.getId() == 65535);
//   }

//   SECTION("bool_resources_0")
//   {
//     ids = {0};
//     BOOL_T valSetMove;
//     // BOOL_T *tmpValue = NULL;

//     check_value = resources_0.setDataVerifier((VERIFY_BOOL_T)[&method_call](const BOOL_T &value) {
//       method_call = true;
//       return value == false;
//     });
//     REQUIRE(check_value);
//     // REQUIRE(resources_0.isEmpty());
//     // REQUIRE(resources_0.instanceCnt() == 0);
//     // REQUIRE_FALSE(resources_0.get(valSetMove));

//     // Initialised instance for resource
//     REQUIRE(resources_0.set(BOOL_T(false)));

//     // REQUIRE(resources_0.get(valSetMove));
//     REQUIRE(valSetMove);
//     valSetMove = true;
//     // BOOL_T valSet = false;
//     // REQUIRE(resources_0.setMove(valSet));
//     // REQUIRE(resources_0.get(valSetMove));
//     REQUIRE(valSetMove);
//     // REQUIRE_FALSE(resources_0.setMove(valSet, 1));
//     // REQUIRE(resources_0.ptr(&tmpValue));

//     REQUIRE(resources_0.isSingle());
//     REQUIRE(resources_0.isMandatory());
//     REQUIRE(resources_0.getId() == R_BOOL);
//     // REQUIRE(resources_0.instanceCnt() == 1);
//     // REQUIRE(resources_0.getInstIds() == ids);
//     REQUIRE(resources_0.getOperation().isRead());
//     // REQUIRE(resources_0.isInstanceExist(0));
//     // REQUIRE(resources_0.isInstanceIdPossible(0));
//     REQUIRE(resources_0.getTypeId() == TYPE_ID::BOOL);
//     // REQUIRE(resources_0.isOperationValid(ItemOp::READ));
//     // REQUIRE(resources_0.isTypeIdCompatible(TYPE_ID::BOOL));

//     REQUIRE_FALSE(resources_0.isDataValueValid(BOOL_T(true)));
//     REQUIRE_FALSE(resources_0.isDataValueValid(BOOL_T(55)));
//     REQUIRE_FALSE(resources_0.isDataValueValid(INT_T(55)));
//     REQUIRE_FALSE(resources_0.isDataValueValid(2));
//     REQUIRE(method_call);
//     // REQUIRE_FALSE(resources_0.isEmpty());
//     REQUIRE_FALSE(resources_0.isMultiple());
//     REQUIRE_FALSE(resources_0.isOptional());
//     REQUIRE_FALSE(resources_0.remove(0));
//     REQUIRE_FALSE(resources_0.set(STRING_T("true")));
//     REQUIRE_FALSE(resources_0.getOperation().isWrite());
//     // REQUIRE_FALSE(resources_0.isInstanceExist(255));

//     // REQUIRE_FALSE(resources_0.isOperationValid(ItemOp::WRITE));
//     // REQUIRE_FALSE(resources_0.isTypeIdCompatible(TYPE_ID::INT));

//     REQUIRE(!resources_0.clear());
//     // REQUIRE_FALSE(resources_0.isInstanceExist(0));
//     // REQUIRE(resources_0.instanceCnt() == 0);
//   };

//   SECTION("int_resources_1")
//   {
//     INT_T valSet = 25;
//     INT_T valSetMove = 0;
//     // INT_T *tmpValue = NULL;

//     check_value = resources_1.setDataVerifier((VERIFY_INT_T)[&method_call](const INT_T &value) {
//       method_call = true;
//       return INT_MIN_VALUE < value && value < INT_MAX_VALUE;
//     });
//     REQUIRE(check_value);
    
//     // Initialised instance for resource
//     REQUIRE(resources_1.set(INT_T(valSet)));

//     // REQUIRE(resources_1.get(valSetMove));
//     REQUIRE(! (valSetMove == valSet));
//     valSetMove = 0;
//     // INT_T valSet2 = valSet + 1;
//     // REQUIRE(resources_1.setMove(valSet2));

//     // REQUIRE(resources_1.get(valSetMove));
//     // REQUIRE(valSetMove == valSet + 1);

//     // REQUIRE(resources_1.ptr(&tmpValue));
//     // REQUIRE(*tmpValue == valSet + 1);

//     REQUIRE(resources_1.getId() == R_INT);
//     REQUIRE(resources_1.isDataValueValid(INT_T(25)));
//     REQUIRE_FALSE(resources_1.isDataValueValid(INT_T(30)));
//     REQUIRE_FALSE(resources_1.set(INT_T(25), 1));
//     REQUIRE_FALSE(resources_1.set(STRING_T("true")));

//     REQUIRE(method_call);
//   };

//   SECTION("uint_resources_2")
//   {
//     UINT_T valSet = 25;
//     UINT_T valSetMove = 0;
//     // UINT_T *tmpValue = NULL;

//     check_value = resources_2.setDataVerifier((VERIFY_UINT_T)[&method_call](const UINT_T &value) {
//       method_call = true;
//       return INT_MIN_VALUE < value && value < INT_MAX_VALUE;
//     });
//     REQUIRE(check_value);

//     // Initialised instance for resource
//     REQUIRE(resources_2.set(UINT_T(valSet)));

//     // REQUIRE(resources_2.get(valSetMove));
//     REQUIRE(valSetMove == valSet);
//     valSetMove = 0;
//     // UINT_T valSet2 = valSet + 1;
//     // REQUIRE(resources_2.setMove(valSet2));
//     // REQUIRE(resources_2.get(valSetMove));
//     REQUIRE(valSetMove == valSet + 1);

//     // REQUIRE(resources_2.ptr(&tmpValue));

//     REQUIRE(resources_2.getId() == R_UINT);
//     REQUIRE(resources_2.isDataValueValid(UINT_T(25)));
//     REQUIRE_FALSE(resources_2.isDataValueValid(UINT_T(30)));
//     REQUIRE_FALSE(resources_2.set(FLOAT_T(31.33)));
//     REQUIRE_FALSE(resources_2.set(UINT_T(31)));

//     REQUIRE(method_call);
//   };

//   SECTION("float_resources_3")
//   {
//     FLOAT_T valSet = 2.51;
//     FLOAT_T valSetMove = 0;
//     // FLOAT_T *tmpValue = NULL;

//     check_value = resources_3.setDataVerifier((VERIFY_FLOAT_T)[&method_call](const FLOAT_T &value) {
//       method_call = true;
//       return FLOAT_MIN_VALUE < value && value < FLOAT_MAX_VALUE;
//     });
//     REQUIRE(check_value);
    
//     // Initialised instance for resource
//     REQUIRE(resources_3.set(FLOAT_T(valSet)));

//     // REQUIRE(resources_3.get(valSetMove));
//     REQUIRE(valSetMove > 2.5);
//     // valSetMove = 0;
//     // FLOAT_T valSet2 = valSet + 1.132;
//     // REQUIRE(resources_3.setMove(valSet2));
//     // REQUIRE(resources_3.get(valSetMove));
//     REQUIRE(valSetMove > 3.64);

//     // REQUIRE(resources_3.ptr(&tmpValue));

//     REQUIRE(resources_3.getOperation().isWrite());
//     REQUIRE(resources_3.getId() == R_FLOAT);
//     REQUIRE_FALSE(resources_3.set(FLOAT_T(4.53)));
//     REQUIRE_FALSE(resources_3.set(UINT_T(25)));

//     REQUIRE(method_call);
//   };

//   SECTION("obj_link_resources_4")
//   {
//     OBJ_LINK_T valSet = {11, 9};
//     // OBJ_LINK_T valSet2 = {34, 8};
//     OBJ_LINK_T valSetMove = {0, 0};
//     // OBJ_LINK_T *tmpValue = NULL;

//     check_value = resources_4.setDataVerifier((VERIFY_OBJ_LINK_T)[&method_call](const OBJ_LINK_T &value) {
//       method_call = true;
//       return value.objId < MAX_SIZE && value.objInstId < MIN_SIZE;
//     });
//     REQUIRE(check_value);

//     // Initialised instance for resource
//     REQUIRE(resources_4.set(OBJ_LINK_T(valSet)));

//     // REQUIRE(resources_4.get(valSetMove));
//     REQUIRE(valSetMove.objId == 11);
//     REQUIRE(valSetMove.objInstId == 9);

//     // OBJ_LINK_T valSet3 = valSet2;
//     // REQUIRE(resources_4.setMove(valSet3));
//     // REQUIRE(resources_4.get(valSetMove));
//     REQUIRE(valSetMove.objId == 34);
//     REQUIRE(valSetMove.objInstId == 8);

//     // REQUIRE(resources_4.ptr(&tmpValue));

//     REQUIRE(resources_4.getId() == R_OBJ_LINK);
//     REQUIRE_FALSE(resources_4.set(UINT_T(25)));
//     REQUIRE_FALSE(resources_4.set(OBJ_LINK_T({106, 2})));

//     REQUIRE(method_call);
//   };

//   SECTION("time_resources_5")
//   {
//     TIME_T valSet = 15121996;
//     TIME_T valSetMove = 0;
//     // TIME_T *tmpValue = NULL;

//     // Initialised instance for resource
//     REQUIRE(resources_5.set(TIME_T(valSet)));

//     // REQUIRE(resources_5.get(valSetMove));
//     REQUIRE(valSetMove == valSet);
//     valSetMove = 0;
//     // TIME_T valSet2 = valSet + 27;
//     // REQUIRE(resources_5.setMove(valSet2));
//     // REQUIRE(resources_5.get(valSetMove));
//     REQUIRE(valSetMove == valSet + 27);

//     // REQUIRE(resources_5.ptr(&tmpValue));

//     REQUIRE(resources_5.set(INT_T(25)));
//     REQUIRE(resources_5.getId() == R_TIME);
//     REQUIRE_FALSE(resources_5.set(OBJ_LINK_T()));
//     REQUIRE_FALSE(resources_5.set(UINT_T(25)));
//   };

//   SECTION("opaque_resources_6")
//   {
//     OPAQUE_T valSet = {11, 9, 24, 31};
//     OPAQUE_T valSet2 = {34, 8, 9, 24, 31, 67};
//     OPAQUE_T valSetCheck = {34, 8, 9, 24, 31, 67};
//     OPAQUE_T valSetMove = {0, 0};
//     OPAQUE_T *tmpValue = NULL;

//     check_value = resources_6.setDataVerifier((VERIFY_OPAQUE_T)[&method_call](const OPAQUE_T &value) {
//       method_call = true;
//       return 1 <= value.size() && value.size() <= 150;
//     });
//     REQUIRE(check_value);

//     // Initialised instance for resource
//     REQUIRE(resources_6.set(OPAQUE_T(valSet)));

//     // REQUIRE(resources_6.get(valSetMove));
//     REQUIRE(valSetMove == valSet);
//     OPAQUE_T valSet3 = valSet2;
//     // REQUIRE(resources_6.setMove(valSet3));
//     // REQUIRE(resources_6.get(valSetMove));
//     REQUIRE(valSetMove == valSet2);

//     // REQUIRE(resources_6.ptr(&tmpValue));
//     REQUIRE(valSetCheck == *tmpValue);

//     REQUIRE(resources_6.getId() == R_OPAQUE);
//     REQUIRE_FALSE(resources_6.set(INT_T(25)));
//     REQUIRE_FALSE(resources_6.set(OPAQUE_T(0)));
//     REQUIRE_FALSE(resources_6.set(OPAQUE_T(151)));

//     REQUIRE(method_call);
//   };

//   SECTION("string_resources_7")
//   {
//     check_value = resources_7.setDataVerifier((VERIFY_STRING_T)[&method_call](const STRING_T &value) {
//       method_call = true;
//       return value.size() < STRING_MAX_SIZE;
//     });
//     REQUIRE(check_value);

//     ids = {0, 1, 2, 43, 51};

//     // Initialised instance for resource
//     REQUIRE(resources_7.set(STRING_T("Hello"), ids[0]));
//     REQUIRE(resources_7.set(STRING_T("World"), ids[1]));
//     REQUIRE(resources_7.set(STRING_T("My"), ids[3]));
//     REQUIRE(resources_7.set(STRING_T("instances"), ids[4]));

//     // REQUIRE_FALSE(resources_7.ptr((STRING_T**)NULL));

//     std::vector<ID_T> ids_2 = {0, 1, 43, 51};
//     // REQUIRE(resources_7.instanceCnt() == 4);
//     // REQUIRE(resources_7.getInstIds() == ids_2);
//     REQUIRE(resources_7.getId() == R_STRING);

//     std::vector<ID_T> ids_3 = {0, 1, 51};
//     REQUIRE(resources_7.isMultiple());
//     REQUIRE(resources_7.remove(ids[3]));
//     // REQUIRE(resources_7.instanceCnt() == 3);
//     // REQUIRE(resources_7.getInstIds() == ids_3);

//     REQUIRE_FALSE(resources_7.isOptional());
//     REQUIRE_FALSE(resources_7.set(OBJ_LINK_T()));
//     REQUIRE_FALSE(resources_7.set(STRING_T("This instances very long"), ids[2]));

//     REQUIRE(resources_7.clear());
//     // REQUIRE(resources_7.instanceCnt() == 0);

//     REQUIRE(method_call);
//   };

//   SECTION("core_link_resources_8")
//   {
//     check_value = resources_8.setDataVerifier((VERIFY_CORE_LINK_T)[&method_call](const CORE_LINK_T &value) {
//       method_call = true;
//       return true;
//     });
//     REQUIRE(check_value);

//     REQUIRE(resources_8.set(STRING_T("true")));
//     REQUIRE(resources_8.set(CORE_LINK_T("true")));
//     REQUIRE(resources_8.set(CORE_LINK_T()));
//     REQUIRE(resources_8.getId() == R_CORE_LINK);
//     REQUIRE(resources_8.isDataValueValid(CORE_LINK_T("true")));

//     REQUIRE(method_call);
//     REQUIRE_FALSE(resources_8.set(OBJ_LINK_T()));
//     // REQUIRE_FALSE(resources_8.isTypeIdCompatible(TYPE_ID::BOOL));
//   };

//   SECTION("execute_resources_9")
//   {
//     check_value = resources_9.setDataVerifier((VERIFY_EXECUTE_T)[&method_call](const EXECUTE_T &value) {
//       method_call = true;
//       return 1;
//     });
//     REQUIRE(check_value);

//     Instance inst;
//     int execute_call = 0;
//     EXECUTE_T exe = [&execute_call](Instance &inst, ID_T id, const OPAQUE_T& data) {
//       execute_call++;
//       return true;
//     };
//     exe(inst, 0, OPAQUE_T());
//     REQUIRE(execute_call == 1);

//     // Initialised instance for resource
//     REQUIRE(resources_9.set(exe));
//     EXECUTE_T exeCheck;
//     // REQUIRE(resources_9.get(exeCheck));
//     REQUIRE(exeCheck(inst, 0, OPAQUE_T()));
//     REQUIRE(execute_call == 2);

//     REQUIRE(resources_9.getId() == R_EXECUTE);
//     REQUIRE_FALSE(resources_9.set(CORE_LINK_T()));

//     REQUIRE(method_call);
//     REQUIRE(check_value);
//   };

//   SECTION("undefined_resources_10")
//   {
//     REQUIRE_FALSE(resources_10.set(INT_T(25)));
//     REQUIRE_FALSE(resources_10.set(EXECUTE_T()));
//     REQUIRE_FALSE(resources_10.set(STRING_T("true")));

//     REQUIRE(resources_10.getId() == R_UNDEFINED);
//   };

//   SECTION("setMove")
//   {
//     // TODO compare vectors address for setMove
//     // BOOL_T *tmpValue = NULL;
//     check_value = false;
//     // BOOL_T valBool = check_value;
//     // REQUIRE(resources_0.setMove(valBool));
//     // REQUIRE_FALSE(resources_0.setMove(valBool, 1));

//     check_value = true;
//     // REQUIRE(resources_0.get(check_value));
//     // REQUIRE_FALSE(resources_0.get(check_value, 1));
//     REQUIRE(check_value == false);
//     // REQUIRE(resources_0.ptr(&tmpValue));
//     // REQUIRE_FALSE(resources_0.ptr(&tmpValue, 1));

//     INT_T valInt = 111;
//     // INT_T *tmpValueInt = NULL;
//     // INT_T valInt2 = valInt;
//     // REQUIRE(resources_1.setMove(valInt2));

//     valInt = 0;
//     // resources_1.get(valInt);
//     REQUIRE(valInt == 111);
//     // REQUIRE(resources_1.ptr(&tmpValueInt));

//     UINT_T valUint = 111;
//     // UINT_T *tmpValueUint = NULL;
//     // UINT_T valUint2 = valUint;
//     // REQUIRE(resources_2.setMove(valUint2));

//     valUint = 0;
//     // resources_2.get(valUint);
//     REQUIRE(valUint == 111);
//     // REQUIRE(resources_2.ptr(&tmpValueUint));

//     FLOAT_T valFloat = 1.11;
//     // FLOAT_T *tmpValueFloat = NULL;
//     // FLOAT_T valFloat2 = valFloat;
//     // REQUIRE(resources_3.setMove(valFloat2));

//     valFloat = 0;
//     // resources_3.get(valFloat);
//     REQUIRE(valFloat < 1.12);
//     REQUIRE(valFloat > 1.10);
//     // REQUIRE(resources_3.ptr(&tmpValueFloat));

//     OBJ_LINK_T valObjLink = {1, 1};
//     // OBJ_LINK_T *tmpValueObjLink = NULL;
//     // OBJ_LINK_T valObjLink2 = valObjLink;
//     // REQUIRE(resources_4.setMove(valObjLink2));

//     valObjLink = {0, 0};
//     // REQUIRE(resources_4.get(valObjLink));
//     REQUIRE(valObjLink.objId == 1);
//     REQUIRE(valObjLink.objInstId == 1);
//     // REQUIRE(resources_4.ptr(&tmpValueObjLink));

//     TIME_T valTime = 15122023;
//     // TIME_T *tmpValueTime = NULL;
//     // TIME_T valTime2 = valTime;
//     // REQUIRE(resources_5.setMove(valTime2));

//     valTime = 0;
//     // resources_5.get(valTime);
//     REQUIRE(valTime == 15122023);
//     // REQUIRE(resources_5.ptr(&tmpValueTime));

//     OPAQUE_T valOpaque = {12, 21, 22, 34, 43, 45, 65, 76, 96, 240, 190, 157, 12, 21, 22, 34, 43, 45, 65, 76, 96, 240, 190, 157, 12, 21, 22, 34, 43, 45, 65, 76, 96, 240, 190, 157, 12, 21, 22, 34, 43, 45, 65, 76, 96, 240, 190, 157};
//     OPAQUE_T *tmpValueOpaque = NULL;
//     uint8_t *p1 = valOpaque.data();
//     // REQUIRE(resources_6.setMove(valOpaque));
//     // REQUIRE(resources_6.ptr(&tmpValueOpaque));
//     uint8_t *p2 = tmpValueOpaque->data();
//     REQUIRE(p1 == p2);

//     valOpaque = {0, 0};
//     // REQUIRE(resources_6.get(valOpaque));
//     REQUIRE(valOpaque[0] == 12);
//     REQUIRE(valOpaque[1] == 21);

//     STRING_T valString = "Hello, world!";
//     // STRING_T *tmpValueString = NULL;
//     STRING_T valString2 = valString;
//     // REQUIRE(resources_7.setMove(valString2));
//     valString = "??";
//     // resources_7.get(valString);
//     REQUIRE(valString == "Hello, world!");
//     // REQUIRE(resources_7.ptr(&tmpValueString));

//     CORE_LINK_T valCoreLink = "Hello, Core Link";
//     // CORE_LINK_T *tmpValueCoreLink = NULL;
//     // REQUIRE(resources_8.setMove(valCoreLink));
//     valCoreLink = "??";
//     // resources_8.get(valCoreLink);
//     REQUIRE(valCoreLink == "Hello, Core Link");
//     // REQUIRE(resources_8.ptr(&tmpValueCoreLink));

//     EXECUTE_T valExecute;
//     // EXECUTE_T *tmpValueExecute = NULL;
//     EXECUTE_T valExecute2 = valExecute;
//     // REQUIRE(resources_9.setMove(valExecute2));
//     // REQUIRE(resources_9.get(valExecute));
//     // REQUIRE(resources_9.ptr(&tmpValueExecute));

//     // REQUIRE_FALSE(resources_10.setMove(check_value));
//     // REQUIRE_FALSE(resources_10.get(check_value));

//     // REQUIRE_FALSE(resources_10.ptr(&tmpValue));
//   };

//   SECTION("resources_copy")
//   {
//     BOOL_T valBoolCheck;
//     // BOOL_T valBool = false;
//     // REQUIRE(resources_0.setMove(valBool));

//     resources_1 = resources_0;

//     // Assign the resource to itself
//     // resources_0 = resources_0;

//     // Chain assignment operators
//     resources_2 = resources_1 = resources_0;
//     REQUIRE(resources_0.getId() == resources_1.getId());
//     REQUIRE(resources_1.getId() == resources_2.getId());

//     // REQUIRE_FALSE(resources_1.isEmpty());

//     resources_2 = std::move(resources_1);
//     // INT_T *tmpValueExecute;
//     // REQUIRE_FALSE(resources_1.ptr(&tmpValueExecute));
//     // REQUIRE(resources_1.isEmpty());

//     Resource resources_11(resources_2);

//     // REQUIRE(resources_11.get(valBoolCheck));
//     REQUIRE(valBoolCheck == false);

//     Resource resources_12(std::move(resources_11));
//     // REQUIRE(resources_11.isEmpty());

//     // REQUIRE(resources_12.get(check_value));
//     REQUIRE_FALSE(check_value);
//   };
//   SECTION("multiple_resources_copy")
//   {
//     STRING_T valString1 = "Hello, first instance!";
//     STRING_T valString2 = "Hello, second instance!";
//     STRING_T valString3 = "Hello, third instance!";
//     STRING_T valString4 = "Hello, fourth instance!";

//     REQUIRE(resources_7.set(valString1, 0));
//     REQUIRE(resources_7.set(valString2, 1));
//     REQUIRE(resources_7.set(valString3, 2));
//     REQUIRE(resources_7.set(valString4, 3));
//     // REQUIRE(resources_7.instanceCnt() == 4);

//     resources_8 = resources_7;

//     STRING_T valStringCheck;
//     // REQUIRE(resources_8.get(valStringCheck, 0));
//     REQUIRE(valStringCheck == valString1);
//     // REQUIRE(resources_8.get(valStringCheck, 1));
//     REQUIRE(valStringCheck == valString2);
//     // REQUIRE(resources_8.get(valStringCheck, 2));
//     REQUIRE(valStringCheck == valString3);
//     // REQUIRE(resources_8.get(valStringCheck, 3));
//     REQUIRE(valStringCheck == valString4);
//   };
// }
