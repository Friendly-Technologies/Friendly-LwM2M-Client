#include "catch_amalgamated.hpp"
#include "InstOp.h"

using namespace wpp;

TEST_CASE("InstOp", "[InstOp]") {
    SECTION("Default Constructor") {
        InstOp op;

        REQUIRE(op.getFlags() == 0 );
        REQUIRE(op.isCompatible(InstOp::TYPE::CREATE));

        REQUIRE_FALSE(op.isCreate());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::NONE));
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::CREATE));
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::DELETE));
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockCreate());
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::BLOCK1_CREATE));
#endif
    }

    SECTION("NONE Function") {
        InstOp op(InstOp::TYPE::NONE);

        REQUIRE(op.getFlags() == 0);
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::NONE));
    }

    SECTION("isCreate Function") {
        InstOp op(InstOp::TYPE::CREATE);
        
        REQUIRE(op.isSupported(InstOp::TYPE::CREATE));
        REQUIRE(op.isCreate());
        REQUIRE(op.getFlags() == 1);

        REQUIRE_FALSE(op.isDelete());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockCreate());
#endif
    }

    SECTION("isDelete Function") {
        InstOp op(InstOp::TYPE::DELETE);

        REQUIRE(op.isSupported(InstOp::TYPE::DELETE));
        REQUIRE(op.isDelete());
        REQUIRE(op.getFlags() == 2);

        REQUIRE_FALSE(op.isCreate());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockCreate());
#endif
    }
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
    SECTION("isBlockCreate Function") {
        InstOp op(InstOp::TYPE::BLOCK1_CREATE);

        REQUIRE(op.isSupported(InstOp::TYPE::BLOCK1_CREATE));
        REQUIRE(op.isBlockCreate());
        REQUIRE(op.getFlags() == 4);

        REQUIRE_FALSE(op.isCreate());
        REQUIRE_FALSE(op.isDelete());
    }

    SECTION("getFlags Function") {
        InstOp op(InstOp::TYPE::BLOCK1_CREATE | InstOp::TYPE::DELETE | InstOp::TYPE::CREATE);
        REQUIRE(op.getFlags() == 7);
    }

    SECTION("getFlags with duplicates TYPE Function") {
        InstOp op(InstOp::TYPE::BLOCK1_CREATE | InstOp::TYPE::BLOCK1_CREATE | InstOp::TYPE::BLOCK1_CREATE);
        REQUIRE(op.getFlags() == 4);
    }
#endif
    SECTION("isCompatible Function") {
        InstOp op1(InstOp::TYPE::NONE   | InstOp::TYPE::CREATE);
        InstOp op2(InstOp::TYPE::CREATE | InstOp::TYPE::DELETE);

        REQUIRE(op1.isCompatible(op2));
        REQUIRE_FALSE(op2.isCompatible(op1));

#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        InstOp op3(InstOp::TYPE::DELETE | InstOp::TYPE::BLOCK1_CREATE);
        REQUIRE_FALSE(op1.isCompatible(op3));
#endif
    }

    SECTION("asVector Function") {
        InstOp op(
            InstOp::TYPE::NONE          |
            InstOp::TYPE::CREATE        |
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
            InstOp::TYPE::BLOCK1_CREATE |
#endif
            InstOp::TYPE::DELETE);
        std::vector<InstOp::TYPE> result = op.asVector();
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE(result.size() == 3);
#else
        REQUIRE(result.size() == 2);
#endif
        REQUIRE(result[0] == InstOp::TYPE::CREATE);
        REQUIRE(result[1] == InstOp::TYPE::DELETE);
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE(result[2] == InstOp::TYPE::BLOCK1_CREATE);
#endif
    }
}
