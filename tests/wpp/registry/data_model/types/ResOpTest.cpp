#include "catch_amalgamated.hpp"
#include "ResOp.h"

using namespace wpp;

TEST_CASE("ResOp", "[ResOp]") {
    SECTION("Default Constructor") {
        ResOp op;
        REQUIRE(op.getFlags() == 0);

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isDelete());        
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockWrite());
        REQUIRE_FALSE(op.isBlockExecute());
#endif
        REQUIRE_FALSE(op.isSupported(ResOp::TYPE::NONE));
        REQUIRE_FALSE(op.isSupported(ResOp::TYPE::READ));
        REQUIRE_FALSE(op.isSupported(ResOp::TYPE::WRITE));
        REQUIRE_FALSE(op.isSupported(ResOp::TYPE::DELETE));
        REQUIRE_FALSE(op.isSupported(ResOp::TYPE::EXECUTE));
        REQUIRE_FALSE(op.isSupported(ResOp::TYPE::DISCOVER));
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isSupported(ResOp::TYPE::BLOCK_WRITE));
        REQUIRE_FALSE(op.isSupported(ResOp::TYPE::BLOCK_EXECUTE));
#endif
    }

    SECTION("isRead Function") {
        ResOp op(ResOp::TYPE::READ);
        
        REQUIRE(op.isRead());
        REQUIRE(op.getFlags() == 1);
        REQUIRE(op.isSupported(ResOp::TYPE::READ));

        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockWrite());
        REQUIRE_FALSE(op.isBlockExecute());
#endif
    }

    SECTION("isWrite(WRITE_UPD) Function") {
        ResOp op(ResOp::TYPE::WRITE_UPD);

        REQUIRE(op.isWrite());
        REQUIRE(op.getFlags() == 2);
        REQUIRE(op.isSupported(ResOp::TYPE::WRITE_UPD)); 

        REQUIRE_FALSE(op.getFlags() == 4);
        REQUIRE_FALSE(op.getFlags() == 8);
    }
    SECTION("isWrite(WRITE_REPLACE_RES) Function") {
        ResOp op(ResOp::TYPE::WRITE_REPLACE_RES);

        REQUIRE(op.isWrite());
        REQUIRE(op.getFlags() == 4);
        REQUIRE(op.isSupported(ResOp::TYPE::WRITE_REPLACE_RES));

        REQUIRE_FALSE(op.getFlags() == 2);
        REQUIRE_FALSE(op.getFlags() == 8);
    }
    SECTION("isWrite(WRITE_REPLACE_INST) Function") {
        ResOp op(ResOp::TYPE::WRITE_REPLACE_INST);

        REQUIRE(op.isWrite());
        REQUIRE(op.getFlags() == 8);
        REQUIRE(op.isSupported(ResOp::TYPE::WRITE_REPLACE_INST));

        REQUIRE_FALSE(op.getFlags() == 2);
        REQUIRE_FALSE(op.getFlags() == 4);
    }

    SECTION("isWrite Function") {
        ResOp op(ResOp::TYPE::WRITE);
        
        REQUIRE(op.isWrite());
        REQUIRE(op.getFlags() == 14);
        REQUIRE(op.isSupported(ResOp::TYPE::WRITE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockWrite());
        REQUIRE_FALSE(op.isBlockExecute());
#endif
        REQUIRE_FALSE((op.getFlags() == 2));
        REQUIRE_FALSE((op.getFlags() == 8));
    }
    SECTION("isExecute Function") {
        ResOp op(ResOp::TYPE::EXECUTE);
        
        REQUIRE(op.isExecute());
        REQUIRE(op.getFlags() == 16);
        REQUIRE(op.isSupported(ResOp::TYPE::EXECUTE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isDiscover());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockWrite());
        REQUIRE_FALSE(op.isBlockExecute());
#endif
    }
    SECTION("isDiscover Function") {
        ResOp op(ResOp::TYPE::DISCOVER);

        REQUIRE(op.isDiscover());
        REQUIRE(op.getFlags() == 32);
        REQUIRE(op.isSupported(ResOp::TYPE::DISCOVER));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE( op.isDelete());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockWrite());
        REQUIRE_FALSE(op.isBlockExecute());
#endif
    }
    SECTION("isDelete Function") {
        ResOp op(ResOp::TYPE::DELETE);

        REQUIRE(op.isDelete());
        REQUIRE(op.getFlags() == 64);
        REQUIRE(op.isSupported(ResOp::TYPE::DELETE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE_FALSE(op.isBlockWrite());
        REQUIRE_FALSE(op.isBlockExecute());
#endif
    }

#ifdef LWM2M_RAW_BLOCK1_REQUESTS
   SECTION("isBlockWrite Function") {
        ResOp op(ResOp::TYPE::BLOCK_WRITE);

        REQUIRE(op.isBlockWrite());
        REQUIRE(op.getFlags() == 128);
        REQUIRE(op.isSupported(ResOp::TYPE::BLOCK_WRITE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
        REQUIRE_FALSE(op.isBlockExecute());
    }
   SECTION("isBlockExecute Function") {
        ResOp op(ResOp::TYPE::BLOCK_EXECUTE);

        REQUIRE(op.isBlockExecute());
        REQUIRE(op.getFlags() == 256);
        REQUIRE(op.isSupported(ResOp::TYPE::BLOCK_EXECUTE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isDelete());        
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
        REQUIRE_FALSE(op.isBlockWrite());
    }
#endif
    SECTION("EXECUTE|WRITE Function") {
        ResOp op(ResOp::TYPE::EXECUTE | ResOp::TYPE::WRITE);

        REQUIRE(op.isWrite());
        REQUIRE(op.isExecute());
        REQUIRE(op.getFlags() == 30);
    }
    SECTION("READ|WRITE_REPLACE_RES Function") {
        ResOp op(ResOp::TYPE::READ | ResOp::TYPE::WRITE_REPLACE_RES);

        REQUIRE(op.isRead());
        REQUIRE(op.isWrite());
        REQUIRE(op.getFlags() == 5);
    }
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
    SECTION("DISCOVER|BLOCK_EXECUTE Function") {
        ResOp op(ResOp::TYPE::DISCOVER | ResOp::TYPE::BLOCK_EXECUTE);

        REQUIRE(op.isDiscover());
        REQUIRE(op.isBlockExecute());
        REQUIRE(op.getFlags() == 288);
    }
    SECTION("BLOCK_WRITE|DELETE Function") {
        ResOp op(ResOp::TYPE::BLOCK_WRITE | ResOp::TYPE::DELETE | ResOp::TYPE::WRITE_REPLACE_INST);
        
        REQUIRE(op.isBlockWrite());
        REQUIRE(op.isDelete());
        REQUIRE(op.getFlags() == 200);
    }
#endif
    SECTION("DELETE with duplicates TYPE Function") {
        ResOp op(ResOp::TYPE::DELETE | ResOp::TYPE::DELETE | ResOp::TYPE::DELETE);
                REQUIRE(op.getFlags() == 64);
    }

    SECTION("isCompatible Function") {
        ResOp op1(ResOp::TYPE::NONE        | ResOp::TYPE::WRITE);
        ResOp op2(ResOp::TYPE::WRITE       | ResOp::TYPE::EXECUTE);
        ResOp op3(ResOp::TYPE::DISCOVER    | ResOp::TYPE::DELETE);

        REQUIRE(op1.isCompatible(op2));
        REQUIRE_FALSE(op2.isCompatible(op1));
        REQUIRE_FALSE(op1.isCompatible(op3));
        REQUIRE_FALSE(op2.isCompatible(op3));
    }

    SECTION("asVector Function") {
        ResOp op(
            ResOp::TYPE::NONE           |
            ResOp::TYPE::READ           |
            ResOp::TYPE::WRITE          |
            ResOp::TYPE::EXECUTE        |
            ResOp::TYPE::DISCOVER       |
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
            ResOp::TYPE::BLOCK_EXECUTE  |
            ResOp::TYPE::BLOCK_WRITE    |
#endif
            ResOp::TYPE::DELETE);
        std::vector<ResOp::TYPE> result = op.asVector();
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE(result.size() == 9);
#else
        REQUIRE(result.size() == 7);
#endif
        REQUIRE(result[0] == ResOp::TYPE::READ);
        REQUIRE(result[1] == ResOp::TYPE::WRITE_UPD);
        REQUIRE(result[2] == ResOp::TYPE::WRITE_REPLACE_RES);
        REQUIRE(result[3] == ResOp::TYPE::WRITE_REPLACE_INST);
        REQUIRE(result[4] == ResOp::TYPE::EXECUTE);
        REQUIRE(result[5] == ResOp::TYPE::DISCOVER);
        REQUIRE(result[6] == ResOp::TYPE::DELETE);
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE(result[7] == ResOp::TYPE::BLOCK_WRITE);
        REQUIRE(result[8] == ResOp::TYPE::BLOCK_EXECUTE);
#endif
    }

    SECTION("asVector2 Function") {
        ResOp op(
            ResOp::TYPE::READ               |
            ResOp::TYPE::WRITE_UPD          |
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
            ResOp::TYPE::BLOCK_WRITE        |
#endif
            ResOp::TYPE::WRITE_REPLACE_INST);
        std::vector<ResOp::TYPE> result = op.asVector();
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE(result.size() == 4);
#else
        REQUIRE(result.size() == 3);
#endif
        REQUIRE(result[0] == ResOp::TYPE::READ);
        REQUIRE(result[1] == ResOp::TYPE::WRITE_UPD);
        REQUIRE(result[2] == ResOp::TYPE::WRITE_REPLACE_INST);
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
        REQUIRE(result[3] == ResOp::TYPE::BLOCK_WRITE);
#endif
    }
}