#include "catch_amalgamated.hpp"
#include "ItemOp.h"

using namespace wpp;

TEST_CASE("ItemOp", "[ItemOp]") {
    SECTION("Default Constructor") {
        ItemOp op;

        REQUIRE(op.getFlags() == 0 );
        REQUIRE(op.isCompatible(ItemOp::TYPE::CREATE));

        REQUIRE_FALSE(op.isCreate());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::NONE));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::CREATE));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::DELETE));
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockCreate());
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::BLOCK1_CREATE));
#endif
    }

    SECTION("NONE Function") {
        ItemOp op(ItemOp::TYPE::NONE);

        REQUIRE(op.getFlags() == 0);
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::NONE));
    }

    SECTION("isCreate Function") {
        ItemOp op(ItemOp::TYPE::CREATE);
        
        REQUIRE(op.isSupported(ItemOp::TYPE::CREATE));
        REQUIRE(op.isCreate());
        REQUIRE(op.getFlags() == 1);

        REQUIRE_FALSE(op.isDelete());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockCreate());
#endif
    }

    SECTION("isDelete Function") {
        ItemOp op(ItemOp::TYPE::DELETE);

        REQUIRE(op.isSupported(ItemOp::TYPE::DELETE));
        REQUIRE(op.isDelete());
        REQUIRE(op.getFlags() == 2);

        REQUIRE_FALSE(op.isCreate());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockCreate());
#endif
    }
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
    SECTION("isBlockCreate Function") {
        ItemOp op(ItemOp::TYPE::BLOCK1_CREATE);

        REQUIRE(op.isSupported(ItemOp::TYPE::BLOCK1_CREATE));
        REQUIRE(op.isBlockCreate());
        REQUIRE(op.getFlags() == 4);

        REQUIRE_FALSE(op.isCreate());
        REQUIRE_FALSE(op.isDelete());
    }

    SECTION("getFlags Function") {
        ItemOp op(ItemOp::TYPE::BLOCK1_CREATE | ItemOp::TYPE::DELETE | ItemOp::TYPE::CREATE);
        REQUIRE(op.getFlags() == 7);
    }

    SECTION("getFlags with duplicates TYPE Function") {
        ItemOp op(ItemOp::TYPE::BLOCK1_CREATE | ItemOp::TYPE::BLOCK1_CREATE | ItemOp::TYPE::BLOCK1_CREATE);
        REQUIRE(op.getFlags() == 4);
    }
#endif
    SECTION("isCompatible Function") {
        ItemOp op1(ItemOp::TYPE::NONE   | ItemOp::TYPE::CREATE);
        ItemOp op2(ItemOp::TYPE::CREATE | ItemOp::TYPE::DELETE);

        REQUIRE(op1.isCompatible(op2));
        REQUIRE_FALSE(op2.isCompatible(op1));

#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        ItemOp op3(ItemOp::TYPE::DELETE | ItemOp::TYPE::BLOCK1_CREATE);
        REQUIRE_FALSE(op1.isCompatible(op3));
#endif
    }

    SECTION("asVector Function") {
        ItemOp op(
            ItemOp::TYPE::NONE          |
            ItemOp::TYPE::CREATE        |
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
            ItemOp::TYPE::BLOCK1_CREATE |
#endif
            ItemOp::TYPE::DELETE);
        std::vector<ItemOp::TYPE> result = op.asVector();
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE(result.size() == 3);
#else
        REQUIRE(result.size() == 2);
#endif
        REQUIRE(result[0] == ItemOp::TYPE::CREATE);
        REQUIRE(result[1] == ItemOp::TYPE::DELETE);
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE(result[2] == ItemOp::TYPE::BLOCK1_CREATE);
#endif
    }
}
