#include "catch_amalgamated.hpp"
#include "WppTypes.h"

using namespace wpp;

TEST_CASE("types", "[types]") {
  SECTION("Check defines") {
    BOOL_T bool_value = SINGLE_INSTANCE_ID;
    CHECK_FALSE(bool_value);

    INT_T int_value = ID_T_MAX_VAL;
    REQUIRE(LWM2M_MAX_ID == 0xFFFF);
    REQUIRE(int_value == 0xFFFF);
  }

  SECTION("Check dataTypeToID") {
    REQUIRE(dataTypeToID<TYPE_ID>() == TYPE_ID::UNDEFINED);
    REQUIRE(dataTypeToID<BOOL_T>() == TYPE_ID::BOOL);
    REQUIRE(dataTypeToID<INT_T>() == TYPE_ID::INT);
    REQUIRE(dataTypeToID<UINT_T>() == TYPE_ID::UINT);
    REQUIRE(dataTypeToID<FLOAT_T>() == TYPE_ID::FLOAT);
    REQUIRE(dataTypeToID<OPAQUE_T>() == TYPE_ID::OPAQUE);
    REQUIRE(dataTypeToID<OBJ_LINK_T>() == TYPE_ID::OBJ_LINK);
    REQUIRE(dataTypeToID<STRING_T>() == TYPE_ID::STRING);
    REQUIRE(dataTypeToID<EXECUTE_T>() == TYPE_ID::EXECUTE);
  }
}
