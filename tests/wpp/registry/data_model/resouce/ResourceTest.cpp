#include "catch_amalgamated.hpp"
#include "Resource.h"
#include <iostream>

using namespace wpp;

namespace wpp
{
    class Instance
    {
    public:
        Instance() {}
        ~Instance() {}
    };
}

TEST_CASE("Resources", "[resources]")
{
#define MIN_SIZE 10
#define MAX_SIZE 50
#define INT_MIN_VALUE 1
#define INT_MAX_VALUE 30
#define FLOAT_MIN_VALUE 1.1
#define FLOAT_MAX_VALUE 4.25
#define STRING_MAX_SIZE 10

    bool check_value;
    bool method_call;
    std::vector<ID_T> ids;

    enum ID : uint8_t
    {
        R_BOOL = 0,      // bool
        R_INT = 1,       // int64_t
        R_UINT = 2,      // uint64_t
        R_FLOAT = 3,     // double
        R_OBJ_LINK = 4,  // {object ID, instance ID}
        R_TIME = 5,      // Derived from INT
        R_OPAQUE = 6,    // vector<uint8_t>
        R_STRING = 7,    // string
        R_CORE_LINK = 8, // Derived from STRING
        R_EXECUTE = 9,   // Type of executable resources
        R_UNDEFINED = 10 // Undefined type
    };

    Resource resources;
    Resource resources_0 = {R_BOOL, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL};
    Resource resources_1 = {R_INT, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT};
    Resource resources_2 = {R_UINT, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::UINT};
    Resource resources_3 = {R_FLOAT, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT};
    Resource resources_4 = {R_OBJ_LINK, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OBJ_LINK};
    Resource resources_5 = {R_TIME, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::TIME};
    Resource resources_6 = {R_OPAQUE, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE};
    Resource resources_7 = {R_STRING, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY, TYPE_ID::STRING};
    Resource resources_8 = {R_CORE_LINK, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::CORE_LINK};
    Resource resources_9 = {R_EXECUTE, ItemOp(ItemOp::EXECUTE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE};
    Resource resources_10 = {R_UNDEFINED, ItemOp(ItemOp::READ | ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL, TYPE_ID::UNDEFINED};

    SECTION("default_resources")
    {
        REQUIRE(resources.getId() == 65535);
    }

    SECTION("bool_resources_0")
    {
        ids = {};

        check_value = resources_0.setDataVerifier((VERIFY_BOOL_T)[&method_call](const BOOL_T &value) {
            method_call = true;
            return value == false;
        });
        REQUIRE(check_value);
        REQUIRE(resources_0.instCount() == 0);
        REQUIRE_FALSE(resources_0.get<BOOL_T>(1));

        // Initialised instance for resource
        REQUIRE_FALSE(resources_0.set(true, 0));
        REQUIRE_FALSE(resources_0.get<BOOL_T>(0));

        REQUIRE(resources_0.isSingle());
        REQUIRE(resources_0.isMandatory());
        REQUIRE(resources_0.getId() == R_BOOL);
        REQUIRE(resources_0.instIds() == ids); // ?? {0}
        REQUIRE(resources_0.getOperation().isRead());
        REQUIRE(resources_0.isExist(0) == false);
        // REQUIRE(resources_0.isInstanceIdPossible(0)); // priv
        REQUIRE(resources_0.getTypeId() == TYPE_ID::BOOL);
        // REQUIRE(resources_0.isTypeIdCompatible(TYPE_ID::BOOL)); // priv

        REQUIRE_FALSE(resources_0.isDataValueValid(BOOL_T(true)));
        REQUIRE_FALSE(resources_0.isDataValueValid(BOOL_T(55)));
        REQUIRE_FALSE(resources_0.isDataValueValid(INT_T(55)));
        REQUIRE_FALSE(resources_0.isDataValueValid(2));
        REQUIRE(method_call);
        REQUIRE_FALSE(resources_0.isMultiple());
        REQUIRE_FALSE(resources_0.isOptional());
        REQUIRE_FALSE(resources_0.remove(0));
        REQUIRE_FALSE(resources_0.set(STRING_T("true")));
        REQUIRE_FALSE(resources_0.getOperation().isWrite());
        REQUIRE_FALSE(resources_0.isExist(255));

        // REQUIRE_FALSE(resources_0.isTypeIdCompatible(TYPE_ID::INT));  // priv

        REQUIRE_FALSE(resources_0.clear()); // isSingle
        REQUIRE_FALSE(resources_0.isExist(0));
    };

    SECTION("int_resources_1")
    {
        INT_T valSet = 25;
        INT_T valSetMove = 0;

        check_value = resources_1.setDataVerifier((VERIFY_INT_T)[&method_call](const INT_T &value) {
            method_call = true;
            return INT_MIN_VALUE < value && value < INT_MAX_VALUE;
        });
        REQUIRE(check_value);

        // Initialised instance for resource
        REQUIRE(resources_1.set(INT_T(valSet)));
        REQUIRE_FALSE(resources_1.set("31"));

        // REQUIRE(resources_1.get(valSetMove));
        REQUIRE(!(valSetMove == valSet));
        valSetMove = 0;
        // INT_T valSet2 = valSet + 1;
        // REQUIRE(resources_1.setMove(valSet2));

        // REQUIRE(resources_1.get(valSetMove));
        // REQUIRE(valSetMove == valSet + 1);

        // REQUIRE(resources_1.ptr(&tmpValue));
        // REQUIRE(*tmpValue == valSet + 1);

        REQUIRE(resources_1.getId() == R_INT);
        REQUIRE(resources_1.isDataValueValid(INT_T(25)));
        REQUIRE_FALSE(resources_1.isDataValueValid(INT_T(30)));
        REQUIRE(resources_1.set(INT_T(25), 1)); // REQUIRE_FALSE(resources_1.set(INT_T(25), 1));
        REQUIRE_FALSE(resources_1.set(STRING_T("true")));

        REQUIRE(method_call);
    };

    SECTION("uint_resources_2")
    {
        UINT_T valSet = 25;

        check_value = resources_2.setDataVerifier((VERIFY_UINT_T)[&method_call](const UINT_T &value) {
            method_call = true;
            return INT_MIN_VALUE < value && value < INT_MAX_VALUE;
        });
        REQUIRE(check_value);

        // Initialised instance for resource
        REQUIRE(resources_2.set(valSet));

        REQUIRE(resources_2.get<UINT_T>(0) == valSet);
        REQUIRE(resources_2.set(valSet + 1));
        REQUIRE(resources_2.get<UINT_T>(0) == valSet + 1);

        REQUIRE(resources_2.getId() == R_UINT);
        REQUIRE(resources_2.isDataValueValid(UINT_T(25)));
        REQUIRE_FALSE(resources_2.isDataValueValid(UINT_T(30)));
        REQUIRE_FALSE(resources_2.set(FLOAT_T(31.33)));
        REQUIRE_FALSE(resources_2.set(UINT_T(31)));

        REQUIRE(method_call);
    };

    SECTION("float_resources_3")
    {
        FLOAT_T valSet = 2.51;

        check_value = resources_3.setDataVerifier((VERIFY_FLOAT_T)[&method_call](const FLOAT_T &value) {
            method_call = true;
            return FLOAT_MIN_VALUE < value && value < FLOAT_MAX_VALUE;
        });
        REQUIRE(check_value);

        // Initialised instance for resource
        REQUIRE(resources_3.set(valSet));

        REQUIRE(resources_3.get<FLOAT_T>(0) > 2.5);
        REQUIRE(resources_3.set(valSet + 1.132, 1));
        REQUIRE(resources_3.get<FLOAT_T>(1) > 3.64);

        REQUIRE(resources_3.newInstId() == 1);

        REQUIRE(resources_3.getOperation().isWrite());
        REQUIRE(resources_3.getId() == R_FLOAT);
        REQUIRE_FALSE(resources_3.set(FLOAT_T(4.53)));
        REQUIRE_FALSE(resources_3.set(UINT_T(25)));

        REQUIRE(method_call);
    };

    SECTION("obj_link_resources_4")
    {
        OBJ_LINK_T valSet = {11, 9};
        OBJ_LINK_T valSet2 = {34, 8};

        check_value = resources_4.setDataVerifier((VERIFY_OBJ_LINK_T)[&method_call](const OBJ_LINK_T &value) {
            method_call = true;
            return value.objId < MAX_SIZE && value.objInstId < MIN_SIZE;
        });
        REQUIRE(check_value);

        // Initialised instance for resource
        REQUIRE(resources_4.set(valSet));

        REQUIRE(resources_4.get<OBJ_LINK_T>(0).objId == 11);
        REQUIRE(resources_4.get<OBJ_LINK_T>(0).objInstId == 9);

        REQUIRE(resources_4.newInstId() == 1);

        REQUIRE(resources_4.set(valSet2, 1));
        REQUIRE(resources_4.set(valSet2, 2));
        REQUIRE(resources_4.get<OBJ_LINK_T>(2).objId == 34);
        REQUIRE(resources_4.get<OBJ_LINK_T>(2).objInstId == 8);

        REQUIRE(resources_4.getId() == R_OBJ_LINK);
        REQUIRE_FALSE(resources_4.set(UINT_T(25)));
        REQUIRE_FALSE(resources_4.set(OBJ_LINK_T({106, 2})));

        REQUIRE(method_call);
    };

    SECTION("time_resources_5")
    {
        TIME_T valSet = 15121996;

        // Initialised instance for resource
        REQUIRE(resources_5.set(TIME_T(valSet)));
        REQUIRE(resources_5.get<TIME_T>(0) == valSet);

        REQUIRE(resources_5.set(TIME_T(valSet + 27)));
        REQUIRE(resources_5.get<TIME_T>(0) == valSet + 27);

        REQUIRE(resources_5.set(INT_T(25)));
        REQUIRE(resources_5.getId() == R_TIME);
        REQUIRE_FALSE(resources_5.set(OBJ_LINK_T()));
        REQUIRE_FALSE(resources_5.set(UINT_T(25)));
    };

    SECTION("opaque_resources_6")
    {
        OPAQUE_T valSet = {11, 9, 24, 31};
        OPAQUE_T valSet2 = {34, 8, 9, 24, 31, 67};

        check_value = resources_6.setDataVerifier((VERIFY_OPAQUE_T)[&method_call](const OPAQUE_T &value) {
            method_call = true;
            return 1 <= value.size() && value.size() <= 150;
        });
        REQUIRE(check_value);

        // Initialised instance for resource
        REQUIRE(resources_6.set(OPAQUE_T(valSet)));
        REQUIRE(resources_6.get<OPAQUE_T>(0) == valSet);

        REQUIRE(resources_6.set(OPAQUE_T(valSet2)));
        REQUIRE(resources_6.get<OPAQUE_T>(0) == valSet2);

        REQUIRE(resources_6.getId() == R_OPAQUE);
        REQUIRE_FALSE(resources_6.set(INT_T(25)));
        REQUIRE_FALSE(resources_6.set(OPAQUE_T(0)));
        REQUIRE_FALSE(resources_6.set(OPAQUE_T(151)));

        REQUIRE(method_call);
    };

    SECTION("string_resources_7")
    {
        check_value = resources_7.setDataVerifier((VERIFY_STRING_T)[&method_call](const STRING_T &value) {
            method_call = true;
            return value.size() < STRING_MAX_SIZE;
        });
        REQUIRE(check_value);

        ids = {0, 1, 2, 5, 4};

        // Initialised instance for resource
        REQUIRE(resources_7.set(STRING_T("Hello"), ids[0]));
        REQUIRE(resources_7.set(STRING_T("World"), ids[1]));
        REQUIRE(resources_7.set(STRING_T("My"), ids[3]));
        REQUIRE(resources_7.set(STRING_T("instances"), ids[4]));

        std::vector<ID_T> ids_2 = {0, 1, 5, 4};
        REQUIRE(resources_7.instCount() == 4);
        REQUIRE(resources_7.instIds() == ids_2);

        REQUIRE(resources_7.getId() == R_STRING);

        REQUIRE(resources_7.newInstId() == 2);

        std::vector<ID_T> ids_3 = {0, 1, 4};
        REQUIRE(resources_7.isMultiple());
        REQUIRE(resources_7.remove(ids[3]));
        REQUIRE(resources_7.instCount() == 3);
        REQUIRE(resources_7.instIds() == ids_3);

        REQUIRE_FALSE(resources_7.isOptional());
        REQUIRE_FALSE(resources_7.set(OBJ_LINK_T()));
        REQUIRE_FALSE(resources_7.set(STRING_T("This instances very long"), ids[2]));

        REQUIRE(resources_7.clear());
        REQUIRE(resources_7.instCount() == 0);

        REQUIRE(method_call);
    };

    SECTION("core_link_resources_8")
    {
        check_value = resources_8.setDataVerifier((VERIFY_CORE_LINK_T)[&method_call](const CORE_LINK_T &value) {
            method_call = true;
            return true;
        });
        REQUIRE(check_value);

        REQUIRE(resources_8.set(STRING_T("true")));
        REQUIRE(resources_8.set(CORE_LINK_T("true")));
        REQUIRE(resources_8.set(CORE_LINK_T()));
        REQUIRE(resources_8.getId() == R_CORE_LINK);
        REQUIRE(resources_8.isDataValueValid(CORE_LINK_T("true")));

        REQUIRE(method_call);
        REQUIRE_FALSE(resources_8.set(OBJ_LINK_T()));
        // REQUIRE_FALSE(resources_8.isTypeIdCompatible(TYPE_ID::BOOL));
    };

    SECTION("execute_resources_9")
    {
        check_value = resources_9.setDataVerifier((VERIFY_EXECUTE_T)[&method_call](const EXECUTE_T &value) {
            method_call = true;
            return 1;
        });
        REQUIRE(check_value);

        Instance inst;
        int execute_call = 0;
        EXECUTE_T exe = [&execute_call](Instance &inst, ID_T id, const OPAQUE_T &data)
        {
            execute_call++;
            return true;
        };
        exe(inst, 0, OPAQUE_T());
        REQUIRE(execute_call == 1);

        // Initialised instance for resource
        REQUIRE(resources_9.set(exe));
        EXECUTE_T exeCheck = [&execute_call](Instance &inst, ID_T id, const OPAQUE_T &data)
        {
            execute_call++;
            return true;
        };
        REQUIRE(resources_9.get<EXECUTE_T>(0));
        REQUIRE(exeCheck(inst, 0, OPAQUE_T()));
        REQUIRE(execute_call == 2);

        REQUIRE(resources_9.getId() == R_EXECUTE);
        REQUIRE_FALSE(resources_9.set(CORE_LINK_T()));

        REQUIRE(method_call);
        REQUIRE(check_value);
    };

    SECTION("undefined_resources_10")
    {
        REQUIRE_FALSE(resources_10.set(INT_T(25)));
        REQUIRE_FALSE(resources_10.set(EXECUTE_T()));
        REQUIRE_FALSE(resources_10.set(STRING_T("true")));

        REQUIRE(resources_10.getId() == R_UNDEFINED);
    };

    SECTION("resources_copy")
    {
        REQUIRE(resources_0.set(true));
        resources_1 = resources_0;

        // Chain assignment operators
        resources_2 = resources_1 = resources_0;
        REQUIRE(resources_0.getId() == resources_1.getId());
        REQUIRE(resources_1.getId() == resources_2.getId());
        resources_2 = std::move(resources_1);

        Resource resources_11(resources_2);
        REQUIRE(resources_11.get<UINT_T>(0) == 0);

        Resource resources_12(std::move(resources_11));
        REQUIRE(resources_12.get<UINT_T>(0) == 0);
    };
    SECTION("multiple_resources_copy")
    {
        STRING_T valString1 = "Hello, first instance!";
        STRING_T valString2 = "Hello, second instance!";
        STRING_T valString3 = "Hello, third instance!";
        STRING_T valString4 = "Hello, fourth instance!";

        REQUIRE(resources_7.set(valString1, 0));
        REQUIRE(resources_7.set(valString2, 1));
        REQUIRE(resources_7.set(valString3, 2));
        REQUIRE(resources_7.set(valString4, 3));
        REQUIRE(resources_7.instCount() == 4);

        resources_8 = resources_7;

        REQUIRE(resources_8.get<CORE_LINK_T>(0) == "Hello, first instance!");
        REQUIRE(resources_8.get<CORE_LINK_T>(1) == "Hello, second instance!");
        REQUIRE(resources_8.get<CORE_LINK_T>(2) == "Hello, third instance!");
        REQUIRE(resources_8.get<CORE_LINK_T>(3) == "Hello, fourth instance!");
    };

    SECTION("newInstIds")
    {
        REQUIRE(resources_0.newInstId() == 0);
        REQUIRE(resources_1.newInstId() == 0);
        REQUIRE(resources_2.newInstId() == 0);
        REQUIRE(resources_3.newInstId() == 0);
        REQUIRE(resources_4.newInstId() == 0);
        REQUIRE(resources_5.newInstId() == 0);
        REQUIRE(resources_6.newInstId() == 0);
        REQUIRE(resources_7.newInstId() == 0);
        REQUIRE(resources_8.newInstId() == 0);
        REQUIRE(resources_9.newInstId() == 0);
        REQUIRE(resources_10.newInstId() == 0);
    }
}
