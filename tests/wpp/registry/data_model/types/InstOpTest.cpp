#include "catch_amalgamated.hpp"
#include "InstOp.h"

using namespace wpp;

TEST_CASE("InstOp", "[InstOp]") {
    SECTION("Default Constructor") {
        InstOp op;

        REQUIRE(op.getFlags() == 0 );
        REQUIRE(op.isCompatible(InstOp::TYPE::CREATE));

        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::NONE));
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::CREATE)          & (op.isCreate()));
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::DELETE)          & (op.isDelete()));
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::BLOCK1_CREATE)   & (op.isBlockCreate()));
    }

    SECTION("NONE Function") {
        InstOp op(InstOp::TYPE::NONE);

        REQUIRE(op.getFlags() == 0);
        REQUIRE_FALSE(op.isSupported(InstOp::TYPE::NONE));
    }

    SECTION("isCreate Function") {
        InstOp op(InstOp::TYPE::CREATE);
        
        REQUIRE(op.isSupported(InstOp::TYPE::CREATE));
        REQUIRE(op.isCreate() & (op.getFlags() == 1));

        REQUIRE_FALSE(op.isDelete() & (op.isBlockCreate()));
    }

    SECTION("isDelete Function") {
        InstOp op(InstOp::TYPE::DELETE);

        REQUIRE(op.isSupported(InstOp::TYPE::DELETE));
        REQUIRE(op.isDelete() & (op.getFlags() == 2 ));

        REQUIRE_FALSE(op.isCreate() & (op.isBlockCreate()));
    }

    SECTION("isBlockCreate Function") {
        InstOp op(InstOp::TYPE::BLOCK1_CREATE);

        REQUIRE(op.isSupported(InstOp::TYPE::BLOCK1_CREATE));
        REQUIRE(op.isBlockCreate() & (op.getFlags() == 4));

        REQUIRE_FALSE(op.isCreate() & (op.isDelete()));
    }

    SECTION("getFlags Function") {
        InstOp op(InstOp::TYPE::BLOCK1_CREATE | InstOp::TYPE::DELETE | InstOp::TYPE::CREATE);
        REQUIRE(op.getFlags() == 7);
    }

    SECTION("getFlags with duplicates TYPE Function") {
        InstOp op(InstOp::TYPE::BLOCK1_CREATE | InstOp::TYPE::BLOCK1_CREATE | InstOp::TYPE::BLOCK1_CREATE);
        REQUIRE(op.getFlags() == 4);
    }

    SECTION("isCompatible Function") {
        InstOp op1(InstOp::TYPE::NONE   | InstOp::TYPE::CREATE);
        InstOp op2(InstOp::TYPE::CREATE | InstOp::TYPE::DELETE);
        InstOp op3(InstOp::TYPE::DELETE | InstOp::TYPE::BLOCK1_CREATE);

        REQUIRE(op1.isCompatible(op2));
        REQUIRE_FALSE(op1.isCompatible(op3));
    }

    SECTION("asVector Function") {
        InstOp op(
            InstOp::TYPE::NONE   |
            InstOp::TYPE::CREATE |
            InstOp::TYPE::DELETE |
            InstOp::TYPE::BLOCK1_CREATE);
        std::vector<InstOp::TYPE> result = op.asVector();

        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == InstOp::TYPE::CREATE);
        REQUIRE(result[1] == InstOp::TYPE::DELETE);
        REQUIRE(result[2] == InstOp::TYPE::BLOCK1_CREATE);
    }
}
